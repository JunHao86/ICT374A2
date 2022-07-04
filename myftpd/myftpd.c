/*	Filename: myftpd.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): 04/07/22
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

/*
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
*/

//================================================================

#define FILE_BLOCK_SIZE 512  
#define SERV_TCP_PORT 40004 //Listening TCP Port Number
#define LOGPATH "/myftpd.log" //Logfile for daemon process

//================================================================

/*
 * To do list
 * 1. Write functions
 * 2. Uncheck any headers if necessary 
 * 3. Get supposed FILE_BLOCK_SIZE
 */

//================================================================

void claim_children()
{
    pid_t pid=1;
     
    while (pid>0) { /* claim as many zombies as we can */
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
    
    /* child continues */
    setsid();                      /* become session leader */
    chdir("/");                    /* change working directory */
    umask(0);                      /* clear file mode creation mask */

    /* catch SIGCHLD to remove zombies from system */
    act.sa_handler = claim_children; /* use reliable signal */
    sigemptyset(&act.sa_mask);       /* not to block other signals */
    act.sa_flags   = SA_NOCLDSTOP;   /* not catch stopped children */
    sigaction(SIGCHLD,(struct sigaction *)&act,(struct sigaction *)0);
}

void serve_a_client(int sd)
{   int nr, nw;
    char buf[FILE_BLOCK_SIZE];

    while (1){
         /* read data from client */
         if ((nr = read(sd, buf, sizeof(buf))) <= 0) 
             exit(0);   /* connection broken down */

         /* process data */
         //buf[nr] = '\0'; reverse(buf);
              
         /* send results to client */
         nw = write(sd, buf, nr);
    } 
}

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
    int nsd;
    struct sockaddr_in ser_addr, cli_addr;

    /* turn the program into a daemon */
    daemon_init(); 





    exit(0);
}