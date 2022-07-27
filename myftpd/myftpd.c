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
#include  <fcntl.h>
/*
#include <dirent.h>

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
    fprintf(logfile,"\n");
    fflush(logfile);
}

void serve_a_client(int sd)
{   int nr,nw; 
    char buf[FILE_BLOCK_SIZE];

    while (1)
    {
        /* read data from client */
        if ((nr = read(sd, buf, sizeof(buf))) <= 0)
        {
            exit(0);   /* connection broken down */
        }
            
        /* ================= process data =================== */
        buf[nr] = '\0';
        //reverse(buf);
              
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
    int sd,nsd; //what is the purpose of n?
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

    /* setting path for logfile */
    getcwd(current_directory,sizeof(current_directory));
    strcpy(logfile,current_directory);
    strcat(logfile,LOGPATH); //e.g. "./myftpd.log" , "./home/myftpd.log"

    /*announces whenever program is started up, prevents confusion across clients*/
    logger(logfile,"\n======Welcome to myftpd======\n"); 
    

    /* turn the program into a daemon */
    daemon_init(); 

    logger(logfile,"Server initialized");
    
    //logger(logfile,"initial directory set to %s", current_directory);

    /* set up listening socket sd */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("server: socket error");
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
        perror("server: bind failure"); 
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
            if (errno == EINTR) /* if interrupted by SIGCHLD */
            {
                continue;
            }   
            perror("server: accept error"); 
            exit(1);
        }
        logger(logfile,"Now listening to a client");

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
        logger(logfile,"Child has finished serving the client");
        exit(0);
    }
}