//main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "token.h"
#include "testScanner.h"

void help(char* []);

int main(int argc, char* argv[]) {
	int option;
	FILE *fptr = NULL;
	char buffer[256];

        /* Switch Case for getopt */
	while ((option = getopt(argc, argv, "h")) != -1) {
		switch(option) {
			case 'h':
			help(argv);
			exit(0);
		case '?':
			fprintf(stderr, "Invalid option. Type %s -h for usage help.\n", argv[0]);
			exit(1);
		default:
			break;
		}
	}

	/* Check number of arguments */
	if (argc >= 3) {	// If 3 or more arguments, print error and exit.
		fprintf(stderr, "Fatal: Improper usage\n");
		help(argv);
		exit(1);
	}
	else if (argc == 2) {	// If exactly 2 arguments, use argument as filename to open
		fptr = fopen(argv[1], "r");
	}
	else if (argc == 1) {   // If 1 argument, get filename input from stdin until EOF
		do {
			scanf("%s", buffer);
		} while (feof(stdin));
	
		if ((fptr = fopen(buffer, "r")) == NULL) {
			fprintf(stderr, "Error: '%s' could not be opened or does not exist.\n", buffer);
			exit(1);
		}
	}

	/* Call to testScan() with fptr */
	testScan(fptr);
	
	/* When testScan is complete, close fptr */
	fclose(fptr);
	
	return 0;
}

/* This function prints a help message to screen */
void help(char* argv[]) {
	printf("Usage:\n");
	printf("   %s [filename]", argv[0]);
	printf("\nOptions:\n");
	printf("   -h     display help message and exit\n");
	printf("\nfilename  optional source filename to scan from\n\n");
}
