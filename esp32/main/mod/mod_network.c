/*
 * mod_network.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdint.h>

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

static const char *TAG = "mod_network";

static esp_netif_t *s_wifi_sta = NULL;
static esp_netif_t *s_wifi_ap = NULL;

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
            ESP_LOGI(TAG, "STA new ip:" IPSTR, IP2STR(&event->ip_info.ip));
            break;
        }

        case IP_EVENT_ETH_GOT_IP: {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "ETH new ip:" IPSTR, IP2STR(&event->ip_info.ip));
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
