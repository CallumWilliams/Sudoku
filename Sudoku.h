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

