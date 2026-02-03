/*
 * mod_fs.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_spiffs.h"
// #include "esp_littlefs.h"

#include "mod_fs.h"

#define FS_PARTITION_NAME    "fs"

#define SPIFFS_MOUNT_PATH    "/spiffs"

static const char *TAG = "mod_fs";

static const char *priv_get_subtype_str(esp_partition_subtype_t subtype)
{
    switch (subtype) {
        case ESP_PARTITION_SUBTYPE_DATA_SPIFFS:
            return "PARTITION_SPIFFS";

        case ESP_PARTITION_SUBTYPE_DATA_LITTLEFS:
            return "PARTITION_LITTLEFS";

        case ESP_PARTITION_SUBTYPE_DATA_FAT:
            return "PARTITION_FAT";

        default:
            return "PARTITION_UNKNOWN";
    }
}

static int priv_spiffs_init(void)
{
    esp_err_t err = ESP_OK;

    size_t total = 0;
    size_t used = 0;

    esp_vfs_spiffs_conf_t conf = {
        .base_path = SPIFFS_MOUNT_PATH,
        .partition_label = FS_PARTITION_NAME,
        .max_files = 20,
        .format_if_mount_failed = true,
    };

    err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS regiter failed: %s", esp_err_to_name(err));
        return -1;
    }

    err = esp_spiffs_info(conf.partition_label, &total, &used);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS info: total: %d, used: %d", total, used);
        return 0;
    }

    ESP_LOGE(TAG, "SPIFFS partition information get failed: %s", esp_err_to_name(err));
    return -1;
}

FILE *mod_fs_open(mod_fs_type_t type, const char *path, const char *mode)
{
    char real_path[128] = {0};

    if ((path == NULL) || (mode == NULL)) {
        return NULL;
    }

    if (type == MOD_FS_SPIFFS) {
        snprintf(real_path, sizeof(real_path) / sizeof(real_path[0]), "%s%s", SPIFFS_MOUNT_PATH, path);
    } else {
        snprintf(real_path, sizeof(real_path) / sizeof(real_path[0]), "%s", path);
    }

    return fopen(real_path, mode);
}

void mod_fs_close(FILE *fp)
{
    if (fp == NULL) {
        return;
    }

    fclose(fp);
}

size_t mod_fs_read(FILE *fp, void *buf, size_t size)
{
    if ((fp == NULL) || (buf == NULL) || (size == 0)) {
        return 0;
    }

    return fread(buf, 1, size, fp);
}

size_t mod_fs_write(FILE *fp, const void *buf, size_t size)
{
    if ((fp == NULL) || (buf == NULL) || (size == 0)) {
        return 0;
    }

    return fwrite(buf, 1, size, fp);
}

void *mod_fs_file_read(mod_fs_type_t type, const char *path)
{
    FILE *fp = NULL;

    char *buf = NULL;
    size_t size = 0;

    if (path == NULL) {
        return NULL;
    }

    fp = mod_fs_open(type, path, "r");
    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    buf = (char *)malloc(size + 1);
    if (buf == NULL) {
        mod_fs_close(fp);
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);
    fread(buf, 1, size, fp);
    buf[size] = '\0';
    mod_fs_close(fp);

    return buf;
}

int mod_fs_file_write(mod_fs_type_t type, const char *path, const void *buf, size_t size)
{
    FILE *fp = NULL;
    size_t write_len = 0;

    if ((path == NULL) || (buf == NULL) || (size == 0)) {
        return -1;
    }

    fp = mod_fs_open(type, path, "w");
    if (fp == NULL) {
        return -1;
    }

    write_len = fwrite(buf, 1, size, fp);
    mod_fs_close(fp);

    return write_len;
}

void mod_fs_buf_free(void *buf)
{
    if (buf == NULL) {
        return;
    }

    free(buf);
}

int mod_fs_init(mod_fs_type_t type)
{
    esp_partition_t *part = NULL;

    part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, FS_PARTITION_NAME);
    if (part == NULL) {
        ESP_LOGE(TAG, "Partition %s not found", FS_PARTITION_NAME);
        return -1;
    }
    ESP_LOGI(TAG, "Partition %s - %s", FS_PARTITION_NAME, priv_get_subtype_str(part->subtype));

    switch (type) {
        case MOD_FS_SPIFFS:
            if (part->subtype != ESP_PARTITION_SUBTYPE_DATA_SPIFFS) {
                return -1;
            }
            return priv_spiffs_init();
            break;

        case MOD_FS_LITTLEFS:
            if (part->subtype != ESP_PARTITION_SUBTYPE_DATA_LITTLEFS) {
                return -1;
            }
            break;

        case MOD_FS_FATFS:
            if (part->subtype != ESP_PARTITION_SUBTYPE_DATA_FAT) {
                return -1;
            }
            break;

        case MOD_FS_DEFAULT:
        default:
            if (part->subtype == ESP_PARTITION_SUBTYPE_DATA_SPIFFS) {
                return priv_spiffs_init();
            }
            break;
    }

    return 0;
}
