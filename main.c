#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "term_display.h"
#include "life.h"

#define RNG_FULLNESS_FACTOR 0.3
#define RNG_FRAME_TIME 3000
#define SIMULATION_FRAME_TIME 33000
#define SIMULATION_MAX_STEPS 350

Size size = { .w = 48, .h = 48 };
TermDisplay d;
Grid g;
char keep_running = 1;

void stop() {
	keep_running = 0;
}

int main() {
	srand(time(NULL));
	d = init_display(128, 128);
	signal(SIGINT, stop);	

	while (keep_running) {
		g = create_grid(size);

		// Random-Generation Loop
		int step = 0;
		int generated = 0;
		int cell_total = size.w * size.h;
		while (keep_running) {
			int rand_idx = rand() % cell_total;
			g.cells[rand_idx] = (Cell){ .state = ALIVE };
			generated++;

			put_grid(g, step, &d);
			draw(&d);

			usleep(RNG_FRAME_TIME);
			if (generated > cell_total * RNG_FULLNESS_FACTOR) break;
		}

		// Simulation Loop
		while (keep_running) {
			g = update_grid(g);		
			step++;

			put_grid(g, step, &d);
			draw(&d);

			usleep(SIMULATION_FRAME_TIME);
			if (step > SIMULATION_MAX_STEPS) break;
			
		}

		destroy_grid(g);
	}

	destroy_display(d);
}
