#include <stdio.h>
#include "life.h"

#ifndef TERM_DISPLAY_H
#define TERM_DISPLAY_H

typedef struct {
	int w;
	int h;
	int cur_idx;
	char* buf;
} TermDisplay;

TermDisplay init_display(int w, int h) {	
	TermDisplay d = { .w = w, .h = h, .cur_idx = 0 };
	d.buf = malloc(w * h * sizeof(char));
	return d;
}

void destroy_display(TermDisplay d) {
	free(d.buf);
}

void display_panic() {
	printf("PANIC: DISPLAY BUFFER OVERFLOW OCCURRED. EXITING!\n");
	exit(1);
}

void put_char(char c, TermDisplay* d) {
	d->buf[d->cur_idx] = c;
	d->cur_idx++;
}

void put_string(char* s, int size, TermDisplay* d) {
	for (int i = 0; i < size; i++) {
		char to_copy = s[i];
		if (to_copy == 0) return;
		if (d->cur_idx >= d->w * d->h) display_panic();
		d->buf[d->cur_idx] = to_copy;
		d->cur_idx++;	
	}	
}

void draw(TermDisplay* d) {
	if (d->cur_idx >= d->w * d->h) display_panic();
	d->buf[d->cur_idx] = 0;
	printf("%s", d->buf); 
	d->cur_idx = 0;
}

void print_space(int size, TermDisplay* d) {
	for (int i = 0; i < size; i++) put_char(' ', d);
}

void print_x_label(int width, int space_before, TermDisplay* d) {
	print_space(space_before, d);
	for (int i = 0; i < width; i++) {
		char s[3];
		if (i % 10 == 0)	
			snprintf(s, 3, "%1d ", i / 10);
		else
			snprintf(s, 3, "  ");
		put_string(s, 3, d);
	}
	put_char('\n', d);

	print_space(space_before, d);
	for (int i = 0; i < width; i++) {
		char s[3];
		snprintf(s, 3, " %d", i % 10);
		put_string(s, 3, d);
	}
	put_char('\n', d);	
}

void print_line(int width, char symbol, int space_before, TermDisplay* d) {
	print_space(space_before, d);
	put_char('*', d);
	for (int i = 0; i < width; i++) {
		put_char(symbol, d);
	}
	put_string("*\n", 3, d);
}

void print_grid(Grid grid, int step, TermDisplay* d) {
	int space_before = 4;
	char title[64];
	snprintf(title, 64, "gmlife - step %d\n", step);
	put_string(title, 64, d);
	print_line(grid.size.w * 2, '-', space_before, d);
	for (int y = 0; y < grid.size.h; y++) {
		char s[6]; 
		snprintf(s, 6, "%2d. |", y);
		put_string(s, 6, d);
		for (int x = 0; x < grid.size.w; x++) {
			char cell_str[3];
			if (cat(grid, x, y) == ALIVE) {
				snprintf(cell_str, 3, "%s", ALIVE_STRING);
			}
			else {
				snprintf(cell_str, 3, "%s", DEAD_STRING);
			}
			put_string(cell_str, 3, d);
		}
		put_string("|\n", 3, d);
	}
	print_line(grid.size.w * 2, '-', space_before, d);
	print_x_label(grid.size.w, space_before + 1, d);
	draw(d);
}

#endif // TERM_DISPLAY_H
