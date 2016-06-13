#include <stdio.h>
#include <stdlib.h>
#include "Sudoku.h"

int main (int argv, char *argc[]) {
	
	Cell ***grid; 		/*2D Array for the grid*/
	int i, j;			/*counter variables*/
	FILE *fp;			/*file pointer for input*/
	char *fLine; 		/*line reading variable for fp*/
	
	if (argv != 2) {
		INPUTERROR();
		return 0;
	}
	
	grid = malloc(sizeof(Cell));
	UNALLOCATEDCHECK(grid);
	
	for (i = 0; i < 9; i++) {
		
		grid[i] = malloc(sizeof(Cell)*9);
		UNALLOCATEDCHECK(grid[i]);
		for (j = 0; j < 9; j++) {
			
			grid[i][j] = malloc(sizeof(Cell));
			UNALLOCATEDCHECK(grid[i][j]);
			
		}
		
	}
	
	fLine = malloc(sizeof(char)*10);
	UNALLOCATEDCHECK(fLine);
	
	fp = fopen(argc[1], "r");
	if (!fp) {
		FILENOTFOUND();
		return 0;
	}
	
	/*Initializing grid*/
	for (i = 0; i < 9; i++) {
		
		fLine = fgets(fLine, 100, fp);
		for (j = 0; j < 9; j++) {
			
			grid[i][j]->val = fLine[j];
			printf("%c\n", fLine[j]);
			if (grid[i][j]->val == '0') {
				grid[i][j]->isEditable = FALSE;
			}else {
				grid[i][j]->isEditable = TRUE;
			}
			printf("Cell %d contains %c, editable = %d\n", (i*9)+j, grid[i][j]->val, grid[i][j]->isEditable);
		}
		printf("---------------------------------------------------\n");
	}
	
	/*wrap-up code*/
	
	
	return 1;
	
}
