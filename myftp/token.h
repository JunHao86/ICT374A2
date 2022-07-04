/*	Filename: token.h
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): 04/07/22
 *  Description: Tokeniser to split a line into several character tokens
 */

#define MAX_NUM_TOKENS  100
#define tokenSeparators " \t\n" //white space, tab and newline for separating

/*
 * Takes a line and splits it into several character tokens
 * Uses tokenSeparators to determine when to split the line into tokens
 * Returns  - Description
 * >=0      - Number of tokens from the command line
 * -1       - Failure
 * Notes    - If return value ntokens >= 0, then token[ntokens] is set to NULL. 
 */
int tokenise (char line[], char *token[]);

