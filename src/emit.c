#include <generator.h>

void emit(char* instr, FILE* f, bool indent) {
	if (indent) {
		fputc('\t', f);
	}
	fputs(instr, f);
	fputc('\n', f);
}