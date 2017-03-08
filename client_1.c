#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <mysql/mysql.h>
#include <openssl/sha.h>
#include <gcrypt.h>
#define LINE_BUFSIZE 2096
#define SHA_DIGEST_LENGTH 20

int data_read,data_write;
int main(int argc, char *argv[])
{

/*------------------declare variables-------------------------*/

int sockfd, portnum;
int i;
char buffer[2096];
char command[1024],command2[1024],dummy[255];
char password[1024];
unsigned char temp[SHA_DIGEST_LENGTH];
    char buf[SHA_DIGEST_LENGTH*2];
char line[LINE_BUFSIZE];
    FILE *pipe;
FILE *iterative;
FILE *iterativec;
struct sockaddr_in6 serv_addr;
struct hostent *server;
if (argc<3)
	{
	printf("port or hostname not provided\n");
	}

/*-------------------create socket----------------------------*/

sockfd = socket(AF_INET6, SOCK_STREAM, 0);
if (sockfd<0)
	{
	printf("cant open a socket.\n");
	exit(1);
	}

/*-----------set the address in the sockaddr_in--------------*/
                             
bzero((char*)&serv_addr, sizeof(serv_addr));
portnum = atoi(argv[2]);
server = gethostbyname2 (argv[1],AF_INET6);
if (portnum==0)
	{
	printf("no such host.\n");
	exit(1);
	}

serv_addr.sin6_flowinfo= 0;
serv_addr.sin6_family= AF_INET6;

bcopy((char*)server->h_addr, (char*)&serv_addr.sin6_addr.s6_addr, server->h_length);
serv_addr.sin6_port = htons(portnum);

/*-------------------connect--------------------------------*/

int len= sizeof(serv_addr);
if(connect(sockfd,(struct sockaddr *) &serv_addr,len)<0)
	{
	printf("can't connect to socket.\n");
	exit(1);
	}

/*------------------Read and Write------------------------*/
	
for(;;)
{
while(1)
{

	bzero(buffer, 2096);
	if(read(sockfd,buffer,2096)<0)     	// Receive the data that is sent by the server.
	{
	printf("Read unsuccessfull\n");		// Error message when the read cannot be done.
	}
printf("%s:",buffer); 	

if((strcmp(buffer,"Password"))==0)
{
  
 goto LOOP;
}	

	bzero(buffer, 2096);// Clearing the buffer

	scanf("%s",&buffer);			// Getting user input						
		if((strcmp(buffer,"exit"))==0)
{
 exit(1);
}		
	if(send(sockfd, buffer, sizeof(buffer), 0)<0)  	// Send back the reply to the server.
	{
		printf("Send unsuccessfull\n");		// Error message when send cannot be done.
	}


}
//---------------------------------------------------------------------------------
LOOP:   
    pipe = popen("./my_script", "r");       // Bash Script to mask the password
    if (pipe == NULL)
 	{  
       exit(1); 
       
    }

    while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
        printf("%s",line);
          }
iterative = fopen("data.txt","r+");
fscanf(iterative,"%s",password);


    memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
    memset(temp, 0x0, SHA_DIGEST_LENGTH);
 
    SHA1(password, strlen(password), temp);    //Function to hash the password
 
    for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", temp[i]);
    }
 iterativec=fopen("data.txt","w+");
fprintf(iterativec,"%s",buf);
    printf("\n\nSHA of %s is %s\n", password, buf);
bzero(buffer,2096);

/* Sending data back to client */
strcpy(buffer,buf);
if(send(sockfd, buffer, sizeof(buffer), 0)<0) 
	{
		printf("Send unsuccessfull\n");		
	}
       pclose(pipe);
printf("\n");
//---------------------------------------------------------------------------------
}					// while end
return 0;
}
