#pragma once

#include <generator.h>

extern generator_t x86_microos_fasm;
extern generator_t x86_64_linux_gas;
extern generator_t x86_64_linux_nasm;
extern generator_t aarch64_linux_gas;
extern generator_t riscv64_linux_gas;
extern generator_t phoenix_none_as;
extern generator_t phoenixv2_none_as;
extern generator_t any_any_gcc;
extern generator_t any_any_firestorm;
extern generator_t armv6_pico_gcc;
extern generator_t armv6_pico_gas;
extern generator_t armv7m_stm32_gas;
extern generator_t avr_m328p_avra;
extern generator_t avr_t85_avra;
extern generator_t riscv_esp32_gas;
extern generator_t xtensa_esp32_gas;
