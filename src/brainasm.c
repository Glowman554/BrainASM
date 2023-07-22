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

	int c = 0;
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
	&x86_microos_fasm,
	&x86_64_linux_gas,
	&x86_64_linux_nasm,
	&aarch64_linux_gas,
	&riscv64_linux_gas,
	&phoenix_none_as,
	&phoenixv2_none_as,
	&any_any_gcc,
	&any_any_firestorm,
	&armv6_pico_gcc,
	&armv6_pico_gas,
	&armv7m_stm32_gas,
	&avr_m328p_avra,
	&avr_t85_avra,
	&riscv_esp32_gas,
	&xtensa_esp32_gas
};

int main(int argc, char* argv[]) {
	char* out_str = "a.out";
	char* in_str = NULL;
	char* target_str = "any-any-gcc";

	int idx = 1;
	while (idx < argc) {
		if (strcmp(argv[idx], "-o") == 0) {
			if (idx + 1 < argc) {
				idx++;
				out_str = argv[idx];
			} else {
				printf("Error: -o requires an argument\n");
				return -1;
			}
		} else if (strcmp(argv[idx], "-t") == 0) {
			if (idx + 1 < argc) {
				idx++;
				target_str = argv[idx];
			} else {
				printf("Error: -t requires an argument\n");
				return -1;
			}
		} else if (strcmp(argv[idx], "-h") == 0) {
			printf("Usage: %s [-o <out>] [-t <target>] <in>\n", argv[0]);
			printf("Supported targets:\n");
			for (int i = 0; i < sizeof(generators) / sizeof(generators[0]); i++) {
				printf("\t%s\n", generators[i]->name);
			}
			return 0;
		} else {
			if (in_str == NULL) {
				in_str = argv[idx];
			} else {
				printf("Error: Too many arguments\n");
				return -1;
			}
		}

		idx++;
	}

	if (in_str == NULL) {
		printf("Please specfy a input file!\n");
		return -1;
	}

	FILE* in = fopen(in_str, "r");
	FILE* out = fopen(out_str, "w");

	if (in == NULL || out == NULL) {
		printf("Could not open input or output file!\n");
		return -1;
	}

	generator_t* generator = NULL;
	for (int i = 0; i < sizeof(generators) / sizeof(generators[0]); i++) {
		if (strcmp(generators[i]->name, target_str) == 0) {
			generator = generators[i];
			break;
		}
	}
	if (generator == NULL) {
		printf("Target %s not found!\n", target_str);
		return -1;
	}

	generator->emit_initial_setup(out);

	int* ids = NULL;
	int num_ids = 0;
	int curr_id = 0;

	int c = 0;
	while ((c = getc(in)) != EOF) {
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
