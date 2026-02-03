/*
 * http_auth.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __HTTP_AUTH_H__
#define __HTTP_AUTH_H__

#include <stdbool.h>
#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Validate the HTTP request
 * @param req HTTP request
 * @return
 *  - true: request is valid
 *  - false: request is invalid
 * @note This function will send HTTP response automatically if request is invalid
 */
bool http_auth_validate(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_AUTH_H__ */
