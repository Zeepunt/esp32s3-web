/*
 * mod_network.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __MOD_NETWORK_H__
#define __MOD_NETWORK_H__ 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize Network Module
 * @return
 *  - 0: success
 *  - -1: failure
 */
int mod_network_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MOD_NETWORK_H__ */
