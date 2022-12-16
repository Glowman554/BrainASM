#include "generator.h"
#include <targets.h>

void aarch64_linux_gas_emit_initial_setup(FILE* f) {
	emit(".section .data", f, false);
	emit("tape: .space 30000", f, false);
	emit(".section .text", f, false);
	emit(".global main", f, false);
	emit(".extern putchar", f, false);
	emit(".extern getchar", f, false);
	emit("main:", f, false);
	emit("mov x21, x30", f, true);
	emit("ldr x19, =tape", f, true);
}
void aarch64_linux_gas_emit_clean_exit(FILE* f) {
	emit("mov w0, #0", f, true);
	emit("mov x30, x21", f, true);
	emit("ret", f, true);
}

void aarch64_linux_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add x19, x19, %d", amount);
	emit(buf, f, true);
}
void aarch64_linux_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub x19, x19, %d", amount);
	emit(buf, f, true);
}

void aarch64_linux_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb w20, [x19, #0]", f, true);
	sprintf(buf, "add w20, w20, %d", amount);
	emit(buf, f, true);
	emit("strb w20, [x19, #0]", f, true);
}
void aarch64_linux_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb w20, [x19, #0]", f, true);
	sprintf(buf, "sub w20, w20, %d", amount);
	emit(buf, f, true);
	emit("strb w20, [x19, #0]", f, true);
}

void aarch64_linux_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, false);

	emit("ldrb w20, [x19, #0]", f, true);
	emit("cmp w20, #0", f, true);

	sprintf(buf, "b.eq _%d_loop_exit", id);
	emit(buf, f, true);
}
void aarch64_linux_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "b _%d_loop", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, false);
}

void aarch64_linux_gas_emit_stdin_read(FILE* f) {
	emit("bl getchar", f, true);
	emit("strb w0, [x19, #0]", f, true);
}
void aarch64_linux_gas_emit_stdout_write(FILE* f) {
	emit("ldrb w0, [x19, #0]", f, true);
	emit("bl putchar", f, true);
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
