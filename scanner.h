//scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#define MAX_KEYWORDS 13
#define MAX_OPDELIM 10

typedef struct data_struct {
	char ch;
	int col;
	int line;
} data_t;

void lookahead(FILE *);
Token scanner(FILE *);
data_t filter(FILE *);
int fsaCol(char);

#endif
