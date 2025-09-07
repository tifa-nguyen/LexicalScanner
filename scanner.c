//scanner.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "scanner.h"

/* Declare the FSA table for all valid inputs, WS, EOF, and other (OTH) inputs that are not valid. */
int fsaTable[14][17] = {
/*	   A-Z 	 a-z   0-9     :     <     =     &     +     ^     #     .     [     ]     ;    WS   EOF,  OTH	*/
	{    1,   -2,    2,    3,   -1,   -1,   -1,    7,    8,    9,   10,   11,   12,   13,    0, 1000,   -1 },	/*  0: Start		*/
	{    1,    1,    1, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001 },	/*  1: ID Token		*/
	{ 1002, 1002,    2, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002 },	/*  2: NUM Token	*/
	{   -3,   -3,   -3,   -3,    4,    5,    6,   -3,   -3,   -3,   -3,   -3,   -3,   -3,   -3,   -3,   -3 },	/*  3: : Character	*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  4: :< Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  5: := Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  6: :& Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  7: + Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  8: ^ Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/*  9: # Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/* 10: . Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/* 11: [ Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 },	/* 12: ] Token		*/
	{ 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003 }	/* 13: ; Token		*/
};

/* Declare character arrays for the keywords, operators, and delimiters that scanner() will compare IDTk and OPTk values and
 * assign the appropriate final token. */
char *keywords[] = { "Begin", "End", "Repeat", "If", "Then", "Let", "Hop", "Mark", "Var", "Return", "See", "Scribe", "Code" };
char *opAndDelim[] = { ":<", ":=", ":&", "+", "^", "#", ".", "[", "]", ";" };

/* Global variables */
int lineNum = 1;
data_t nextChar;

/* This function performs a lookahead and assigns to nextChar */
void lookahead(FILE *input) {
	nextChar = filter(input);
}

/* This function is the scanner that checks the state of the automaton and processes nextChar */
Token scanner(FILE *input) {
	tokenID state = 0;
	tokenID nextState;
	Token token;
	char tokenString[MAX_TOKEN_LENGTH] = "";

	/* If the state is less than a final state (1000s), set appropriate tokens for errors and final tokens. Else, append the tokenString */
	while (state < 1000) {
		nextState = fsaTable[state][nextChar.col];

		if (nextState < 0) {	// If less than 0, the state is an error
			if (nextState == -1) {	// Error for other characters not allowed by scanner
				printf("SCANNER ERROR: No token starts with '%c' - Line %d\n", nextChar.ch, nextChar.line);
				exit(1);
			}
			else if (nextState == -2) {	// Error for no tokens starting with lower case letters
				printf("SCANNER ERROR: No token starts with a lower case '%c' - Line %d\n", nextChar.ch, nextChar.line);
				exit(1);
			}
			else if (nextState == -3) {
				printf("SCANNER ERROR: ':%c' is not a valid token - Line %d\n", nextChar.ch, nextChar.line);
			}
		}
		if (nextState >= 1000) {	// If state is final
			if (nextState == 1001) {	// If the state is an IDTk, check if the string is a keyword and assign appropriate final token
				int n;
				for (n = 0; n < MAX_KEYWORDS; n++) { 
					if (strcmp(tokenString, keywords[n]) == 0) {	// If token string is a keyword, set appropriate keyword token
						token.type = (1013 + n);		// Assign appropriate token state based on keyword offset.
						strcpy(token.string, tokenString);
						token.line = nextChar.line;

						return token;
					}
					else {	// Else, set token string as IDTk
						token.type = nextState;
						strcpy(token.string, tokenString);
						token.line = nextChar.line;
					}
				}
				return token;
			}
			else if (nextState == 1003) {	// If state is an Operator Token, compare to assign appropriate final token
				int n;
				for (n = 0; n < MAX_OPDELIM; n++) {
					if (strcmp(tokenString, opAndDelim[n]) == 0) {
						token.type = (1003 + n);	// Assign appropriate token state based on operator and delimiter offset
						strcpy(token.string, tokenString);
						token.line = nextChar.line;
					}
				}
				return token;
			}
			else if (nextState == 1000) {	// If final token is EOF, set token.string to WS for printf formatting
				token.type = nextState;
				strcpy(token.string, " ");
				token.line = nextChar.line;

				return token;
			}
			else {	// If not an IDTk, assign final token
				token.type = nextState;
				strcpy(token.string, tokenString);
				token.line = nextChar.line;

				return token;
			}
		}
		else {	// Else if not final state
			state = nextState;
			/* If nextChar is not WS, append nextChar to tokenString and move NULL character by 1 */
			if (nextChar.col != 14) {
				tokenString[strlen(tokenString)] = nextChar.ch;
				tokenString[strlen(tokenString) + 1] = '\0';
			}

			/* Get next character from filter */
			nextChar = filter(input);
			
			/* If the tokenString is the MAX_TOKEN_LENGTH (8), create IDTk */
			if (strlen(tokenString) == MAX_TOKEN_LENGTH) {
				token.type = state;
				strcpy(token.string, tokenString);
				token.line = nextChar.line;

				return token;
			}
		}
	}
	return token;
}

/* This function is the filter that counts lines, skips over white spaces and comments, constructs a string
 * of characters for the current token, and returns the column number in the FSA table */
data_t filter(FILE *input) {
	data_t temp;
	char c = fgetc(input);
	int column = fsaCol(c);

	while (column == 17) {		// If column returns for comments
		while (c != '%') {	// Read file until closing comment symbol
			c = fgetc(input);
			if (c == '\n') {	// If character is new line, increment lineNum counter
				lineNum++;
			}
		}
		c = fgetc(input);
		column = fsaCol(c);
	}

	if (column == 16) {	// If the column is for other characters, print error
		printf("SCANNER ERROR: No token starts with '%c' - Line %d\n", c, lineNum);
		exit(1);
	}
	else {	// Else, assign values to data_t structure
		temp.ch = c;
		temp.col = column;
		temp.line = lineNum;
		if (c == '\n') {	// If character is new line, increment lineNum counter
			lineNum++;
		}
		return temp;
	}
}

/* This function validates the character and returns the column number from the FSA table */
int fsaCol(char ch) {
	int column;
	if (isalpha(ch)) {
		if (isupper(ch)) {	// A-Z	IDTk
			column = 0;
		}
		else if (islower(ch)) {	// a-z	IDTk
			column = 1;
		}
	}
	else if (isdigit(ch)) {		// 0-9	IDTk or NUMTk
		column = 2;
	}
	else if (ch == ':') {		// : for :<, :=, and :&
		column = 3;
	}
	else if (ch == '<') {		// < for :<	RELATETk
		column = 4;
	}
	else if (ch == '=') {		// = for :=	ASSIGNTk
		column = 5;
	}
	else if (ch == '&') {		// & for :&	ANDTk
		column = 6;
	}
	else if (ch == '+') {		// +	ADDTk
		column = 7;
	}
	else if (ch == '^') {		// ^	XORTk
		column = 8;
	}
	else if (ch == '#') {		// #	PREPROCTk
		column = 9;
	}
	else if (ch == '.') {		// .	MEMBERTk
		column = 10;
	}
	else if (ch == '[') {		// [	LBRACKETTk
		column = 11;
	}
	else if (ch == ']') {		// ]	RBRACKETTk
		column = 12;
	}
	else if (ch == ';') {		// ;	SemicolTk
		column = 13;
	}
	else if (isspace(ch)) {		// WS	White Space
		column = 14;
	}
	else if (ch == EOF) {		// EOF	End of File
		column = 15;
	}
	else if (ch == '%') {		// % for comments
		column = 17;
	}
	else {				// Invalid characters
		column = 16;
	}

	return column;
}
