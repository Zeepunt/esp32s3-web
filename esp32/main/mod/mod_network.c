/*
 * mod_network.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdint.h>

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_wifi.h"

#include "mod_network.h"

#define WIFI_AP_CHANNEL        1
#define WIFI_AP_CONNECT_NUM    10
#define WIFI_AP_SSID           "esp32_web"
#define WIFI_AP_PASSWORD       "esp32_web"

/* SPI 以太网硬件配置 */
#define SPI_ETH_HOST           SPI3_HOST
#define SPI_ETH_CLOCK_HZ       (20 * 1000 * 1000)

#define SPI_ETH_CS_GPIO        14
#define SPI_ETH_SCLK_GPIO      18
#define SPI_ETH_MISO_GPIO      17
#define SPI_ETH_MOSI_GPIO      16

#define SPI_ETH_INT_GPIO       19
#define SPI_ETH_RESET_GPIO     20

static const char *TAG = "mod_network";

static esp_netif_t *s_wifi_sta = NULL;
static esp_netif_t *s_wifi_ap = NULL;

static esp_netif_t *s_eth = NULL;
static esp_eth_mac_t *s_eth_mac = NULL;
static esp_eth_phy_t *s_eth_phy = NULL;
static esp_eth_handle_t s_eth_handle = NULL;
static esp_eth_netif_glue_handle_t s_eth_glue = NULL;

static void priv_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGE(TAG, "STA connected");
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGE(TAG, "STA disconnected");
                esp_wifi_connect();
                break;

            default:
                break;
        }
    } else if (event_base == ETH_EVENT) {
        switch (event_id) {
            case ETHERNET_EVENT_CONNECTED:
                ESP_LOGE(TAG, "ETH connected");
                break;

            case ETHERNET_EVENT_DISCONNECTED:
                ESP_LOGE(TAG, "ETH disconnected");
                esp_wifi_connect();
                break;

            default:
                break;
        }
    }
}

static void priv_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base != IP_EVENT) {
        return;
    }

    switch (event_id) {
        case IP_EVENT_STA_GOT_IP: {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "STA new ip: " IPSTR, IP2STR(&event->ip_info.ip));
            break;
        }

        case IP_EVENT_ETH_GOT_IP: {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "ETH new ip: " IPSTR, IP2STR(&event->ip_info.ip));
            break;
        }

        default:
            break;
    }
}

static void priv_net_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &priv_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &priv_ip_event_handler, NULL));
}

static void priv_eth_init(void)
{
    esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH();
    s_eth = esp_netif_new(&netif_cfg);
    if (s_eth == NULL) {
        ESP_LOGE(TAG, "esp_netif_new failed");
        return;
    }

#if defined(CONFIG_ETH_USE_SPI_ETHERNET)
    /* SPI Init */
    gpio_install_isr_service(0);
    spi_bus_config_t spi_bus_cfg = {
        .miso_io_num = SPI_ETH_MISO_GPIO,
        .mosi_io_num = SPI_ETH_MOSI_GPIO,
        .sclk_io_num = SPI_ETH_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    if (spi_bus_initialize(SPI_ETH_HOST, &spi_bus_cfg, SPI_DMA_CH_AUTO) != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed");
        esp_netif_destroy(s_eth);
        s_eth = NULL;
        return;
    }

    spi_device_interface_config_t spi_dev_iface_cfg = {
        .mode = 0,
        .clock_speed_hz = SPI_ETH_CLOCK_HZ,
        .spics_io_num = SPI_ETH_CS_GPIO,
        .queue_size = 20,
    };
#endif

    /* MAC Init */
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
#if defined(CONFIG_ETH_SPI_ETHERNET_W5500)
    eth_w5500_config_t w5500_config = ETH_W5500_DEFAULT_CONFIG(SPI_ETH_HOST, &spi_dev_iface_cfg);
    w5500_config.int_gpio_num = SPI_ETH_INT_GPIO;

    s_eth_mac = esp_eth_mac_new_w5500(&w5500_config, &mac_config);
    if (s_eth_mac == NULL) {
        ESP_LOGE(TAG, "esp_eth_mac_new_w5500 failed");
    }
#endif

    /* PHY Init */
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
    phy_config.reset_gpio_num = SPI_ETH_RESET_GPIO;
#if defined(CONFIG_ETH_SPI_ETHERNET_W5500)
    s_eth_phy = esp_eth_phy_new_w5500(&phy_config);
    if (s_eth_phy == NULL) {
        ESP_LOGE(TAG, "esp_eth_phy_new_w5500 failed");
    }
#endif

    esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(s_eth_mac, s_eth_phy);
    ESP_ERROR_CHECK(esp_eth_driver_install(&eth_config, &s_eth_handle));

    /* Set MAC Address */
    uint8_t eth_mac[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(eth_mac, ESP_MAC_ETH));
    ESP_ERROR_CHECK(esp_eth_ioctl(s_eth_handle, ETH_CMD_S_MAC_ADDR, eth_mac));

    s_eth_glue = esp_eth_new_netif_glue(s_eth_handle);
    esp_netif_attach(s_eth, s_eth_glue);

    ESP_ERROR_CHECK(esp_eth_start(s_eth_handle));
}

static void priv_wifi_init(void)
{
    uint8_t mac[6] = {0};

    wifi_config_t ap_config = {0};
    wifi_config_t sta_config = {0};

    ESP_ERROR_CHECK(esp_efuse_mac_get_default(mac));
    ESP_LOGI(TAG, "WIFI MAC: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    s_wifi_sta = esp_netif_create_default_wifi_sta();
    s_wifi_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    ap_config.ap.channel = WIFI_AP_CHANNEL;
    ap_config.ap.max_connection = WIFI_AP_CONNECT_NUM;
    strncpy((char *)ap_config.ap.ssid, WIFI_AP_SSID, 32);
    strncpy((char *)ap_config.ap.password, WIFI_AP_PASSWORD, 64);
    if (strlen((char *)ap_config.ap.password) == 0) {
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    } else {
        ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    }
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

int mod_network_init(void)
{
    priv_net_init();
    priv_eth_init();
    priv_wifi_init();

    return 0;
}
