/*	Filename: myftp.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): TBD
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
#include  <dirent.h>           /* client commands dir, ldir */
#include  "token.h"            /* client commands cd,lcd,get,put */            

//================================================================

#define SERV_TCP_PORT 40228 //Assigned Listening TCP Port Number
#define BUFSIZE         256
#define MAX_TOKENS        2 //Token 1 : Command name
                            //Token 2 : Filename/Pathname
//================================================================

/*
 * To do list
 * 1. Write functions for client-server commands
 * 2. Uncheck dirent.h    sys/stat.h    fcntl.h if necessary 
 */

//================================================================

/*
 * Client Side Commands
 * 1. func_lpwd - to display current working directory of the client
 * 2. func_ldir - to display the file names under the current directory
 *                of the client
 * 3. func_lcd  - to change the current directory of the client
 */

void func_lpwd()
{
    char working_directory[BUFSIZE];

    getcwd(working_directory,sizeof(working_directory));
    printf("%s\n",working_directory);
}
 
void func_ldir(char* token1)
{
	DIR *dir;
	struct dirent *directory;

    if(token1 == NULL) //if no tokens specified, set as cwd
    {
		token1 = ".";
	}
	if ((dir = opendir(token1))!= NULL)
    {
    	while ((directory = readdir(dir)) != NULL)
        {
	        printf("Filename: %s\n", directory->d_name);
	    }
	    closedir(dir);
	}
    else
    {
        perror("dirent error");
    }
}

void func_lcd(char* token1)
{
    char working_directory[BUFSIZE];
    if(chdir(token1)==0)
    {
        getcwd(working_directory,sizeof(working_directory));
        printf("Directory changed to : %s\n",working_directory);
    }
    else
    {
        perror("unable to change to specified directory");
    }
}

//================================================================

/*
 * Server Side Commands
 * 1. func_pwd - to display the current directory of the server
 *               that is serving the client
 * 2. func_dir - to display the file names under the current directory 
 *               of the server that is serving the client
 * 3. func_cd  - to change the current directory 
 *               of the server that is serving the client
 * 4. func_get - to download the named file from the current directory 
 *               of the remote server 
 *               and save it in the current directory of the client
 * 5. func_put - to upload the named file from the current directory of 
 *               the client to the current directory of the remove server
 */

void func_pwd(int sd, char* token)
{

}

void func_dir(int sd, char* token)
{

}

void func_cd(int sd, char* token)
{

}

void func_get(int sd, char* filename)
{

}

void func_put(int sd, char* filename)
{

}

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
    char *tokenArray[MAX_TOKENS];

    struct sockaddr_in ser_addr; 
    struct hostent *hp;
    
    //getting hostname (uses default port)
    if (argc==1)  // assume server runs on local host 
        gethostname(host, sizeof(host));
    else if (argc == 2) // use the given hostname
        strcpy(host, argv[1]);
    else 
    {
        printf("Usage: %s [<server_host_name>]\n", argv[0]); 
        exit(1); 
    }

    //getting host address, building server socket address
    bzero((char *) &ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERV_TCP_PORT);
    if ((hp = gethostbyname(host)) == NULL)
    {
        printf("host %s not found\n", host); 
        exit(1);   
    }
    ser_addr.sin_addr.s_addr = * (u_long *) hp->h_addr;

    //create TCP socket & connect socket to server address
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(sd, (struct sockaddr *) &ser_addr, sizeof(ser_addr))<0) 
    { 
        perror("client connect"); 
        exit(1);
    }
    
    while(1) 
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

        //if-else statements for functions 
        if(strcmp(tokenArray[0],"quit")==0)
        {
            printf("Terminating myftp session\n");
            exit(0);
        }
        else if(strcmp(tokenArray[0],"pwd")==0) //server side command 1
        {
            printf("pwd session\n");
            //function here
        }
        else if(strcmp(tokenArray[0],"lpwd")==0) 
        {
            func_lpwd();
        }
        else if(strcmp(tokenArray[0],"dir")==0) //server side command 2
        {
            printf("dir session\n");
            //function here
        }
        else if(strcmp(tokenArray[0],"ldir")==0) 
        {
            func_ldir(tokenArray[1]);
        }
        else if(strcmp(tokenArray[0],"cd")==0) //server side command 3
        {
            printf("cd session\n");
            //function here
        }
        else if(strcmp(tokenArray[0],"lcd")==0) 
        {
            func_lcd(tokenArray[1]);
        }
        else if(strcmp(tokenArray[0],"get")==0) //server side command 4
        {
            printf("get session\n");
            //function here
        }
        else if(strcmp(tokenArray[0],"put")==0) //server side command 5
        {
            printf("put session\n");
            //function here
        }
        else
        {
            printf("Invalid command, here are the available commands: \n");
            printf("Requires a filename/pathname: cd, lcd, get, put\n");
            printf("\n");
            printf("Client sided commands: lpwd, ldir, lcd\n");
            printf("Server sided commands: pwd, dir, cd, get, put\n");
        }
    }
    exit(0);
}