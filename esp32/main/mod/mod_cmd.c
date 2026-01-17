/*
 * mod_cmd.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "argtable3/argtable3.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_console.h"

#include "mod_cmd.h"

#define CMD_PROMPT    "cmd>"

static const char *TAG = "mod_cmd";

static esp_console_repl_t *s_repl = NULL;

int mod_cmd_init(void)
{
    if (s_repl != NULL) {
        ESP_LOGI(TAG, "CMD already initialized");
        return 0;
    }

    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.task_stack_size = 4 * 1024;
    repl_config.prompt = CMD_PROMPT;

    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &s_repl));
    ESP_ERROR_CHECK(esp_console_start_repl(s_repl));

    return 0;
}
