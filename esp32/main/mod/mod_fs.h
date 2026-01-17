/*
 * mod_fs.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __MOD_FS_H__
#define __MOD_FS_H__ 

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MOD_FS_DEFAULT  = 0,
    MOD_FS_SPIFFS   = 1,
    MOD_FS_LITTLEFS = 2,
    MOD_FS_FATFS    = 3,
} mod_fs_type_t;

FILE *mod_fs_open(mod_fs_type_t type, const char *path, const char *mode);
void mod_fs_close(FILE *fp);
size_t mod_fs_read(FILE *fp, void *buf, size_t size);
size_t mod_fs_write(FILE *fp, const void *buf, size_t size);

char *mod_fs_read_content(mod_fs_type_t type, const char *path);
void mod_fs_free_content(char *content);

/**
 * @brief Initialize File System Module
 * @return
 *  - 0: success
 *  - -1: failure
 */
int mod_fs_init(mod_fs_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* __MOD_FS_H__ */
