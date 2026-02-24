#include <stdio.h>
#include <stdlib.h>

#ifndef LIFE_H
#define LIFE_H

#define ALIVE_STRING "[]"
#define DEAD_STRING "  "

typedef enum {
	DEAD,
	ALIVE
} CellState;

typedef struct {
	CellState state;
} Cell;

typedef struct {
	int w;
	int h;
} Size;

typedef struct {
	Size size;
	Cell* cells;
	Cell* temp_cells;
} Grid;

/*
* createGrid(Size size)
* 
* Will allocate grid cells on the heap.
* 
* Should always be paired with destroyGrid()
* to avoid memory leaks. 
*/
Grid create_grid(Size size) {
	int totalCells = size.w * size.h;
	Grid grid = {
		.size = size,
		.cells = malloc(totalCells * sizeof(int)),
		.temp_cells = malloc(totalCells * sizeof(int))
	};

	for (int i = 0; i < totalCells; i++) {
		grid.cells[i] = (Cell){ .state = DEAD };
	}

	return grid;
}

/*
* temp_set
*
* Sets the state of a temporary cell
* at location (x, y).
*/
void temp_set(Grid grid, int x, int y, CellState state) {
	char out_of_bounds = x < 0 || x >= grid.size.w || y < 0 || y >= grid.size.h;
	if (out_of_bounds) {
		printf("WARNING: Tried to set the state of an out-of-bounds cell.\n");
		return;
	}
	
	int index = (y * grid.size.w) + x;
	grid.temp_cells[index].state = state;
}

/*
* cat (get Cell AT location) * 
* Returns the state of the cell
* at location (x, y)
*/
CellState cat(Grid grid, int x, int y) {
	/* 
	*  For now, we need to return "dead" if we try
	*  to read a cell outside our simulation frame.
	*  This is because we don't yet support grid
	*  resizing.
	*
	*  It will make for innacurate simulation when 
	*  cells go out of bounds, but should make our
	*  logic much simpler. 
	*/
	char out_of_bounds = x < 0 || x >= grid.size.w || y < 0 || y >= grid.size.h;
	if (out_of_bounds) return DEAD;

	int index = (y * grid.size.w) + x;
	return grid.cells[index].state;
}

void destroy_grid(Grid grid) {
	free(grid.cells);
	free(grid.temp_cells);
}

int count_alive_neighbors(Grid grid, int x, int y) {
	int total = 0;
	for (int offset_y = -1; offset_y <= 1; offset_y++) {
		for (int offset_x = -1; offset_x <= 1; offset_x++) {
			if (offset_x == 0 && offset_y == 0) continue;
			CellState state = cat(grid, x + offset_x, y + offset_y);
			if (state == ALIVE) {
				total++;
			} 
		}
	}
	return total;
}


Grid update_grid(Grid grid) {
	for (int y = 0; y < grid.size.h; y++) {
		for (int x = 0; x < grid.size.w; x++) {
			int alive_neighbors = count_alive_neighbors(grid, x, y);
			if (alive_neighbors == 2 || alive_neighbors == 3)
				temp_set(grid, x, y, ALIVE); 
			else temp_set(grid, x, y, DEAD);
		}
	}

	Cell* temp = grid.cells;
	grid.cells = grid.temp_cells;
	grid.temp_cells = temp;
	return grid;
}

char get_one_char() {
	char c = getchar();
	if (c != '\n' && c != EOF) {
		char temp;
		while ((temp = getchar()) != '\n' && temp != EOF); // Consume extra characters from stdin. C/Unix quirk.
	}
	return c;
}

#endif // LIFE_H
