/*
 * mod_cmd.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __MOD_CMD_H__
#define __MOD_CMD_H__ 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize Command Module
 * @return
 *  - 0: success
 *  - -1: failure
 */
int mod_cmd_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MOD_CMD_H__ */
