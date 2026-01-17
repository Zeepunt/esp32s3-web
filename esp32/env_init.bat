::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: SPDX-License-Identifier: Apache-2.0
:: SPDX-FileCopyrightText: 2026 Zeepunt
::
:: ESP-IDF 编译环境初始化
::
:: 修改说明:
:: 1. ESP_IDF_ROOT 是 ESP-IDF 的安装目录
:: 2. ESP_IDF_ID 是 $(ESP_IDF_ROOT)/esp_idf.json 文件的 idfSelectedId 字段
:: 3. IDF_PATH 是 ESP_IDF_ROOT/frameworks 里面的 SDK 目录名称
::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
@echo off

setlocal

:: ESP-IDF v5.5
set ESP_IDF_ROOT=D:\ESP-IDF\v5.5
set ESP_IDF_ID=esp-idf-a32e7a2615ce72436e57557d51e55eb4
set IDF_PATH=%ESP_IDF_ROOT%\frameworks\esp-idf-v5.5

powershell -ExecutionPolicy Bypass -NoExit -Command "$env:IDF_PATH='%IDF_PATH%'; . '%ESP_IDF_ROOT%\Initialize-Idf.ps1' -IdfId '%ESP_IDF_ID%'"

endlocal