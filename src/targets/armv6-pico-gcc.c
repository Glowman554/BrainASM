#include "generator.h"
#include <targets.h>

void armv6_pico_gcc_emit_initial_setup(FILE* f) {
	emit("#include <stdio.h>", f, false);
	emit("#include \"pico/stdlib.h\"", f, false);
	emit("int main() {", f, false);
	emit("stdio_init_all();", f, true);
	emit("while (true) {", f, true);
	emit("char tape[30000] = { 0 };", f, true);
	emit("char* ptr = tape;", f, true);
}
void armv6_pico_gcc_emit_clean_exit(FILE* f) {
	emit("}", f, true);
	emit("}", f, false);
}

void armv6_pico_gcc_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "ptr += %d;", amount);
	emit(buf, f, true);
}
void armv6_pico_gcc_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "ptr -= %d;", amount);
	emit(buf, f, true);
}

void armv6_pico_gcc_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "(*ptr) += %d;", amount);
	emit(buf, f, true);
}
void armv6_pico_gcc_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "(*ptr) -= %d;", amount);
	emit(buf, f, true);
}

void armv6_pico_gcc_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "while (*ptr) { // %d begin", id);
	emit(buf, f, true);
}
void armv6_pico_gcc_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "} // %d end", id);
	emit(buf, f, true);
}

void armv6_pico_gcc_emit_stdin_read(FILE* f) {
	emit("*ptr = getchar();", f, true);
}
void armv6_pico_gcc_emit_stdout_write(FILE* f) {
	emit("putchar(*ptr);", f, true);
}

generator_t armv6_pico_gcc = {
	.name = "armv6-pico-gcc",
	.emit_initial_setup = armv6_pico_gcc_emit_initial_setup,
	.emit_clean_exit = armv6_pico_gcc_emit_clean_exit,
	.emit_ptr_plus = armv6_pico_gcc_emit_ptr_plus,
	.emit_ptr_minus = armv6_pico_gcc_emit_ptr_minus,
	.emit_ptr_deref_plus = armv6_pico_gcc_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = armv6_pico_gcc_emit_ptr_deref_minus,
	.emit_loop_begin = armv6_pico_gcc_emit_loop_begin,
	.emit_loop_end = armv6_pico_gcc_emit_loop_end,
	.emit_stdin_read = armv6_pico_gcc_emit_stdin_read,
	.emit_stdout_write = armv6_pico_gcc_emit_stdout_write
};