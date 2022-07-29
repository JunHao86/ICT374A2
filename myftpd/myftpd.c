/*	Filename: myftpd.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): 30/7/22
 *  Description: Assignment 2 - Simple File Transfer Protocol - Server Program
 */

#include  <netinet/in.h>       /* struct sockaddr_in, htons, htonl */
#include  <errno.h>            /* extern int errno, EINTR, perror() */
#include  <sys/types.h>        /* pid_t, u_long, u_short */
#include  <sys/socket.h>       /* struct sockaddr, socket(), etc */
#include  <signal.h>           /* SIGCHLD, sigaction() */
#include  <sys/wait.h>         /* waitpid(), WNOHAND */

#include  <stdio.h>   
#include  <stdlib.h>    
#include  <string.h>
#include  <unistd.h> 
#include  <sys/stat.h>
#include  <fcntl.h>
#include "stream.h"
#include <dirent.h>

/*
#include <time.h>
#include <stdarg.h>
#include <signal.h>
*/

//================================================================

#define BUFSIZE         256
#define FILE_BLOCK_SIZE 512  
#define SERV_TCP_PORT 40228 // Assigned Listening TCP Port Number
#define LOGPATH "/myftpd.log" //Logfile for daemon process

//================================================================

/*
 * Done
 * 1. Function - claim_children()
 * 2. Function - daemon_init()
 * 3. File directory setting
 * 4. Standard server stuff - adapted from ser6.c
 * 5. Function - logger()
 */

/*
 * To do list
 * 1. Functions - serve_a_client()
 * 2. Write any other functions if required
 * 3. Uncheck any headers if necessary 
 */

//================================================================

void claim_children()
{
    pid_t pid=1;
     
    while (pid>0) { 
        pid = waitpid(0, (int *)0, WNOHANG); 
    } 
}

void daemon_init(void)
{       
    pid_t   pid;
    struct sigaction act;
    if ( (pid = fork()) < 0) 
    {
        perror("fork"); 
        exit(1); 
    } 
    else if (pid > 0) //parent process
    {
        printf("myftpd IPID: %d\n",pid); //for verification
        exit(0);                   
    }
    
    setsid();                      
    chdir("/");                    
    umask(0);                      

    
    act.sa_handler = claim_children; 
    sigemptyset(&act.sa_mask);       
    act.sa_flags   = SA_NOCLDSTOP;   
    sigaction(SIGCHLD,(struct sigaction *)&act,(struct sigaction *)0);
}

void logger(char* log, char* description)
{
    FILE *logfile;
    logfile = fopen(log,"a+");
    if(!logfile)
    {
        fprintf(stderr, "Unable to create or write to log file %s\n",log);
        perror ("unable to create or write to log file\n");
        exit(1);
    }
    fprintf(logfile,"%s",description);
    fflush(logfile);
}

//================================================================

void serve_pwd(char* log,int sd)
{
    logger(log,"opcode for PWD function received\n");

    char working_directory[FILE_BLOCK_SIZE];
    //gets "/" only, child process from shell
    //how to get actual cwd?
    getcwd(working_directory,sizeof(working_directory)); 
    logger(log,working_directory);
    logger(log,"\n");

    //Server > Client
    if(write_onebyte_length(sd,'A') == -1)
    {
        logger(log,"opcode write failed\n");
        return;
    }

    if(write_twobyte_length(sd,strlen(working_directory)) == -1)
    {
        logger(log,"length write failed\n");
        return;
    }

    if(writen(sd,working_directory,strlen(working_directory))== -1)
    {
        logger(log,"failed to write directory\n");
        return;
    }

    logger(log,"PWD function successful\n");
}

void serve_dir(char* log,int sd)
{
    logger(log,"opcode for DIR function received\n");
    
    DIR *dir;
	struct dirent *directory;
    char working_directory[FILE_BLOCK_SIZE];

    if ((dir = opendir("."))!= NULL)
    {
        while((directory = readdir(dir))!=NULL)
        {
            strcat(working_directory,directory->d_name);
            strcat(working_directory,"\n");
        }
        //end of while loop
        working_directory[strlen(working_directory)-1] = '\0';
        closedir(dir);
        logger(log,"closedir successful\n");
    }
    else
    {
        logger(log,"DIR function dirent error");
    }

    if(write_onebyte_length(sd,'B') == -1)
    {
        logger(log,"opcode write failed\n");
        return;
    }

    if(write_fourbyte_length(sd,strlen(working_directory)) == -1)
    {
        logger(log,"length write failed\n");
        return;
    }

    if(writen(sd,working_directory,strlen(working_directory))== -1)
    {
        logger(log,"failed to write list of files in cwd\n");
        return;
    }

    logger(log,"DIR function successful\n");
}

void serve_cd(char* log,int sd)
{
    logger(log,"opcode for CD function received\n");

    int token_size; 
    char opcode2; //second opcode to determine successful directory change
    

    //Client > Server
    if(read_twobyte_length(sd,&token_size) == -1)
    {
        logger(log,"token length read failed\n");
        return;
    }

    char token[token_size+1];

    if(readn(sd,token,token_size) == -1)
    {
        logger(log,"token directory path read failed\n");
        return;
    }
    
    token[token_size] = '\0';

    //Directory change
    if(chdir(token) == 0)
    {
        opcode2 = '0';
        logger(log,"directory changed\n");
    }
    else
    {
        opcode2 = '1';
        logger(log,"failed to find and change directory\n");
    }

    //Server > Client
    if(write_onebyte_length(sd,'C') == -1)
    {
        logger(log,"opcode write failed\n");
        return;
    }

    if(write_onebyte_length(sd,opcode2) == -1)
    {
        logger(log,"opcode2 write failed\n");
        return;
    } 

    logger(log,"CD function successful\n");
}


void serve_a_client(char* log, int sd)
{   
    char opcode;
    logger(log,"connected to client\n");

    //Client > Server
	while (read_onebyte_length(sd,&opcode) > 0){

		switch(opcode){
			case 'A':
                serve_pwd(log,sd);
			    break;
			case 'B':
                serve_dir(log,sd);
                break;
			case 'C':
                serve_cd(log,sd);
                break;
			case 'D':
                logger(log,"dir cmd received\n");
                //function here
                break;
			case 'E':
                logger(log,"cd cmd received\n");
                //function here
                break;
			default:
				logger(log,"invalid opcode recieved\n");
			    break;
		}
	}
	logger(log,"disconnected\n");
	return;
}

//================================================================

/*
No. of args - Description
argc == 1   - Uses current directory
            - ./myftpd
argc == 2   - User specifies initial current directory       
            - ./myftpd <initial_current_directory>
*/
int main(int argc, char *argv[])
{
    pid_t pid;
    int sd,nsd;
    socklen_t cli_addrlen;
    struct sockaddr_in ser_addr, cli_addr;
    unsigned short port = SERV_TCP_PORT;

    char initial_directory[BUFSIZE] = ".";
    char current_directory[BUFSIZE];
    char logfile[256];

    
    //get current directory
    getcwd(current_directory,sizeof(current_directory));
    
    if (argc==2) //use the user-specified directory
    {
        strcpy(initial_directory,argv[1]);
    }
    else if (argc>2)
    {
        printf("Usage: %s [<initial_current_directory>]\n", argv[0]); 
        exit(1); 
    }

    if (chdir(initial_directory)==-1) //changes to user-specified directory
    {
        perror("initial directory set failed\n");
        exit(1);
    }

    //setting path for logfile
    getcwd(current_directory,sizeof(current_directory));
    strcpy(logfile,current_directory);
    strcat(logfile,LOGPATH); //e.g. "./myftpd.log" , "./home/myftpd.log"

    //announces whenever program is started up, prevents confusion across clients*/
    logger(logfile,"\n======Welcome to myftpd======\n"); 
    

    //turn the program into a daemon
    daemon_init(); 

    logger(logfile,"Server initialized\n");
    
    //logger(logfile,"initial directory set to %s", current_directory);
    //unusable as logger is now changed to accept a predefined string

    //set up listening socket sd
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("server: socket error");
        exit(1);
    } 

    //build server Internet socket address
    bzero((char *)&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    //bind server address to socket sd
    if (bind(sd, (struct sockaddr *) &ser_addr, sizeof(ser_addr))<0)
    {
        perror("server: bind failure"); 
        exit(1);
    }

    //become a listening socket
    listen(sd, 5);
    

    while (1) {

        //wait to accept a client request for connection
        cli_addrlen = sizeof(cli_addr);
        nsd = accept(sd, (struct sockaddr *) &cli_addr, (socklen_t *)&cli_addrlen);
        if (nsd < 0) 
        {
            if (errno == EINTR) //if interrupted by SIGCHLD
            {
                continue;
            }   
            perror("server: accept error"); 
            exit(1);
        }
        logger(logfile,"Now listening to a client\n");

        //create a child process to handle this client
        if ((pid=fork()) <0) 
        {
            perror("fork"); 
            exit(1);
        } 
        else if (pid > 0) 
        { 
            close(nsd);
            continue; //parent to wait for next client
        }

        //now in child, serve the current client
        close(sd); 
        sd = nsd;
        serve_a_client(logfile,nsd);
        logger(logfile,"Child has finished serving the client\n");
        exit(0);
    }
}