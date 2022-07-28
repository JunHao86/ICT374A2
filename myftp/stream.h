/*	Filename: stream.h
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): TBD
 *  Description: File used for stream reading and stream writing
 */

#define MAX_BLOCK_SIZE (1024*5)    /* maximum size of any piece of */
                                   /* data that can be sent by client */


/*
 * purpose:  write "nbytes" bytes from "buf" to "fd".
 * pre:      1) nbytes <= MAX_BLOCK_SIZE,
 * post:     1) nbytes bytes from buf written to fd;
 *           2) return value = nbytes : number ofbytes written
 *                           = -3     : too many bytes to send 
 *                           otherwise: write error
 */   


//================================================================


/*
 * Pairs of read-write functions for data on a socket, between client and server
 * 1. readn 
 *    writen
 *    Function:
 * 
 * 2. read_onebyte_length  
 *    write_onebyte_length
 *    Function: 
 * 
 * 3. read_twobyte_length  
 *    write_twobyte_length
 *    Function: 
 * 
 * 4. read_fourbyte_length  
 *    write_fourbyte_length
 *    Function: 
 */
int readn(int sd, char *buf, int bytesize);
int writen(int sd, char *buf, int bytesize);

int read_onebyte_length(int sd, char *opcode);
int write_onebyte_length(int sd,char opcode);

int read_twobyte_length(int sd, int *length);
int write_twobyte_length(int sd, int length);

int read_fourbyte_length(int sd, int *length);
int write_fourbyte_length(int sd, int length);

