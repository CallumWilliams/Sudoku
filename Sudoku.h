#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*ERROR OUTPUTS*/
#define INPUTERROR() printf("Invalid input\n");
#define MEMERROR() printf("Insufficient memory\n");
#define FILENOTFOUND() printf("Unable to find file '%s'\n", argc[1]);

#define UNALLOCATEDCHECK(X) if (!X) {MEMERROR(); return 0;}

typedef enum Boolean {FALSE, TRUE} bool;

typedef struct c {
	
	char val;
	bool isEditable;
	
}Cell;

/*function for initialization of the grid
 * fp -> file pointer
 * d -> dimensions*/
Cell ***buildGrid(FILE *fp, int d) {
	
	Cell ***g;
	int i, j;
	char *fLine;
	
	/*Allocation*/
	g = malloc(sizeof(Cell)*d);
	UNALLOCATEDCHECK(g);
	
	for (i = 0; i < 9; i++) {
		
		g[i] = malloc(sizeof(Cell)*d);
		UNALLOCATEDCHECK(g[i]);
		for (j = 0; j < 9; j++) {
			
			g[i][j] = malloc(sizeof(Cell));
			UNALLOCATEDCHECK(g[i][j]);
			
		}
		
	}
	
	fLine = malloc(sizeof(char)*10);
	UNALLOCATEDCHECK(fLine);
	
	/*Adding values*/
	for (i = 0; i < 9; i++) {
		
		fLine = fgets(fLine, 100, fp);
		for (j = 0; j < 9; j++) {
			
			g[i][j]->val = fLine[j];
			if (g[i][j]->val == '0') {
				g[i][j]->isEditable = TRUE;
			}else {
				g[i][j]->isEditable = FALSE;
			}
			
		}
		
	}
	
	return g;
	
}

/*Function used to print the grid in a formatted output
 * g -> loaded grid
 * d -> dimensions*/
void printGrid(Cell ***g, int d) {
	
	int i, j;
	
	for (i = 0; i < d; i++) {
		
		if (i % (int)sqrt(d) == 0) {
			printf("-------------\n");
		}
		for (j = 0; j < d; j++) {
			
			if (j % (int)sqrt(d) == 0) {
				printf("|");
			}
			printf("%c", g[i][j]->val);
			
		}
		printf("|\n");
		
	}
	printf("-------------\n");
	
}

/*Function used to check if the grid has been solved
 * g -> loaded grid
 * d -> dimensions*/
bool gridUnsolved(Cell ***g, int d) {
	
	int i, j;
	
	for (i = 0; i < d; i++) {
		
		for (j = 0; j < d; j++) {
			
			if (g[i][j]->val == '0') {
				return TRUE;
			}
			
		}
		
	}
	
	return FALSE;
	
}

/*Function used to check whether the value works in the current cell
 * g -> loaded grid
 * d -> dimensions
 * value -> value being tested
 * currI -> current 'i' coordinate
 * currJ -> current 'j' coordinate*/
bool valueAccepted(Cell ***g, int d, int value, int currI, int currJ) {
	
	int i, j;
	int blockI, blockJ; /*stores the top-left corner of each grid*/
	
	/*checking row*/
	for (i = 0; i < d; i++) {
		
		if (i == currI) {
			/*ignore*/
		}else {
			if (g[i][currJ]->val == value) {
				printf("%d found at %d:%d (row)\n", value, i, currJ);
				return FALSE;
			}
		}
		
	}
	
	/*checking column*/
	for (i = 0; i < d; i++) {
		
		if (i == currJ) {
			/*ignore*/
		}else {
			if (g[currI][i]->val == value) {
				printf("%d found at %d:%d (col)\n", value, currI, i);
				return FALSE;
			}
		}
		
	}
	
	/*checking block*/
	blockI = (currI/sqrt(d))*sqrt(d);
	blockJ = (currJ/sqrt(d))*sqrt(d);
	for (i = blockI; i < blockI+d; i++) {
		
		for (j = blockJ; j < blockJ+d; j++) {
			
			if (i == currI && j == blockJ) {
				/*ignore*/
			}else {
				if (g[i][j]->val == value) {
					printf("%d found at %d:%d (block)\n", value, i, j);
					return FALSE;
				}
			}
			
		}
		
	}
	
	return TRUE;
	
}

/*Function used to backtrack to the last editable grid cell
 * g -> loaded grid
 * d -> dimensions
 * currI -> current 'i' coordinate
 * currJ -> current 'j' coordinate*/
Cell *returnLastSpot(Cell ***g, int d, int currI, int currJ) {
	
	do {/*find the earliest editable cell*/
		
		if (currJ == 0) {/*move up one row*/
			
			currI--;
			currJ = d - 1;
			
		}else {
			
			currJ--;
			
		}
		
	}while(!g[currI][currJ]->isEditable);
	
	return g[currI][currJ];/*return new spot*/
	
}

/*Function used to contain algorithm for solving Sudoku grid
 * g -> loaded grid
 * d -> dimenions of grid*/
bool solveGrid(Cell ***g, int d) {
	
	Cell *curr; /*pointer to current grid*/
	int i, j;
	char attemptedVal; /*value that is currently being inserted*/
	
	curr = malloc(sizeof(Cell));
	UNALLOCATEDCHECK(curr);
	
	curr = g[0][0];
	i = 0;
	j = 0;
	attemptedVal = 1;
	while (gridUnsolved(g, d)) {
		
		if (!curr->isEditable) {/*move to next one*/
			
			if (j == 8) {/*move to next row*/
				i++;
				j = 0;
			}else {
				j++;
			}
			curr = g[i][j];
			
		}else {/*work on this cell*/
			
			if (valueAccepted(g, sqrt(d), attemptedVal, i, j)) {/*insert value*/
				curr->val = attemptedVal;
				attemptedVal = 1;
				/*move forward*/
				if (j == 8) {
					i++;
					j = 0;
				}else {
					j++;
				}
				curr = g[i][j];
			}else {/*increment attemptedVal and check if backtracking is needed*/
				
				attemptedVal++;
				if (attemptedVal == d+1) {/*backtrack*/
					curr = returnLastSpot(g, d, i, j);
					attemptedVal = curr->val+1;
				}
				
			}
			
		}
		
	}
	
	return TRUE;
	
}

/*Function used to unallocate memory used by grid (at end of program)
 * g -> grid to destroy
 * d -> dimensions of grid*/
void destroyGrid(Cell ***g, int d) {
	
	int i, j;
	
	for (i = 0; i < d; i++) {
		
		for (j = 0; j < d; j++) {
			free(g[i][j]);
		}
		
	}
	free(g);
	
}
