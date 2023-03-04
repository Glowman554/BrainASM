#include "generator.h"
#include <targets.h>

void aarch64_linux_gas_emit_initial_setup(FILE* f) {
	emit(".section .data", f, 0);
	emit("tape: .space 30000", f, 0);
	emit(".section .text", f, 0);
	emit(".global main", f, 0);
	emit(".extern putchar", f, 0);
	emit(".extern getchar", f, 0);
	emit("main:", f, 0);
	emit("mov x21, x30", f, 1);
	emit("ldr x19, =tape", f, 1);
}
void aarch64_linux_gas_emit_clean_exit(FILE* f) {
	emit("mov w0, #0", f, 1);
	emit("mov x30, x21", f, 1);
	emit("ret", f, 1);
}

void aarch64_linux_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add x19, x19, %d", amount);
	emit(buf, f, 1);
}
void aarch64_linux_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub x19, x19, %d", amount);
	emit(buf, f, 1);
}

void aarch64_linux_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb w20, [x19, #0]", f, 1);
	sprintf(buf, "add w20, w20, %d", amount);
	emit(buf, f, 1);
	emit("strb w20, [x19, #0]", f, 1);
}
void aarch64_linux_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb w20, [x19, #0]", f, 1);
	sprintf(buf, "sub w20, w20, %d", amount);
	emit(buf, f, 1);
	emit("strb w20, [x19, #0]", f, 1);
}

void aarch64_linux_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("ldrb w20, [x19, #0]", f, 1);
	emit("cmp w20, #0", f, 1);

	sprintf(buf, "b.eq _%d_loop_exit", id);
	emit(buf, f, 1);
}
void aarch64_linux_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "b _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void aarch64_linux_gas_emit_stdin_read(FILE* f) {
	emit("bl getchar", f, 1);
	emit("strb w0, [x19, #0]", f, 1);
}
void aarch64_linux_gas_emit_stdout_write(FILE* f) {
	emit("ldrb w0, [x19, #0]", f, 1);
	emit("bl putchar", f, 1);
}

generator_t aarch64_linux_gas = {
	.name = "aarch64-linux-gas",
	.emit_initial_setup = aarch64_linux_gas_emit_initial_setup,
	.emit_clean_exit = aarch64_linux_gas_emit_clean_exit,
	.emit_ptr_plus = aarch64_linux_gas_emit_ptr_plus,
	.emit_ptr_minus = aarch64_linux_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = aarch64_linux_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = aarch64_linux_gas_emit_ptr_deref_minus,
	.emit_loop_begin = aarch64_linux_gas_emit_loop_begin,
	.emit_loop_end = aarch64_linux_gas_emit_loop_end,
	.emit_stdin_read = aarch64_linux_gas_emit_stdin_read,
	.emit_stdout_write = aarch64_linux_gas_emit_stdout_write
};
