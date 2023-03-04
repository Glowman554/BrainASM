#include "generator.h"
#include <targets.h>

void x86_64_linux_gas_emit_initial_setup(FILE* f) {
	emit(".section .data", f, 0);
	emit("tape: .space 30000", f, 0);
	emit(".section .text", f, 0);
	emit(".global main", f, 0);
	emit(".extern putchar", f, 0);
	emit(".extern getchar", f, 0);
	emit("main:", f, 0);
	emit("movq $tape, %rbx", f, 1);
}
void x86_64_linux_gas_emit_clean_exit(FILE* f) {
	emit("movq $0, %rax", f, 1);
	emit("ret", f, 1);
}

void x86_64_linux_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addq $%d, %%rbx", amount);
	emit(buf, f, 1);
}
void x86_64_linux_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "subq $%d, %%rbx", amount);
	emit(buf, f, 1);
}

void x86_64_linux_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "addb $%d, (%%rbx)", amount);
	emit(buf, f, 1);
}
void x86_64_linux_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "subb $%d, (%%rbx)", amount);
	emit(buf, f, 1);
}

void x86_64_linux_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("cmpb $0, (%rbx)", f, 1);

	sprintf(buf, "je _%d_loop_exit", id);
	emit(buf, f, 1);
}
void x86_64_linux_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmp _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void x86_64_linux_gas_emit_stdin_read(FILE* f) {
	emit("call getchar", f, 1);
	emit("movb %al, (%rax)", f, 1);
}
void x86_64_linux_gas_emit_stdout_write(FILE* f) {
	emit("movb (%rbx), %dil", f, 1);
	emit("call putchar", f, 1);
}

generator_t x86_64_linux_gas = {
	.name = "x86_64-linux-gas",
	.emit_initial_setup = x86_64_linux_gas_emit_initial_setup,
	.emit_clean_exit = x86_64_linux_gas_emit_clean_exit,
	.emit_ptr_plus = x86_64_linux_gas_emit_ptr_plus,
	.emit_ptr_minus = x86_64_linux_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = x86_64_linux_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = x86_64_linux_gas_emit_ptr_deref_minus,
	.emit_loop_begin = x86_64_linux_gas_emit_loop_begin,
	.emit_loop_end = x86_64_linux_gas_emit_loop_end,
	.emit_stdin_read = x86_64_linux_gas_emit_stdin_read,
	.emit_stdout_write = x86_64_linux_gas_emit_stdout_write
};