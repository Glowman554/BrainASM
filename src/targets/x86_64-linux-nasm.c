#include "generator.h"
#include <targets.h>

void x86_64_linux_nasm_emit_initial_setup(FILE* f) {
	emit("[section .data]", f, false);
	emit("tape: times 30000 db 0", f, false);
	emit("[section .text]", f, false);
	emit("[global main]", f, false);
	emit("[extern putchar]", f, false);
	emit("[extern getchar]", f, false);
	emit("main:", f, false);
	emit("mov rbx, tape", f, true);
}
void x86_64_linux_nasm_emit_clean_exit(FILE* f) {
	emit("mov rbx, 0", f, true);
	emit("ret", f, true);
}

void x86_64_linux_nasm_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add rbx, %d", amount);
	emit(buf, f, true);
}
void x86_64_linux_nasm_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub rbx, %d", amount);
	emit(buf, f, true);
}

void x86_64_linux_nasm_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add byte [rbx], %d", amount);
	emit(buf, f, true);
}
void x86_64_linux_nasm_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub byte [rbx], %d", amount);
	emit(buf, f, true);
}

void x86_64_linux_nasm_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, false);

	emit("cmp [rbx], byte 0", f, true);

	sprintf(buf, "je _%d_loop_exit", id);
	emit(buf, f, true);
}
void x86_64_linux_nasm_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmp _%d_loop", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, false);
}

void x86_64_linux_nasm_emit_stdin_read(FILE* f) {
	emit("call getchar", f, true);
	emit("mov [rbx], al", f, true);
}
void x86_64_linux_nasm_emit_stdout_write(FILE* f) {
	emit("mov dil, [rbx]", f, true);
	emit("call putchar", f, true);
}

generator_t x86_64_linux_nasm = {
	.name = "x86_64-linux-nasm",
	.emit_initial_setup = x86_64_linux_nasm_emit_initial_setup,
	.emit_clean_exit = x86_64_linux_nasm_emit_clean_exit,
	.emit_ptr_plus = x86_64_linux_nasm_emit_ptr_plus,
	.emit_ptr_minus = x86_64_linux_nasm_emit_ptr_minus,
	.emit_ptr_deref_plus = x86_64_linux_nasm_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = x86_64_linux_nasm_emit_ptr_deref_minus,
	.emit_loop_begin = x86_64_linux_nasm_emit_loop_begin,
	.emit_loop_end = x86_64_linux_nasm_emit_loop_end,
	.emit_stdin_read = x86_64_linux_nasm_emit_stdin_read,
	.emit_stdout_write = x86_64_linux_nasm_emit_stdout_write
};