#include "generator.h"
#include <targets.h>

void armv7m_stm32_gas_emit_initial_setup(FILE* f) {
	emit(".cpu cortex-m3", f, false);
	emit(".section .data", f, false);
	emit("tape:", f, false);
	emit(".space 30000", f, true);
	emit(".code 16", f, false);
	emit(".thumb_func", f, false);

	emit(".section .text", f, false);
	emit(".global main", f, false);
	emit(".extern putchar", f, false);
	emit(".extern getchar", f, false);
	emit("main:", f, false);
	emit("push {lr}", f, true);
	emit("ldr r0, =30000", f, true);
	emit("bl thread_sleep_for", f, true);
	emit("ldr r7, =tape", f, true);
}
void armv7m_stm32_gas_emit_clean_exit(FILE* f) {
	emit("mov r0, #0", f, true);
	emit("pop {pc}", f, true);
}

void armv7m_stm32_gas_emit_ptr_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "add r7, #%d", amount);
	emit(buf, f, true);
}
void armv7m_stm32_gas_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "sub r7, #%d", amount);
	emit(buf, f, true);
}

void armv7m_stm32_gas_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb r0, [r7, #0]", f, true);
	sprintf(buf, "add r0, #%d", amount);
	emit(buf, f, true);
	emit("strb r0, [r7, #0]", f, true);
}
void armv7m_stm32_gas_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldrb r0, [r7, #0]", f, true);
	sprintf(buf, "sub r0, #%d", amount);
	emit(buf, f, true);
	emit("strb r0, [r7, #0]", f, true);
}

void armv7m_stm32_gas_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, false);

	emit("ldrb r0, [r7, #0]", f, true);
	emit("cmp r0, #0", f, true);

	sprintf(buf, "bne _%d_loop_skip", id); // done like this since conditional jumps only have 8 bytes to encode the address and that sometimes isnt enough
	emit(buf, f, true);

	sprintf(buf, "b _%d_loop_exit", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_skip:", id);
	emit(buf, f, false);
}
void armv7m_stm32_gas_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "b _%d_loop", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, false);
}

void armv7m_stm32_gas_emit_stdin_read(FILE* f) {
	emit("bl getchar", f, true);
	emit("strb r0, [r7, #0]", f, true);
}
void armv7m_stm32_gas_emit_stdout_write(FILE* f) {
	emit("ldrb r0, [r7, #0]", f, true);
	emit("bl putchar", f, true);
}

generator_t armv7m_stm32_gas = {
	.name = "armv7m-stm32-gas",
	.emit_initial_setup = armv7m_stm32_gas_emit_initial_setup,
	.emit_clean_exit = armv7m_stm32_gas_emit_clean_exit,
	.emit_ptr_plus = armv7m_stm32_gas_emit_ptr_plus,
	.emit_ptr_minus = armv7m_stm32_gas_emit_ptr_minus,
	.emit_ptr_deref_plus = armv7m_stm32_gas_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = armv7m_stm32_gas_emit_ptr_deref_minus,
	.emit_loop_begin = armv7m_stm32_gas_emit_loop_begin,
	.emit_loop_end = armv7m_stm32_gas_emit_loop_end,
	.emit_stdin_read = armv7m_stm32_gas_emit_stdin_read,
	.emit_stdout_write = armv7m_stm32_gas_emit_stdout_write
};
