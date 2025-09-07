//testScanner.c

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "testScanner.h"
#include "scanner.h"

/* Declare token names for printing description to screen */
char *tokenNames[] = {
	"EOF", "Identifier", "Number", "Relation", "Assignment", "And", "Add", "XOR", "Preprocessor", "Member", "Left Bracket", "Right Bracket", "Semicolon",
	"Begin", "End", "Repeat", "If", "Then", "Let", "Hop", "Mark", "Var", "Return", "See", "Scribe", "Code"
};

void testScan(FILE *input) {
	Token token;
	
	lookahead(input);	// Call lookahead and set initial nextChar

	while (token.type != EOFTk) {	// If not EOFTk, call scanner();
		token = scanner(input);
		printf("Line %d	Value: %s	Token: %s\n", token.line, token.string, tokenNames[token.type - 1000]);	// Offset of -1000 to get appropriate index number for token names
	}
}
