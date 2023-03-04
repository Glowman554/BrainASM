#include "generator.h"
#include <targets.h>

void armv6_pico_gas_emit_initial_setup(FILE* f) {
	emit(".cpu cortex-m0plus", f, 0);
	emit(".section .data", f, 0);
	emit("tape:", f, 0);
	emit(".space 30000", f, 1);
	emit(".code 16", f, 0);
	emit(".thumb_func", f, 0);

	emit(".section .text", f, 0);
	emit(".global main", f, 0);
	emit(".extern putchar", f, 0);
	emit(".extern getchar", f, 0);
	emit("main:", f, 0);
	emit("push {lr}", f, 1);
	emit("bl stdio_init_all", f, 1);
	emit("ldr r0, =30000", f, 1);
	emit("bl sleep_ms", f, 1);
	emit("ldr r7, =tape", f, 1);
}
void armv6_pico_gas_emit_clean_exit(FILE* f) {
	emit("mov r0, #0", f, 1);
	emit("pop {pc}", f, 1);
}

void armv6_pico_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add r7, #%d", amount);
	emit(buf, f, 1);
}
void armv6_pico_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub r7, #%d", amount);
	emit(buf, f, 1);
}

void armv6_pico_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb r0, [r7, #0]", f, 1);
	sprintf(buf, "add r0, #%d", amount);
	emit(buf, f, 1);
	emit("strb r0, [r7, #0]", f, 1);
}
void armv6_pico_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb r0, [r7, #0]", f, 1);
	sprintf(buf, "sub r0, #%d", amount);
	emit(buf, f, 1);
	emit("strb r0, [r7, #0]", f, 1);
}

void armv6_pico_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, 0);

	emit("ldrb r0, [r7, #0]", f, 1);
	emit("cmp r0, #0", f, 1);

	sprintf(buf, "bne _%d_loop_skip", id); // done like this since conditional jumps only have 8 bytes to encode the address and that sometimes isnt enough
	emit(buf, f, 1);

	sprintf(buf, "b _%d_loop_exit", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_skip:", id);
	emit(buf, f, 0);
}
void armv6_pico_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "b _%d_loop", id);
	emit(buf, f, 1);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, 0);
}

void armv6_pico_gas_emit_stdin_read(FILE* f) {
	emit("bl getchar", f, 1);
	emit("strb r0, [r7, #0]", f, 1);
}
void armv6_pico_gas_emit_stdout_write(FILE* f) {
	emit("ldrb r0, [r7, #0]", f, 1);
	emit("bl putchar", f, 1);
}

generator_t armv6_pico_gas = {
	.name = "armv6-pico-gas",
	.emit_initial_setup = armv6_pico_gas_emit_initial_setup,
	.emit_clean_exit = armv6_pico_gas_emit_clean_exit,
	.emit_ptr_plus = armv6_pico_gas_emit_ptr_plus,
	.emit_ptr_minus = armv6_pico_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = armv6_pico_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = armv6_pico_gas_emit_ptr_deref_minus,
	.emit_loop_begin = armv6_pico_gas_emit_loop_begin,
	.emit_loop_end = armv6_pico_gas_emit_loop_end,
	.emit_stdin_read = armv6_pico_gas_emit_stdin_read,
	.emit_stdout_write = armv6_pico_gas_emit_stdout_write
};
