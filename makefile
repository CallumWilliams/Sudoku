CFLAGS = -Wall

all:
	gcc $(CFLAGS) Sudoku.c -o Sudoku -lm
	
clean:
	rm Sudoku
