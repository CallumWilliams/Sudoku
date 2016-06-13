CFLAGS = -Wall

all:
	gcc $(CFLAGS) Sudoku.c -o Sudoku
	
clean:
	rm Sudoku
