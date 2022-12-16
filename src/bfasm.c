#include <targets.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int pop_id(int** ids, int* num_ids) {
	int id = (*ids)[*num_ids - 1];

	*ids = realloc(*ids, sizeof(int) * (*num_ids - 1));
	(*num_ids)--;

	return id;
}

void push_id(int id, int** ids, int* num_ids) {
	*ids = realloc(*ids, sizeof(int) * (*num_ids + 1));
	(*ids)[*num_ids] = id;

	(*num_ids)++;
}

int count_sym(char s, FILE* f) {
	int count = 0;

	char c = 0;
	while ((c = getc(f)) != EOF) {
		if (c == s) {
			count++;
		} else {
			if (c == ' ' || c == '\n') {
				continue;
			}

			fseek(f, ftell(f) - 1, SEEK_SET);
			return count;
		}
	}

	return count;
}

generator_t* generators[] = {
	&x86_64_linux_gas,
	&x86_64_linux_nasm
};

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Usage: %s <target> <in> <out>\n", argv[0]);
		return -1;
	}

	FILE* in = fopen(argv[2], "r");
	FILE* out = fopen(argv[3], "w");

	generator_t* generator = NULL;
	for (int i = 0; i < sizeof(generators) / sizeof(generators[0]); i++) {
		if (strcmp(generators[i]->name, argv[1]) == 0) {
			generator = generators[i];
			break;
		}
	}
	if (generator == NULL) {
		printf("Target %s not found!\n", argv[1]);
		return -1;
	}

	generator->emit_initial_setup(out);

	int* ids = NULL;
	int num_ids = 0;
	int curr_id = 0;

	char c = 0;
	while ((c = getc(in)) != EOF) {
		char buf[0xff] = { 0 };

		switch (c) {
			case '<':
				generator->emit_ptr_minus(count_sym('<', in) + 1, out);
				break;
			case '>':
				generator->emit_ptr_plus(count_sym('>', in) + 1, out);
				break;
			case '-':
				generator->emit_ptr_deref_minus(count_sym('-', in) + 1, out);
				break;
			case '+':
				generator->emit_ptr_deref_plus(count_sym('+', in) + 1, out);
				break;
			case '[':
				generator->emit_loop_begin(curr_id, out);
				push_id(curr_id++, &ids, &num_ids);
				break;
			case ']':
				{
					int id = pop_id(&ids, &num_ids);
					generator->emit_loop_end(id, out);
				}
				break;
			case '.':
				generator->emit_stdout_write(out);
				break;
			case ',':
				generator->emit_stdin_read(out);
				break;
		}
	}

	generator->emit_clean_exit(out);

	return 0;
}