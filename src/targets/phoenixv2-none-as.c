#include "generator.h"
#include <targets.h>

void phoenixv2_none_as_emit_initial_setup(FILE* f) {
	emit("entry:", f, false);
	emit("lad A, 0x8004", f, true);
	emit("lad B, 0x0", f, true);
}
void phoenixv2_none_as_emit_clean_exit(FILE* f) {
	emit("jmpi 0xffff", f, true);
}

void phoenixv2_none_as_emit_ptr_plus(int amount, FILE* f) {
	static int add_ov_skip_id = 0;

	emit("cmpi r15, 0", f, true); // clear possible ov

	char buf[0xff] = { 0 };
	sprintf(buf, "addi r0, %d", amount);
	emit(buf, f, true);

	sprintf(buf, "jnoi addr(aos%d)", add_ov_skip_id);
	emit(buf, f, true);

	emit("addi r1, 1", f, true);

	sprintf(buf, "aos%d:", add_ov_skip_id);
	emit(buf, f, false);

	add_ov_skip_id++;
}
void phoenixv2_none_as_emit_ptr_minus(int amount, FILE* f) {
	static int sub_ov_skip_id = 0;

	emit("cmpi r15, 0", f, true); // clear possible ov

	char buf[0xff] = { 0 };
	sprintf(buf, "subi r0, %d", amount);
	emit(buf, f, true);

	sprintf(buf, "jnoi addr(sos%d)", sub_ov_skip_id);
	emit(buf, f, true);

	emit("subi r1, 1", f, true);

	sprintf(buf, "sos%d:", sub_ov_skip_id);
	emit(buf, f, false);

	sub_ov_skip_id++;
}

void phoenixv2_none_as_emit_ptr_deref_plus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldr r5, A", f, true);
	sprintf(buf, "addi r5, %d", amount);
	emit(buf, f, true);
	emit("wtr A, r5", f, true);

}
void phoenixv2_none_as_emit_ptr_deref_minus(int amount, FILE* f) {
	char buf[0xff] = { 0 };
	emit("ldr r5, A", f, true);
	sprintf(buf, "subi r5, %d", amount);
	emit(buf, f, true);
	emit("wtr A, r5", f, true);
}

void phoenixv2_none_as_emit_loop_begin(int id, FILE* f) {
	char buf[0xff] = { 0 };
	sprintf(buf, "l%d:", id);
	emit(buf, f, false);

	emit("ldr r5, A", f, true);
	emit("cmpi r5, 0", f, true);

	sprintf(buf, "jeqi addr(l%d_exit)", id);
	emit(buf, f, true);
}
void phoenixv2_none_as_emit_loop_end(int id, FILE* f) {
	char buf[0xff] = { 0 };

	sprintf(buf, "jmpi addr(l%d)", id);
	emit(buf, f, true);

	sprintf(buf, "l%d_exit:", id);
	emit(buf, f, false);
}

void phoenixv2_none_as_emit_stdin_read(FILE* f) {
	emit("inp r5, B", f, true);
	emit("wtr A, r5", f, true);
}
void phoenixv2_none_as_emit_stdout_write(FILE* f) {
	emit("ldr r5, A", f, true);
	emit("out B, r5", f, true);
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
