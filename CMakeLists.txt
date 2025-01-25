#
# Copyright (c) 2024 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.20.0)

# Set BOARD to nrf52840dk_nrf52840 if not already set
set(BOARD nrf52840dk_nrf52840)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(icm42670_sample)

target_sources(app PRIVATE
    src/main.c
    src/icm42670.c
)
