#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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
#include <errno.h>
#include <string.h>

#define MAXBUFSIZE 20000

/* You will have to modify the program below */

int main(int argc, char * argv[])
{
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	int nbytes;                             // number of bytes send by sendto()
	int sock;                               //this will be our socket
	char buffer[MAXBUFSIZE + 1];
	char buf[MAXBUFSIZE];

	struct sockaddr_in remote;              //"Internet socket address structure"

	if (argc < 3)
	{
		printf("USAGE:  <server_ip> <server_port>\n");
		exit(1);
	}
	/******************
	Here we populate a sockaddr_in struct with
	information regarding where we'd like to send our packet
	i.e the Server.
	******************/
	bzero(&remote, sizeof(remote));               //zero the struct
	remote.sin_family = AF_INET;                 //address family
	remote.sin_port = htons(atoi(argv[2]));      //sets port to network byte order
	remote.sin_addr.s_addr = inet_addr(argv[1]); //sets remote IP address


												 //Causes the system to create a generic socket of type UDP (datagram)
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("unable to create socket");
	}

	//set delay for socket receiving
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
		printf("unable to set socket");
	}
	/******************
	sendto() sends immediately.
	it will report an error if the message fails to leave the computer
	however, with UDP, there is no error if the message is lost in the network once it leaves the computer.
	******************/
	struct sockaddr_in from_addr;
	int addr_length = sizeof(struct sockaddr);
	bzero(buffer, sizeof(buffer));
	while (1)
	{
		int bytes_command;
		nbytes = MAXBUFSIZE;
		char *command;
		char sendBuf[MAXBUFSIZE];
		puts("Please enter a command.");

		command = (char*)malloc(nbytes + 1);
		bytes_command = getline(&command, &nbytes, stdin);

		while (1)
		{
			if ((nbytes = sendto(sock, command, bytes_command, 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
			{
				printf("unable to send command");
			}
			if (nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&from_addr, &addr_length) > 0)
			{
				break;
			}
		}
		if (bytes_command > 4 && strncmp(command, "put ", 4) == 0)
		{
			FILE *fp;
			char file[MAXBUFSIZE];
			strncpy(file, command + 4, bytes_command - 5);
			fp = fopen(file, "r");
			if (fp == NULL)
			{
				puts("file do not exits");
			}
			int bytes_read = fread(sendBuf, sizeof(sendBuf[0]), MAXBUFSIZE, (FILE*)fp);
			while (bytes_read>0)
			{
				buffer[0] = 1;
				strncpy(buffer + 1, sendBuf, MAXBUFSIZE);
				while (1)
				{
					if ((nbytes = sendto(sock, buffer, bytes_read + 1, 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
						printf("unable to send file");
					if (nbytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&from_addr, &addr_length) > 0)
					{
						break;
					}
				}
				bzero(buffer, sizeof(buffer));
				bzero(sendBuf, sizeof(sendBuf));
				bytes_read = fread(sendBuf, sizeof(sendBuf[0]), MAXBUFSIZE, (FILE*)fp);
			}
			fclose(fp);
			while (1)
			{
				bzero(buffer, sizeof(buffer));
				if ((nbytes = sendto(sock, buffer,1, 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
					printf("unable to send file");
				if (nbytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&from_addr, &addr_length) > 0)
				{
					break;
				}
			}
		}
		if (bytes_command > 2 && strncmp(command, "ls", 2) == 0)
		{
			bzero(buffer,sizeof(buffer));
			bzero(buf,sizeof(buf));
			
			nbytes = recvfrom(sock,buffer,sizeof(buffer),0,(struct sockaddr*)&from_addr, &addr_length);
			if(nbytes<0)
			{
				puts("unable to receive files list");
			}
			else
			{
				char msg[] = "ACK";
				if ((nbytes = sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
					printf("unable to send file");
				if (buffer[0] == 0)
					break;
				puts(buffer);
			}
			
		}
		
		// Blocks till bytes are received
	}


	close(sock);

}
