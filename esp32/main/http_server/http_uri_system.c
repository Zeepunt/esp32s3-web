/*
 * http_uri_system.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "esp_err.h"
#include "esp_log.h"

#include "http_auth.h"
#include "http_uri_system.h"

static const char *TAG = "httpd_system";

static esp_err_t priv_login_handle(httpd_req_t *req)
{
    if (req->method != HTTP_POST) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        return ESP_OK;
    }

    if (!http_auth_validate(req)) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t http_server_uri_system_handle(httpd_req_t *req)
{
    ESP_LOGI(TAG, "uri: %s", req->uri);

    if (strcmp(req->uri, "/system/login") == 0) {
        return priv_login_handle(req);
    }

    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, NULL);

    return ESP_OK;
}
