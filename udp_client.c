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

#define MAXBUFSIZE 100

/* You will have to modify the program below */

int main(int argc, char * argv[])
{

	int nbytes;                             // number of bytes send by sendto()
	int sock;                               //this will be our socket
	char buffer[MAXBUFSIZE];

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

	/******************
	sendto() sends immediately.
	it will report an error if the message fails to leave the computer
	however, with UDP, there is no error if the message is lost in the network once it leaves the computer.
	******************/
	while (1)
	{
		int bytes_command;
		nbytes = MAXBUFSIZE;
		char *command;
		puts("Please enter a command.");

		command = (char*)malloc(nbytes + 1);
		bytes_command = getline(&command, &nbytes, stdin);

		if (bytes_command > 4 && strncmp(command, "put ", 4) == 0)
		{
			
			FILE *fp;
			FILE *fp1;
			char file[MAXBUFSIZE+1];
			char buffer[MAXBUFSIZE+1];
			strncpy(file, command + 4, bytes_command - 4);
			fp = fopen(file,"r");
			if(fp==NULL)
			{
				puts("file do not exits");
				puts("file");
			}
			fgets(buffer, MAXBUFSIZE, (FILE*)fp);
			puts(buffer);
			fp1 = fopen("copy.txt", "w");
			fputs(buffer, fp1);
			fclose(fp1);
			fclose(fp);
			char a = getchar();

		}


		if ((nbytes = sendto(sock, command, bytes_command, 0, (struct sockaddr*)&remote, sizeof(remote))) < 0)
		{
			printf("unable to send socket");
		}
		// Blocks till bytes are received
		struct sockaddr_in from_addr;
		int addr_length = sizeof(struct sockaddr);
		bzero(buffer, sizeof(buffer));
		if (nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&from_addr, &addr_length) < 0)
		{
			printf("unable to receive socket");
		}

		printf("Server says %s\n", buffer);

	}


	close(sock);

}
