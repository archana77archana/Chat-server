#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int sd;
	struct sockaddr_in server;
	char msg[1000], server_reply[2000];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		printf("Couldn't create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	if(connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Connect failed.");
		return 1;
	}

	puts("Connected\n");

	while(1)
	{
		printf("Enter message: ");
		fgets(msg, sizeof(msg), stdin);

		if(send(sd, msg, strlen(msg), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}

		if(recv(sd, server_reply, 2000, 0) < 0)
		{
			puts("Receive failed");
			break;
		}

		printf("Server reply: %s\n", server_reply);
		server_reply[0] = '\0';
	}

	//close(sd);
	return 0;
}
