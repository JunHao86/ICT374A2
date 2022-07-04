/*	File: token.h
 *	Author: Emmanuel
 *	Date: 23/06/22
 *	Revised: 23/06/22
 */

#define MAX_NUM_TOKENS  100
//white space, tab and newline for separating
#define tokenSeparators " \t\n"    

// note:
//		If return value ntokens >= 0, then token[ntokens] is set to NULL. 

//takes a line and splits it into several character tokens
//address to first letter in token are then stored in an array
//separator followed with null character
int tokenise (char line[], char *token[]);

