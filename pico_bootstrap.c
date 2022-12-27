#include <stdio.h>
#include "pico/stdlib.h"

char tape[30000] = { 0 };

void bf_main(char* tape);
int main() {
	stdio_init_all();
	
	sleep_ms(30 * 1000);
	
	printf("starting bf...\n");
	bf_main(tape);
	
	while (true) {}
}