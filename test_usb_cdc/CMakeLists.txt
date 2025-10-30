# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# 指定自定义板级路径
set(BOARD_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

# 查找Zephyr包
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
# 定义项目名称
project(hello_usb_cdc)
# 包含USB公共配置
include(${ZEPHYR_BASE}/samples/subsys/usb/common/common.cmake)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
