#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
/* You will have to modify the program below */

#define MAXBUFSIZE 100

int main(int argc, char * argv[])
{
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	int sock;                           //This will be our socket
	struct sockaddr_in sin, remote;     //"Internet socket address structure"
	unsigned int remote_length;         //length of the sockaddr_in structure
	int nbytes;                        //number of bytes we receive in our message
	char buffer[MAXBUFSIZE + 1];             //a buffer to store our received message
	char writeBuf[MAXBUFSIZE];             //buffer to store data to write
	if (argc != 2)
	{
		printf("USAGE:  <port>\n");
		exit(1);
	}
	bzero(buffer, sizeof(buffer));
	bzero(writeBuf, sizeof(writeBuf));
	/******************
	This code populates the sockaddr_in struct with
	the information about our socket
	******************/
	bzero(&sin, sizeof(sin));                    //zero the struct
	sin.sin_family = AF_INET;                   //address family
	sin.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
	sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine


												//Causes the system to create a generic socket of type UDP (datagram)
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("unable to create socket");
	}

	/******************
	Once we've created a socket, we must bind that socket to the
	local address and port we've supplied in the sockaddr_in struct
	******************/
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		printf("unable to bind socket\n");
	}

	remote_length = sizeof(remote);

	//waits for an incoming message
	bzero(buffer, sizeof(buffer));
	while (1)
	{
		nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote, &remote_length);
		if (nbytes < 0)
		{
			printf("unable to receive socket\n");
		}
		printf("The client says %s\n", buffer);
		char msg[] = "ACK";
		if ((sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
		{
			printf("unable to send socket");
		}
		if (nbytes > 4 && strncmp(buffer, "put ", 4) == 0)
		{
			FILE* fp;
			char file[MAXBUFSIZE + 1];
			strncpy(file, buffer + 4, nbytes - 5);
			fp = fopen(file, "w");
			int bytes_write;
			while (1)
			{
				bzero(buffer, sizeof(buffer));
				bzero(writeBuf, sizeof(writeBuf));
				nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote, &remote_length);
				if (nbytes< 0)
				{
					printf("unable to receive socket\n");
				}
				strncpy(writeBuf, buffer + 1, nbytes-1);
				fwrite(writeBuf, sizeof(writeBuf[0]), nbytes - 1, (FILE*)fp);
				if (buffer[0] == 0)
				{
					break;
				}
			}
			fclose(fp);
		}
	}
	close(sock);
}
