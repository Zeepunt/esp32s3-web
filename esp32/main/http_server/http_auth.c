/*
 * http_auth.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "esp_err.h"
#include "esp_log.h"

#include "base64.h"
#include "http_auth.h"

#define HTTP_AUTH_USER_LEN    32
#define HTTP_AUTH_BUF_LEN     4096

typedef struct {
    char name[HTTP_AUTH_USER_LEN];
    char password[HTTP_AUTH_USER_LEN];
} http_auth_user_t;

static const char *TAG = "httpd_auth";

static http_auth_user_t s_user_info[] = {
    {"admin", "88888888"},
    {"test",  "12345678"},
};

static bool priv_auth_validate(const char *token, size_t token_len)
{
    bool valid = false;
    char *buf = NULL;

    if ((token == NULL) || (token_len == 0)) {
        valid = false;
        goto exit;
    }

    buf = (char *)calloc(1, token_len * 2);
    if (buf == NULL) {
        valid = false;
        goto exit;
    }

    if (b64_to_bin(token, token_len, (uint8_t *)buf, token_len * 2) < 0) {
        ESP_LOGE(TAG, "base64 decode failed");
        valid = false;
        goto exit;
    }
    ESP_LOGI(TAG, "base decode: %s", buf);

    char *split = strchr(buf, ':');
    if (split == NULL) {
        ESP_LOGE(TAG, "invalid token");
        valid = false;
        goto exit;
    }
    *split = '\0';

    char *name = buf;
    char *password = split + 1;

    for (int i = 0; i < (sizeof(s_user_info) / sizeof(s_user_info[0])); i++) { 
        if ((strncmp(name, s_user_info[i].name, strlen(s_user_info[i].name)) == 0) &&
            (strncmp(password, s_user_info[i].password, strlen(s_user_info[i].password)) == 0)) {
            valid = true;
            break;
        }
    }

exit:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return valid;
}

bool http_auth_validate(httpd_req_t *req)
{
    bool valid = false;
    esp_err_t err = ESP_OK;
    
    char *buf = NULL;
    size_t buf_len = 0;

    if (req == NULL) {
        valid = false;
        goto exit;
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
    if (buf_len == 1) {
        ESP_LOGE(TAG, "Authorization header not received");
        valid = false;
        goto exit;
    }

    buf = (char *)calloc(1, buf_len);
    if (buf == NULL) {
        ESP_LOGE(TAG, "malloc failed");
        valid = false;
        goto exit;
    }

    err = httpd_req_get_hdr_value_str(req, "Authorization", buf, buf_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Authorization header not found");
        valid = false;
        goto exit;
    }
    ESP_LOGI(TAG, "Authorization string: %s", buf);

    size_t prefix_len = strlen("Basic ");
    valid = priv_auth_validate(buf + prefix_len, strlen(buf) - prefix_len);
    if (valid) {
        ESP_LOGI(TAG, "Authorization success");
    } else {
        ESP_LOGE(TAG, "Authorization failed");
    }

exit:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    if (!valid) {
        httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"\"");
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, NULL);
    }

    return valid;
}
