#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*Function used to generate a set of accepted values
 * d -> dimensions (amount of accepted values)*/

char *generateAcceptedValues(int d) {
	
	char *values;
	int i;
	
	values = malloc(sizeof(char)*d);
	UNALLOCATEDCHECK(values);
	
	for (i = 0; i < d; i++) {
		
		if (i + 49 < 58) {/*use regular numbers (1-9)*/
			
			values[i] = i + 49;
			
		}else {/*start using characters (A-Z)*/
			
			values[i] = i + 56;
			
		}
		
	}
	
	return values;
	
}

/*Function used to find the location of a value in the avs
 * avs -> Accepted Value Set
 * v -> value to find*/
int findValueInSet(char *avs, char v) {
	
	int i;
	
	for (i = 0; i < (int)strlen(avs); i++) {
		
		if (avs[i] == v) {
			return i;
		}
		
	}
	
	return -2;
	
}

/*Function for initialization of the grid
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
bool valueAccepted(Cell ***g, int d, char value, int currI, int currJ) {
	
	int i, j;
	int blockI, blockJ; /*stores the top-left corner of each grid*/
	
	/*checking row*/
	for (i = 0; i < d; i++) {
		
		if (i == currJ) {
			/*ignore*/
		}else {
			if (g[currI][i]->val == value) {
				return FALSE;
			}
		}
		
	}
	
	/*checking column*/
	for (i = 0; i < d; i++) {
		
		if (i == currI) {
			/*ignore*/
		}else {
			if (g[i][currJ]->val == value) {
				return FALSE;
			}
		}
		
	}
	
	/*checking block*/
	blockI = currI/sqrt(d);
	blockJ = currJ/sqrt(d);

	blockI = blockI*sqrt(d);
	blockJ = blockJ*sqrt(d);
	
	for (i = blockI; i < blockI+sqrt(d); i++) {
		
		for (j = blockJ; j < blockJ+sqrt(d); j++) {
			
			if (i == currI && j == currJ) {
				/*ignore*/
			}else {
				if (g[i][j]->val == value) {
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
int *returnLastSpot(Cell ***g, int d, int currI, int currJ) {
	
	int *newCoord;
	
	newCoord = malloc(sizeof(int)*2);
	UNALLOCATEDCHECK(newCoord);
	
	do {/*find the earliest editable cell*/
		
		if (currJ == 0) {/*move up one row*/
			
			currI--;
			currJ = d - 1;
			
		}else {
			
			currJ--;
			
		}
		
		
	}while(!g[currI][currJ]->isEditable);
	
	newCoord[0] = currI;
	newCoord[1] = currJ;
	
	return newCoord;
	
}

/*Function used to contain algorithm for solving Sudoku grid
 * g -> loaded grid
 * d -> dimenions of grid
 * avs -> Accepted Value Set*/
bool solveGrid(Cell ***g, int d, char *avs) {
	
	Cell *curr; /*pointer to current grid*/
	int i, j;
	int *newCoord; /*used for finding new location for backtracker*/
	char attemptedVal; /*value that is currently being inserted*/
	int avsCounter;
	
	curr = malloc(sizeof(Cell));
	UNALLOCATEDCHECK(curr);
	
	curr = g[0][0];
	i = 0;
	j = 0;
	attemptedVal = avs[0];
	avsCounter = 0;
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
			
			if (valueAccepted(g, d, attemptedVal, i, j)) {/*insert value*/
				curr->val = attemptedVal;
				avsCounter = 0;
				attemptedVal = avs[avsCounter];
				/*move forward*/
				if (j == 8) {
					i++;
					j = 0;
				}else {
					j++;
				}
				curr = g[i][j];
			}else {/*increment attemptedVal and check if backtracking is needed*/
				
				avsCounter++;
				attemptedVal = avs[avsCounter];
				if (avsCounter == d) {/*backtrack*/
					if ((curr == g[0][0])) {/*not possible to further backtrack, puzzle not possible*/
						return FALSE;
					}
					/*get previous position*/
					do {/*keep going until we find a cell that has more options*/
						
						newCoord = returnLastSpot(g, d, i, j);
						i = newCoord[0];
						j = newCoord[1];
						avsCounter = findValueInSet(avs, g[i][j]->val);
						avsCounter = findValueInSet(avs, g[i][j]->val);
						if (avsCounter == -2) {/*should NEVER happen, unless something corrupts the avs*/
							printf("Unexpected error\n");
							return FALSE;
						}
						
					}while(avsCounter == (int)strlen(avs)-1);
					
					attemptedVal = avs[avsCounter+1];
					/*move and clear curr pointer*/
					curr = g[i][j];
					curr->val = '0';
					
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
