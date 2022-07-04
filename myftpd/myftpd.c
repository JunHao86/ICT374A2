/*	Filename: myftpd.c
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): TBD
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

#define BUFSIZE         256
#define FILE_BLOCK_SIZE 512  
#define SERV_TCP_PORT 40004 //Listening TCP Port Number
#define LOGPATH "/myftpd.log" //Logfile for daemon process

//================================================================

/*
 * Done
 * 1. Function - claim_children()
 * 2. Function - daemon_init()
 * 3. File directory setting
 * 4. Standard server stuff - adapted from ser6.c
 */

/*
 * To do list
 * 1. Functions - serve_a_client()
 * 2. Write any other functions if required
 * 3. Uncheck any headers if necessary 
 * 4. Get supposed FILE_BLOCK_SIZE
 * 5. Define LOGGER and determine what would be written into the filelog
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

         /* ================= process data =================== */
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
    int sd,nsd,n; //what is the purpose of n?
    socklen_t cli_addrlen;
    struct sockaddr_in ser_addr, cli_addr;
    unsigned short port = SERV_TCP_PORT;

    char initial_directory[BUFSIZE] = ".";
    char current_directory[BUFSIZE];
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

    /* turn the program into a daemon */
    daemon_init(); 

    /* set up listening socket sd */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("server:socket");
        exit(1);
    } 

    /* build server Internet socket address */
    bzero((char *)&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    /* bind server address to socket sd */
    if (bind(sd, (struct sockaddr *) &ser_addr, sizeof(ser_addr))<0)
    {
        perror("server bind"); 
        exit(1);
    }

    /* become a listening socket */
    listen(sd, 5);

    while (1) {

        /* wait to accept a client request for connection */
        cli_addrlen = sizeof(cli_addr);
        nsd = accept(sd, (struct sockaddr *) &cli_addr, (socklen_t *)&cli_addrlen);
        if (nsd < 0) 
        {
            if (errno == EINTR)   /* if interrupted by SIGCHLD */
                continue;
            perror("server:accept"); 
            exit(1);
        }

        /* create a child process to handle this client */
        if ((pid=fork()) <0) 
        {
            perror("fork"); 
            exit(1);
        } 
        else if (pid > 0) 
        { 
            close(nsd);
            continue; /* parent to wait for next client */
        }

        /* now in child, serve the current client */
        close(sd); 
        serve_a_client(nsd);
        exit(0);
    }
}