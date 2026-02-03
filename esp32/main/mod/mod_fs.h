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

/**
 * @brief Open file
 * @param type File system type
 * @param path File path
 * @param mode File mode
 * @return
 *  - File pointer: success
 *  - NULL: failure
 */
FILE *mod_fs_open(mod_fs_type_t type, const char *path, const char *mode);

/**
 * @brief Close file
 * @param fp File pointer
 */
void mod_fs_close(FILE *fp);

/**
 * @brief Read file
 * @param fp File pointer
 * @param buf Buffer
 * @param size Buffer size
 * @return
 *  - Number of bytes read: success
 *  - 0: end of file
 *  - -1: failure
 */
size_t mod_fs_read(FILE *fp, void *buf, size_t size);

/**
 * @brief Write file
 * @param fp File pointer
 * @param buf Buffer
 * @param size Buffer size
 * @return
 *  - Number of bytes written: success
 *  - -1: failure
 */
size_t mod_fs_write(FILE *fp, const void *buf, size_t size);

/**
 * @brief Read file
 * @param type File system type
 * @param path File path
 * @return
 *  - Buffer pointer: success
 *  - NULL: failure
 */
void *mod_fs_file_read(mod_fs_type_t type, const char *path);

/**
 * @brief Write file
 * @param type File system type
 * @param path File path
 * @param buf Buffer
 * @param size Buffer size
 * @return
 *  - 0: success
 *  - -1: failure
 */
int mod_fs_file_write(mod_fs_type_t type, const char *path, const void *buf, size_t size);

/**
 * @brief Free buffer
 * @param buf Buffer pointer
 */
void mod_fs_buf_free(void *buf);

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
