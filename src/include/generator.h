#pragma once

#include <stdio.h>
#include <stdbool.h>

void emit(char* instr, FILE* f, int indent);

typedef struct generator {
	char name[0xff];

	void (*emit_initial_setup)(FILE* f);
	void (*emit_clean_exit)(FILE* f);


	void (*emit_ptr_plus)(int amount, FILE* f);
	void (*emit_ptr_minus)(int amount, FILE* f);

	void (*emit_ptr_deref_plus)(int amount, FILE* f);
	void (*emit_ptr_deref_minus)(int amount, FILE* f);

	void (*emit_loop_begin)(int id, FILE* f);
	void (*emit_loop_end)(int id, FILE* f);

	void (*emit_stdin_read)(FILE* f);
	void (*emit_stdout_write)(FILE* f);
} generator_t;