/*
 * http_server.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize HTTP server
 * @return
 *  - 0: success
 *  - -1: failure
 */
int http_server_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_SERVER_H__ */
