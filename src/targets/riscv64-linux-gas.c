#include "generator.h"
#include <targets.h>

void riscv64_linux_gas_emit_initial_setup(FILE* f) {
	emit(".section .bss", f, 0);
	emit("tape: .space 30000", f, 0);
	emit(".section .text", f, 0);
	emit(".global main", f, 0);
	emit(".extern putchar", f, 0);
	emit(".extern getchar", f, 0);
	emit("main:", f, 0);
	emit("mv t0, ra", f, 1);
	emit("la t1, tape", f, 1);
}
void riscv64_linux_gas_emit_clean_exit(FILE* f) {
	emit("mv ra, t0", f, 1);
	emit("li a0, 0", f, 1);
	emit("ret", f, 1);
}

void riscv64_linux_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi t1, t1, %d", amount * 4);
	emit(buf, f, 1);
}
void riscv64_linux_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addi t1, t1, -%d", amount * 4);
	emit(buf, f, 1);
}

void riscv64_linux_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("lw a5, (t1)", f, 1);
	sprintf(buf, "addi a5, a5, %d", amount);
	emit(buf, f, 1);
	emit("sw a5, (t1)", f, 1);
}
void riscv64_linux_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("lw a5, (t1)", f, 1);
	sprintf(buf, "addi a5, a5, -%d", amount);
	emit(buf, f, 1);
	emit("sw a5, (t1)", f, 1);
}

void riscv64_linux_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("lw a5, (t1)", f, 1);
	sprintf(buf, "beqz a5, _%d_loop_exit", id);
	emit(buf, f, 1);
}
void riscv64_linux_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "j _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void riscv64_linux_gas_emit_stdin_read(FILE* f) {
	emit("call getchar", f, 1);
	emit("sw a0, (t1)", f, 1);
}
void riscv64_linux_gas_emit_stdout_write(FILE* f) {
	emit("lw a0, (t1)", f, 1);
	emit("call putchar", f, 1);
}

generator_t riscv64_linux_gas = {
	.name = "riscv64-linux-gas",
	.emit_initial_setup = riscv64_linux_gas_emit_initial_setup,
	.emit_clean_exit = riscv64_linux_gas_emit_clean_exit,
	.emit_ptr_plus = riscv64_linux_gas_emit_ptr_plus,
	.emit_ptr_minus = riscv64_linux_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = riscv64_linux_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = riscv64_linux_gas_emit_ptr_deref_minus,
	.emit_loop_begin = riscv64_linux_gas_emit_loop_begin,
	.emit_loop_end = riscv64_linux_gas_emit_loop_end,
	.emit_stdin_read = riscv64_linux_gas_emit_stdin_read,
	.emit_stdout_write = riscv64_linux_gas_emit_stdout_write
};
