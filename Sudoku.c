#include <stdio.h>
#include <stdlib.h>
#include "Sudoku.h"

int main (int argv, char *argc[]) {
	
	Cell ***grid; 		/*2D Array for the grid*/
	int i, j;			/*counter variables*/
	FILE *fp;			/*file pointer for input*/
	
	if (argv != 2) {
		INPUTERROR();
		return 0;
	}
	
	fp = fopen(argc[1], "r");
	if (!fp) {
		FILENOTFOUND();
		return 0;
	}
	
	/*Initializing grid*/
	grid = buildGrid(fp, 9);
	fclose(fp);
	
	printGrid(grid, 3);
	if (!solveGrid(grid, 9)) {
		
		printf("Error solving grid\n");
		return 0;
		
	}
	printGrid(grid, 3);
	
	/*wrap-up code*/
	destroyGrid(grid, 9);
	
	return 1;
	
}
