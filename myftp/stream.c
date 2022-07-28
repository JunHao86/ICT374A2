/*	Filename: stream.h
 *	Author: Emmanuel Lim, Daniel Khoo
 *	Date (of finalized update): TBD
 *  Description: File used for stream reading and stream writing
 */

#include  <unistd.h>
#include  <sys/types.h>
#include  <netinet/in.h> /* struct sockaddr_in, htons(), htonl(), */
#include  "stream.h"

//================================================================

int readn(int sd, char *buf, int bytesize)
{
    int n,nread = 0;

    if (bytesize < MAX_BLOCK_SIZE)
    {
        return (-3);     //buffer too small
    }

    for(n=0;n<bytesize;n+=nread)
    {
        if((nread=read(sd,buf+n,bytesize-n)) < 0)
        {
            return(nread); //error in reading
        }
    }
    return n;
}

int writen(int sd, char *buf, int bytesize)
{
    int n,nwrite = 0;

    if (bytesize > MAX_BLOCK_SIZE)
    { 
        return (-3);    // too many bytes to send in one go
    }

    for(n=0;n<bytesize;n+=nwrite)
    {
        if((nwrite = write(sd,buf+n,bytesize-n)) < 0)
        {
            return(nwrite); //error in writing
        }
    }
    return n;
}

int read_onebyte_length(int sd, char *opcode)
{
    char c;
    
    if (read(sd,(char *)&opcode, 1) != 1)
    {
        return (-1);
    }

    *opcode = c;
    return 1;
}

int write_onebyte_length(int sd,char opcode)
{
    if (write(sd,(char *)&opcode, 1) != 1)
    {
        return (-1);
    }
    
    return 1;
}

int read_twobyte_length(int sd, int *length)
{
	short data = 0; //short = 2 bytes long

    if (read(sd, &data, 2) != 2)
    {
        return (-1);
    }

    //receiving end, processes into host byte order 
    int hbo = (int)ntohs(data);
    *length = hbo;

	return 1;
}

int write_twobyte_length(int sd, int length)
{
	short data = length; //short = 2 bytes long
    //sending end, processes into network byte order
	data = htons(data);

    if(length > MAX_BLOCK_SIZE)
    {
        return (-2); //too many bytes to send in a single sequence
    }

	if (write(sd,&data, 2) != 2) 
    {
        return (-1);
    }
    
	return 1;
}

int read_fourbyte_length(int sd, int *length)
{
	int data = 0; //int = 4 bytes long

    if (read(sd, &data, 4) != 4) 
    {
        return (-1);
    }
    
    //receiving end, processes into host byte order 
    int hbo = ntohl(data); 
    *length = hbo;

	return 1;
}

int write_fourbyte_length(int sd, int length)
{
	//sending end, processes into network byte order
    int data = htonl(length); 

    if(length > MAX_BLOCK_SIZE)
    {
        return (-2); //too many bytes to send in a single sequence
    }

	if (write(sd,&data, 4) != 4)
    {
        return (-1);
    } 

	return 1;
}