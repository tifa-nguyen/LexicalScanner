//token.h
#ifndef TOKEN_H
#define TOKEN_H

/* Max length of tokens + 1 for lookahead */
#define MAX_TOKEN_LENGTH 9

typedef enum {
	/* EOF, ID, NUM
	 * Operators and delimiters
	 * Keywords
	 */

	EOFTk = 1000, IDTk = 1001, NUMTk = 1002,
	RELATETk = 1003, ASSIGNTk = 1004, ANDTk = 1005, ADDTk = 1006, XORTk = 1007, PREPROCTk = 1008, MEMBERTk = 1009, LBRACKETTk = 1010, RBRACKETTk = 1011, SemicolTk = 1012,
	BeginTk = 1013, EndTk = 1014, RepeatTk = 1015, IfTk = 1016, ThenTk = 1017, LetTk = 1018, HopTk = 1019, MarkTk = 1020, VarTk = 1021, ReturnTk = 1022, SeeTk = 1023, ScribeTk = 1024, CodeTk = 1025
} tokenID;

typedef struct {
	tokenID type;
	char string[MAX_TOKEN_LENGTH];
	int line;
} Token;

#endif
