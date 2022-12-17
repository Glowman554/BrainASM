#include "generator.h"
#include <targets.h>

void any_any_gcc_emit_initial_setup(FILE* f) {
	emit("#include <stdio.h>", f, false);
	emit("int main() {", f, false);
	emit("char tape[30000] = { 0 };", f, true);
	emit("char* ptr = tape;", f, true);
}
void any_any_gcc_emit_clean_exit(FILE* f) {
	emit("return 0;", f, true);
	emit("}", f, false);
}

void any_any_gcc_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "ptr += %d;", amount);
	emit(buf, f, true);
}
void any_any_gcc_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "ptr -= %d;", amount);
	emit(buf, f, true);
}

void any_any_gcc_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "(*ptr) += %d;", amount);
	emit(buf, f, true);
}
void any_any_gcc_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "(*ptr) -= %d;", amount);
	emit(buf, f, true);
}

void any_any_gcc_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "while (*ptr) { // %d begin", id);
	emit(buf, f, true);
}
void any_any_gcc_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "} // %d end", id);
	emit(buf, f, true);
}

void any_any_gcc_emit_stdin_read(FILE* f) {
	emit("*ptr = getchar();", f, true);
}
void any_any_gcc_emit_stdout_write(FILE* f) {
	emit("putchar(*ptr);", f, true);
}

generator_t any_any_gcc = {
	.name = "any-any-gcc",
	.emit_initial_setup = any_any_gcc_emit_initial_setup,
	.emit_clean_exit = any_any_gcc_emit_clean_exit,
	.emit_ptr_plus = any_any_gcc_emit_ptr_plus,
	.emit_ptr_minus = any_any_gcc_emit_ptr_minus,
	.emit_ptr_deref_plus = any_any_gcc_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = any_any_gcc_emit_ptr_deref_minus,
	.emit_loop_begin = any_any_gcc_emit_loop_begin,
	.emit_loop_end = any_any_gcc_emit_loop_end,
	.emit_stdin_read = any_any_gcc_emit_stdin_read,
	.emit_stdout_write = any_any_gcc_emit_stdout_write
};