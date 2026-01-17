/*
 * main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mod_nvs.h"
#include "mod_cmd.h"
#include "mod_fs.h"
#include "mod_network.h"
#include "http_server.h"

static const char *TAG = "main";

void app_main(void)
{
	/* WIFI 模块依赖 NVS 模块 */
	mod_nvs_init();
	mod_cmd_init();
	mod_fs_init(MOD_FS_DEFAULT);
	mod_network_init();

	http_server_init();

	while (1) {
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
