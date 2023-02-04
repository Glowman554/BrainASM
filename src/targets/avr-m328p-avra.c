#include "generator.h"
#include <targets.h>

void avr_m328p_avra_emit_initial_setup(FILE* f) {
	emit(".device ATmega328P", f, false);

	emit(".org 0x0", f, false);
	emit("jmp _entry", f, true);

	for (int i = 0x2; i <= 0x32; i += 0x2) {
		char buf[0xff] = { 0 };
		sprintf(buf, ".org 0x%x", i);
		emit(buf, f, false);

		emit("reti", f, true);
	}

	emit(".include \"m328p.asm\"", f, false);

	emit("_entry:", f, false);

	// stack setup (put stack at end of ram)
	emit("ldi r16, low(0x8ff)", f, true);
	emit("out 0x3d, r16", f, true);

	emit("ldi r16, high(0x8ff)", f, true);
	emit("out 0x3e, r16", f, true);

	// clear sram (0x0100 - 0x08ff)
	emit("ldi r30, low(0x0100)", f, true);
	emit("ldi r31, high(0x0100)", f, true);
	emit("ldi r16, 0", f, true);
	emit("ldi r17, 1", f, true);
	emit("clear_loop:", f, false);
	emit("st Z, r16", f, true);
	emit("add r30, r17", f, true);
	emit("adc r31, r16", f, true);
	emit("cpi r30, 0xff", f, true);
	emit("brne clear_loop", f, true);
	emit("cpi r31, 0x08", f, true);
	emit("brne clear_loop", f, true);


	emit("call USART0_init", f, true);

	// use z index register for tape
	emit("ldi r30, low(0x100)", f, true);
	emit("ldi r31, high(0x100)", f, true);

}
void avr_m328p_avra_emit_clean_exit(FILE* f) {
	emit("exit_loop:", f, false);
	emit("jmp exit_loop", f, true);
}

void avr_m328p_avra_emit_ptr_plus(int amount, FILE* f) {
	// why does subi / sbci exsist but not addi / adci?
	char buf[0xff] = { 0 };
	sprintf(buf, "ldi r16, low(%d)", amount);
	emit(buf, f, true);
	emit("add r30, r16", f, true);

	sprintf(buf, "ldi r16, high(%d)", amount);
	emit(buf, f, true);
	emit("adc r31, r16", f, true);
}
void avr_m328p_avra_emit_ptr_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "subi r30, low(%d)", amount);
	emit(buf, f, true);

	sprintf(buf, "sbci r31, high(%d)", amount);
	emit(buf, f, true);
}

void avr_m328p_avra_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ld r16, Z", f, true);
	sprintf(buf, "ldi r17, %d", amount);
	emit(buf, f, true);
	emit("add r16, r17", f, true);
	emit("st Z, r16", f, true);
}
void avr_m328p_avra_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ld r16, Z", f, true);
	sprintf(buf, "subi r16, %d", amount);
	emit(buf, f, true);
	emit("st Z, r16", f, true);
}

void avr_m328p_avra_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "_%d_loop:", id);
	emit(buf, f, false);

	emit("ld r16, Z", f, true);
	emit("cpi r16, 0", f, true);

	sprintf(buf, "brne _%d_loop_skip", id); // done like this since conditional jumps only have a range of +/-64
	emit(buf, f, true);

	sprintf(buf, "jmp _%d_loop_exit", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_skip:", id);
	emit(buf, f, false);
}

void avr_m328p_avra_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmp _%d_loop", id);
	emit(buf, f, true);

	sprintf(buf, "_%d_loop_exit:", id);
	emit(buf, f, false);
}

void avr_m328p_avra_emit_stdin_read(FILE* f) {
	emit("call USART0_receive", f, true);
	emit("st Z, r22", f, true);
}
void avr_m328p_avra_emit_stdout_write(FILE* f) {
	emit("ld r22, Z", f, true);
	emit("call USART0_transmit", f, true);
}

generator_t avr_m328p_avra = {
	.name = "avr-m328p-avra",
	.emit_initial_setup = avr_m328p_avra_emit_initial_setup,
	.emit_clean_exit = avr_m328p_avra_emit_clean_exit,
	.emit_ptr_plus = avr_m328p_avra_emit_ptr_plus,
	.emit_ptr_minus = avr_m328p_avra_emit_ptr_minus,
	.emit_ptr_deref_plus = avr_m328p_avra_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = avr_m328p_avra_emit_ptr_deref_minus,
	.emit_loop_begin = avr_m328p_avra_emit_loop_begin,
	.emit_loop_end = avr_m328p_avra_emit_loop_end,
	.emit_stdin_read = avr_m328p_avra_emit_stdin_read,
	.emit_stdout_write = avr_m328p_avra_emit_stdout_write
};
