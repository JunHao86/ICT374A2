/*	Filename: myftp.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): 04/07/22
 *  Description: Assignment 2 - Simple File Transfer Protocol - Client Program
 */

#include  <netinet/in.h>       /* struct sockaddr_in, htons, htonl */
#include  <netdb.h>            /* struct hostent, gethostbyname() */ 

#include  <stdio.h>   
#include  <stdlib.h>    
#include  <string.h>
#include  <sys/types.h>        
#include  <sys/socket.h>
#include  <unistd.h>    
#include "token.h"            /* client commands cd,lcd,get,put */            

//================================================================

#define SERV_TCP_PORT 40004 //Listening TCP Port Number
#define BUFSIZE         256
#define MAX_NUM_TOKENS  2   //Token 1 : Command name
                            //Token 2 : Filename/Pathname (if using cd,lcd,get,put)

//================================================================

/*
 * To do list
 * 1. Write functions for client commands
 * 2. Uncheck dirent.h    sys/stat.h    fcntl.h if necessary 
 * 3. Get allocated port number
 */

//================================================================

/*
No. of args - Description
argc == 1   - Uses localhost and default TCP port
            - ./myftp
argc == 2   - User defines hostname, uses default TCP port          
            - ./myftp <hostname>
*/
int main(int argc, char *argv[])
{
    int sd;
    int length;
    char buf[BUFSIZE],host[60];
    char *tokenArray[MAX_NUM_TOKENS];

    struct sockaddr_in ser_addr; 
    struct hostent *hp;
    
    //getting hostname (uses default port)
    if (argc==1)  // assume server runs on local host 
        gethostname(host, sizeof(host));
    else if (argc == 2) // use the given hostname
        strcpy(host, argv[1]);
    else {
        printf("Usage: %s [<server_host_name>]\n", argv[0]); exit(1); 
    }

    // getting host address, building server socket address
    bzero((char *) &ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERV_TCP_PORT);
    if ((hp = gethostbyname(host)) == NULL){
        printf("host %s not found\n", host); exit(1);   
    }
    ser_addr.sin_addr.s_addr = * (u_long *) hp->h_addr;

    /* create TCP socket & connect socket to server address */
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(sd, (struct sockaddr *) &ser_addr, sizeof(ser_addr))<0) { 
        perror("client connect"); exit(1);
    }
    
    /*
    client program from here
    */

    while(1) // should we use while loop or switch-case?
    {
        printf(">   "); //display prompt
        fgets(buf,sizeof(buf),stdin);
        length = strlen(buf);

        //remove newline from input line
        if (length > 0 && buf[length-1] == '\n')
        {
            buf[length-1] = '\0';
            length--;
        }      
        tokenise(buf,tokenArray);

        //if-else statements for functions (pwd,lpwd,dir,ldir,cd,lcd,get,put,quit)
        if(strcmp(tokenArray[0],"quit")==0)
        {
            printf("Terminating myftp session\n");
            exit(0);
        }
        else if(strcmp(tokenArray[0],"pwd")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"lpwd")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"dir")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"ldir")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"cd")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"lcd")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"get")==0)
        {
            //function here
        }
        else if(strcmp(tokenArray[0],"put")==0)
        {
            //function here
        }
        else
        {
            printf("Invalid command, here are the available commands: \n");
            printf("Standalone: pwd, lpwd, dir, ldir, quit\n");
            printf("Require filename/pathname: cd, lcd, get, put\n");
        }
    }
    exit(0);
}