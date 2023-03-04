#include "generator.h"
#include <targets.h>

void xtensa_esp32_gas_emit_initial_setup(FILE* f) {
	emit(".section .bss", f, 0);
	emit("tape: .space 30000", f, 0);
	emit(".section .text", f, 0);
	emit(".global app_main", f, 0);
	emit(".extern putchar", f, 0);
	emit(".extern getchar", f, 0);
	emit("app_main:", f, 0);
	emit("entry a1, 0", f, 1);
	emit("mov a2, a0", f, 1); // return address
	emit("movi a3, tape", f, 1);
}
void xtensa_esp32_gas_emit_clean_exit(FILE* f) {
	emit("mov a0, a2", f, 1);
	emit("movi a2, 0", f, 1);
	emit("retw", f, 1);
}

void xtensa_esp32_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi a3, a3, %d", amount * 4);
	emit(buf, f, 1);
}
void xtensa_esp32_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi a3, a3, -%d", amount * 4);
	emit(buf, f, 1);
}

void xtensa_esp32_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("l32i a4, a3, 0", f, 1);
	sprintf(buf, "addi a4, a4, %d", amount);
	emit(buf, f, 1);
	emit("s32i a4, a3, 0", f, 1);
}
void xtensa_esp32_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("l32i a4, a3, 0", f, 1);
	sprintf(buf, "addi a4, a4, -%d", amount);
	emit(buf, f, 1);
	emit("s32i a4, a3, 0", f, 1);
}

void xtensa_esp32_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("l32i a4, a3, 0", f, 1);
	sprintf(buf, "beqz a4, _%d_loop_exit", id);
	emit(buf, f, 1);
}
void xtensa_esp32_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "j _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void xtensa_esp32_gas_emit_stdin_read(FILE* f) {
	emit("call8 getchar", f, 1);
	emit("s32i a10, a3, 0", f, 1);
}
void xtensa_esp32_gas_emit_stdout_write(FILE* f) {
	emit("l32i a10, a3, 0", f, 1);
	emit("call8 putchar", f, 1);
}

generator_t xtensa_esp32_gas = {
	.name = "xtensa-esp32-gas",
	.emit_initial_setup = xtensa_esp32_gas_emit_initial_setup,
	.emit_clean_exit = xtensa_esp32_gas_emit_clean_exit,
	.emit_ptr_plus = xtensa_esp32_gas_emit_ptr_plus,
	.emit_ptr_minus = xtensa_esp32_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = xtensa_esp32_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = xtensa_esp32_gas_emit_ptr_deref_minus,
	.emit_loop_begin = xtensa_esp32_gas_emit_loop_begin,
	.emit_loop_end = xtensa_esp32_gas_emit_loop_end,
	.emit_stdin_read = xtensa_esp32_gas_emit_stdin_read,
	.emit_stdout_write = xtensa_esp32_gas_emit_stdout_write
};