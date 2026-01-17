/*
 * http_uri_index.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "esp_err.h"
#include "esp_log.h"

#include "mod_fs.h"
#include "http_uri_index.h"

static const char *TAG = "httpd_index";

esp_err_t http_server_uri_index_handle(httpd_req_t *req)
{
    char *buf = NULL;

    ESP_LOGI(TAG, "uri: %s", req->uri);

    if (strcmp(req->uri, "/") == 0) {
        buf = mod_fs_read_content(MOD_FS_SPIFFS, "/index.html");
        httpd_resp_set_type(req, "text/html");
    } else if (strstr(req->uri, ".js") != NULL) {
        buf = mod_fs_read_content(MOD_FS_SPIFFS, req->uri);
        httpd_resp_set_type(req, "text/javascript");
    } else if (strstr(req->uri, ".css") != NULL) {
        buf = mod_fs_read_content(MOD_FS_SPIFFS, req->uri);
        httpd_resp_set_type(req, "text/css");
    }

    httpd_resp_send(req, buf, strlen(buf));
    mod_fs_free_content(buf);

    return ESP_OK;
}
