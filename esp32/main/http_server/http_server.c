/*
 * http_server.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_http_server.h"

#include "http_uri_index.h"

#define HTTP_SERVER_PORT    80

static const char *TAG = "httpd";

static httpd_handle_t s_httpd_handle = NULL;

static const httpd_uri_t s_index_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = http_server_uri_index_handle,
    .user_ctx = NULL,
};

static const httpd_uri_t s_assets_uri = {
    .uri = "/assets/*",
    .method = HTTP_GET,
    .handler = http_server_uri_index_handle,
    .user_ctx = NULL,
};

int http_server_init(void)
{
    esp_err_t err = ESP_OK;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.lru_purge_enable = true;
    config.max_uri_handlers = 15;
    /* URI 使用通配符匹配 */
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.server_port = HTTP_SERVER_PORT;
    config.stack_size = 20 * 1024;

    ESP_LOGI(TAG, "http server start on port: %d", config.server_port);

    err = httpd_start(&s_httpd_handle, &config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "start http server failed: %s", esp_err_to_name(err));
        return -1;
    }

    httpd_register_uri_handler(s_httpd_handle, &s_index_uri);
    httpd_register_uri_handler(s_httpd_handle, &s_assets_uri);

    return 0;
}
