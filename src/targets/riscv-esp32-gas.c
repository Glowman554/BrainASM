#include "generator.h"
#include <targets.h>

void riscv_esp32_gas_emit_initial_setup(FILE* f) {
	emit(".section .bss", f, false);
	emit("tape: .space 30000", f, false);
	emit(".section .text", f, false);
	emit(".global app_main", f, false);
	emit(".extern putchar", f, false);
	emit(".extern getchar", f, false);
	emit("app_main:", f, false);
	emit("mv t0, ra", f, true);
	emit("la t1, tape", f, true);
}
void riscv_esp32_gas_emit_clean_exit(FILE* f) {
	emit("mv ra, t0", f, true);
	emit("li a0, 0", f, true);
	emit("ret", f, true);
}

void riscv_esp32_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi t1, t1, %d", amount * 4);
	emit(buf, f, true);
}
void riscv_esp32_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi t1, t1, -%d", amount * 4);
	emit(buf, f, true);
}

void riscv_esp32_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("lw a5, (t1)", f, true);
	sprintf(buf, "addi a5, a5, %d", amount);
	emit(buf, f, true);
	emit("sw a5, (t1)", f, true);
}
void riscv_esp32_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("lw a5, (t1)", f, true);
	sprintf(buf, "addi a5, a5, -%d", amount);
	emit(buf, f, true);
	emit("sw a5, (t1)", f, true);
}

void riscv_esp32_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, false);

	emit("lw a5, (t1)", f, true);
	sprintf(buf, "beqz a5, _%d_loop_exit", id);
	emit(buf, f, true);
}
void riscv_esp32_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "j _%d_loop", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, false);
}

void riscv_esp32_gas_emit_stdin_read(FILE* f) {
	emit("call getchar", f, true);
	emit("sw a0, (t1)", f, true);
}
void riscv_esp32_gas_emit_stdout_write(FILE* f) {
	emit("lw a0, (t1)", f, true);
	emit("call putchar", f, true);
}

generator_t riscv_esp32_gas = {
	.name = "riscv-esp32-gas",
	.emit_initial_setup = riscv_esp32_gas_emit_initial_setup,
	.emit_clean_exit = riscv_esp32_gas_emit_clean_exit,
	.emit_ptr_plus = riscv_esp32_gas_emit_ptr_plus,
	.emit_ptr_minus = riscv_esp32_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = riscv_esp32_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = riscv_esp32_gas_emit_ptr_deref_minus,
	.emit_loop_begin = riscv_esp32_gas_emit_loop_begin,
	.emit_loop_end = riscv_esp32_gas_emit_loop_end,
	.emit_stdin_read = riscv_esp32_gas_emit_stdin_read,
	.emit_stdout_write = riscv_esp32_gas_emit_stdout_write
};