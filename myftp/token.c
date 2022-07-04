/*	Filename: token.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): 04/07/22
 *  Description: Tokeniser to split a line into several character tokens
 */

#include <string.h>
#include <stdio.h>
#include "token.h"


int tokenise (char line[], char *token[])
{
    char *tk;
    int i=0;

    //first token is from start of line until first of any -> " \t\n" 
    tk = strtok(line, tokenSeparators); 

    //starting address of token stored in array
    token[i] = tk; 


    //runs through remainder of the line 
    while (tk != NULL) 
    {
        ++i; 
        if (i>=MAX_NUM_TOKENS) //if i >= 100, stops taking in new tokens
        {
        i = -1; 
        break;
        }
        tk = strtok(NULL, tokenSeparators);
        token[i] = tk;
    }
    return i;
}
