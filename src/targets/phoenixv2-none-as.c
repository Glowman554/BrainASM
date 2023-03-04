#include "generator.h"
#include <targets.h>

void phoenixv2_none_as_emit_initial_setup(FILE* f) {
	emit("entry:", f, 0);
	emit("lad A, 0x8004", f, 1);
	emit("lad B, 0x0", f, 1);
}
void phoenixv2_none_as_emit_clean_exit(FILE* f) {
	emit("jmp 0xffff", f, 1);
}

void phoenixv2_none_as_emit_ptr_plus(int amount, FILE* f) {
	emit("cfg", f, 1);

	char buf[0xff] = { 0 };
	sprintf(buf, "add r0, lo(%d)", amount);
	emit(buf, f, 1);

	emit("ado r1, 1", f, 1);

	if (amount > 0xff) {
		sprintf(buf, "add r1, hi(%d)", amount);
		emit(buf, f, 1);
	}
}

void phoenixv2_none_as_emit_ptr_minus(int amount, FILE* f) {
	emit("cfg", f, 1);

	char buf[0xff] = { 0 };
	sprintf(buf, "sub r0, lo(%d)", amount);
	emit(buf, f, 1);

	emit("sbo r1, 1", f, 1);
	
	if (amount > 0xff) {
		sprintf(buf, "sub r1, hi(%d)", amount);
		emit(buf, f, 1);
	}
}

void phoenixv2_none_as_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldr r5, A", f, 1);
	sprintf(buf, "add r5, %d", amount);
	emit(buf, f, 1);
	emit("wtr A, r5", f, 1);

}
void phoenixv2_none_as_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldr r5, A", f, 1);
	sprintf(buf, "sub r5, %d", amount);
	emit(buf, f, 1);
	emit("wtr A, r5", f, 1);
}

void phoenixv2_none_as_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "l%d:", id);
	emit(buf, f, 0);

	emit("ldr r5, A", f, 1);
	emit("cmp r5, 0", f, 1);

	sprintf(buf, "jeq l%d_exit", id);
	emit(buf, f, 1);
}
void phoenixv2_none_as_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmp l%d", id);
	emit(buf, f, 1);

	sprintf(buf, "l%d_exit:", id);
	emit(buf, f, 0);
}

void phoenixv2_none_as_emit_stdin_read(FILE* f) {
	emit("inp r5, B", f, 1);
	emit("wtr A, r5", f, 1);
}
void phoenixv2_none_as_emit_stdout_write(FILE* f) {
	emit("ldr r5, A", f, 1);
	emit("out B, r5", f, 1);
}

generator_t phoenixv2_none_as = {
	.name = "phoenixv2-none-as",
	.emit_initial_setup = phoenixv2_none_as_emit_initial_setup,
	.emit_clean_exit = phoenixv2_none_as_emit_clean_exit,
	.emit_ptr_plus = phoenixv2_none_as_emit_ptr_plus,
	.emit_ptr_minus = phoenixv2_none_as_emit_ptr_minus,
	.emit_ptr_deref_plus = phoenixv2_none_as_emit_ptr_deref_plus,
	.emit_ptr_deref_minus = phoenixv2_none_as_emit_ptr_deref_minus,
	.emit_loop_begin = phoenixv2_none_as_emit_loop_begin,
	.emit_loop_end = phoenixv2_none_as_emit_loop_end,
	.emit_stdin_read = phoenixv2_none_as_emit_stdin_read,
	.emit_stdout_write = phoenixv2_none_as_emit_stdout_write
};
