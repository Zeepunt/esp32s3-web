/*
 * mod_nvs.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __MOD_NVS_H__
#define __MOD_NVS_H__ 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize NVS Module
 * @return
 *  - 0: success
 *  - -1: failure
 */
int mod_nvs_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MOD_NVS_H__ */
