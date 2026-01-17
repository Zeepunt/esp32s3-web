/*
 * mod_nvs.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdbool.h>

#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"

#include "mod_nvs.h"

static const char *TAG = "mod_nvs";

static bool s_nvs_init_flag = false;

int mod_nvs_init(void)
{
    esp_err_t err = ESP_OK;

    if (s_nvs_init_flag) {
        ESP_LOGI(TAG, "NVS already initialized");
        return 0;
    }

    err = nvs_flash_init();
    if ((err == ESP_ERR_NVS_NO_FREE_PAGES) || (err == ESP_ERR_NVS_NEW_VERSION_FOUND)) {
        ESP_LOGW(TAG, "Erase NVS flash");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    if (err == ESP_OK) {
        s_nvs_init_flag = true;
        return 0;
    }

    ESP_LOGE(TAG, "NVS init failed: %s", esp_err_to_name(err));

    return -1;
}
