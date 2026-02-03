/*
 * http_uri_system.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __HTTP_URI_SYSTEM_H__
#define __HTTP_URI_SYSTEM_H__ 

#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief httpd `/system` uri handler
 * @return esp_err_t
 */
esp_err_t http_server_uri_system_handle(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_URI_SYSTEM_H__ */