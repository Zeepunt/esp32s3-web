set(mod_src
    "mod/mod_nvs.c"
    "mod/mod_cmd.c"
    "mod/mod_fs.c"
    "mod/mod_network.c"
)

set(http_server_src
    "http_server/http_server.c"
    "http_server/http_auth.c"
    "http_server/http_uri_index.c"
    "http_server/http_uri_system.c"
)

idf_component_register(SRCS
    "main.c"
    "${mod_src}"
    "${http_server_src}"
    INCLUDE_DIRS "."
    INCLUDE_DIRS "mod"
    INCLUDE_DIRS "http_server"
)

partition_table_get_partition_info(partition_subtype "--partition-name fs" "subtype")
if(partition_subtype STREQUAL "130")     # 0x82 - ESP_PARTITION_SUBTYPE_DATA_SPIFFS
    message(STATUS "Partition 'fs' is SPIFFS type, creating SPIFFS image")
    spiffs_create_partition_image(fs ../fs FLASH_IN_PROJECT)
elseif(partition_subtype STREQUAL "131") # 0x83 - ESP_PARTITION_SUBTYPE_DATA_LITTLEFS
    message(STATUS "Partition 'fs' is LittleFS type, creating LittleFS image")
    littlefs_create_partition_image(fs ../fs FLASH_IN_PROJECT)
elseif(partition_subtype STREQUAL "129") # 0x81 - ESP_PARTITION_SUBTYPE_DATA_FAT
    message(STATUS "Partition 'fs' is FAT type")
else()
    message(FATAL_ERROR "Partition 'fs' has unknown subtype: '${partition_subtype}'")
endif()
