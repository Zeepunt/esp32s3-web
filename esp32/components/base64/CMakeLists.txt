cmake_minimum_required(VERSION 3.10)

file(GLOB SRCS ./*.c)

# 所在的目录名称就是 component 名称
idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS "./"
)