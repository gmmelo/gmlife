#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "term_display.h"
#include "life.h"

void quit(Grid g, TermDisplay d) {
	destroy_grid(g);
	destroy_display(d);
	exit(0);
}

int main() {
	TermDisplay d = init_display(128, 128);
	while (1) {
		srand(time(NULL));

		Size size = { .w = 48, .h = 48 };
		Grid g = create_grid(size);
		
		// Random-Generation Loop
		int step = 0;
		int cell_total = size.w * size.h;
		while (1) {
			int rand_idx = rand() % cell_total;
			g.cells[rand_idx] = (Cell){ .state = ALIVE };

			print_grid(g, step, &d);
			printf("\nType q to quit. Type s to start simulation. Type anything else to continue random generation.\n");	

			char c = get_one_char();	
			if (c == 'q') quit(g, d);
			if (c == 's') break;
		}

		// Simulation Loop
		while (1) {
			g = update_grid(g);		
			step++;

			print_grid(g, step, &d);
			printf("\nType q to quit. Type r to restart. Type anything else to continue simulation.\n");

			char c = get_one_char();
			if (c == 'q') quit(g, d);
			if (c == 'r') break;
			
		}
		
		destroy_grid(g);
	}
	destroy_display(d);
}
