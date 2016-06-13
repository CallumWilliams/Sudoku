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
 * d -> dimensions of a single block (e.x. if using 9x9, d would be 3)*/
void printGrid(Cell ***g, int d) {
	
	int i, j;
	
	for (i = 0; i < 9; i++) {
		
		if (i % 3 == 0) {
			printf("-------------\n");
		}
		for (j = 0; j < 9; j++) {
			
			if (j % 3 == 0) {
				printf("|");
			}
			printf("%c", g[i][j]->val);
			
		}
		printf("|\n");
		
	}
	printf("-------------\n");
	
}

/*Function used to contain algorithm for solving Sudoku grid
 * g -> loaded grid
 * d -> dimenions of grid*/
bool solveGrid(Cell ***g, int d) {
	
	Cell *curr; /*pointer to current grid*/
	int i, j;
	int coord[2]; /*coordinate used to backtrack to*/
	
	curr = malloc(sizeof(Cell));
	UNALLOCATEDCHECK(curr);
	
	curr = g[0][0];
	i = 0;
	j = 0;
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
			
			
			
		}
		
	}
	
	return TRUE;
	
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


/*Function used to backtrack to the last editable grid cell
 * g -> loaded grid
 * d -> dimensions
 * currI -> current 'i' coordinate
 * currJ -> current 'j' coordinate*/
Cell *returnLastSpot(Cell ***g, int d, int currI, int currJ) {
	
	int newCoord[2];
	
	while(!g[newCoord[0]][newCoord[1]]->isEditable) {/*find the earliest editable cell*/
		
		if (newCoord[1] == 0) {/*move up one row*/
			
			newCoord[0]--;
			newCoord[1] = d - 1;
			
		}else {
			
			newCoord[1]--;
			
		}
		
	}
	
	return g[newCoord[0]][newCoord[1]];/*return new spot*/
	
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
