/*
 * http_uri_index.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __HTTP_URI_INDEX_H__
#define __HTTP_URI_INDEX_H__ 

#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief httpd `/index` uri handler
 * @return esp_err_t
 */
esp_err_t http_server_uri_index_handle(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_URI_INDEX_H__ */
