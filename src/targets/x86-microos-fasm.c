#include "generator.h"
#include <targets.h>

void x86_microos_fasm_emit_initial_setup(FILE* f) {
	emit("format ELF executable at 0xA0000000", f, 0);
	emit("entry main", f, 0);

	emit("main:", f, 0);

	emit("mov eax, 0xe", f, 1);
	emit("mov ebx, 0xd0000000", f, 1);
	emit("int 0x30", f, 1);

	emit("mov ecx, 0x1000", f, 1);
	emit("clear_loop:", f, 0);
	emit("mov byte [ecx + 0xd0000000 - 1], 0", f, 1);
	emit("loop clear_loop", f, 1);

	emit("mov ebx, 0xd0000000", f, 1);
}
void x86_microos_fasm_emit_clean_exit(FILE* f) {
	emit("mov eax, 0xd", f, 1);
	emit("mov ebx, 0", f, 1);
	emit("int 0x30", f, 1);
}

void x86_microos_fasm_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add ebx, %d", amount);
	emit(buf, f, 1);
}
void x86_microos_fasm_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub ebx, %d", amount);
	emit(buf, f, 1);
}

void x86_microos_fasm_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add byte [ebx], %d", amount);
	emit(buf, f, 1);
}
void x86_microos_fasm_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub byte [ebx], %d", amount);
	emit(buf, f, 1);
}

void x86_microos_fasm_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("cmp [ebx], byte 0", f, 1);

	sprintf(buf, "je _%d_loop_exit", id);
	emit(buf, f, 1);
}
void x86_microos_fasm_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmp _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void x86_microos_fasm_emit_stdin_read(FILE* f) {
	emit("push ebx", f, 1);

	emit("mov ecx, ebx", f, 1);
	emit("mov eax, 0x3", f, 1);
	emit("mov ebx, 0", f, 1);
	emit("mov edx, 1", f, 1);
	emit("mov esi, 0", f, 1);
	emit("int 0x30", f, 1);

	emit("pop ebx", f, 1);
}
void x86_microos_fasm_emit_stdout_write(FILE* f) {
	emit("push ebx", f, 1);

	emit("mov ecx, ebx", f, 1);
	emit("mov eax, 0x4", f, 1);
	emit("mov ebx, 1", f, 1);
	emit("mov edx, 1", f, 1);
	emit("mov esi, 0", f, 1);
	emit("int 0x30", f, 1);

	emit("pop ebx", f, 1);
}

generator_t x86_microos_fasm = {
	.name = "x86-microos-fasm",
	.emit_initial_setup = x86_microos_fasm_emit_initial_setup,
	.emit_clean_exit = x86_microos_fasm_emit_clean_exit,
	.emit_ptr_plus = x86_microos_fasm_emit_ptr_plus,
	.emit_ptr_minus = x86_microos_fasm_emit_ptr_minus,
	.emit_ptr_deref_plus = x86_microos_fasm_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = x86_microos_fasm_emit_ptr_deref_minus,
	.emit_loop_begin = x86_microos_fasm_emit_loop_begin,
	.emit_loop_end = x86_microos_fasm_emit_loop_end,
	.emit_stdin_read = x86_microos_fasm_emit_stdin_read,
	.emit_stdout_write = x86_microos_fasm_emit_stdout_write
};