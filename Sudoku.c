#include <stdio.h>
#include <stdlib.h>
#include "Sudoku.h"

int main (int argv, char *argc[]) {
	
	Cell ***grid; 		/*2D Array for the grid*/
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
	
	printGrid(grid, 9);
	if (!solveGrid(grid, 9)) {
		
		printf("Error solving grid\n");
		return 0;
		
	}
	printGrid(grid, 9);
	
	/*wrap-up code*/
	destroyGrid(grid, 9);
	
	return 1;
	
}
