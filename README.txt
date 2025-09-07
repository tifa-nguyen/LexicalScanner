Using Option 2:	FSA table + driver
		FSA table located in scanner.c

While debugging the program, enum was not working properly. Because of this, the tokenNames[tokenID] call was not printing the token description correctly in testScanner.c.

To troubleshoot this, I had to do [tokenID - 1000] to get the correct index number. It's also possible the token type was not being assigned properly in scanner.c when assigning the state to token.type.
