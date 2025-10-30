# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(FLASH_LOAD_OFFSET 0x90000000)
set(BOARD_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(prj-learn)

# src/led.c src/button.c
target_sources(app PRIVATE src/led.c src/button.c)
