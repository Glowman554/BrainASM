#include <generator.h>

void emit(char* instr, FILE* f, int indent) {
	for (int i = 0; i < indent; i++) {
		fputc('\t', f);
	}
	fputs(instr, f);
	fputc('\n', f);
}