#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *);

int main()
{
	int sd, new_sd, c, *new_sock;
	struct sockaddr_in server, client;
	
	sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
		printf("Couldn't create a socket");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("Bind failed");
		return 1;
	}
	puts("Bind done");

	listen(sd, 3);
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	while(new_sd = accept(sd, (struct sockaddr *)&client, (socklen_t*)&c))
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		
		new_sock = malloc(1);
		*new_sock = new_sd;

		if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
		{
			perror("Couldnt create thread");
			return 1;
		}

		puts("Handler assigned");
	}

	if(new_sd < 0)
	{
		perror("Accept failed");
		return 1;
	}

	return 0;
}


void *connection_handler(void *sd)
{
	int sock = *(int*)sd;
	int recev;
	char *msg, client_msg[2000];

	while((recev = recv(sock, client_msg, 2000, 0)) > 0)
	{
		write(sock, client_msg, strlen(client_msg));
	}

	if(recev == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	
	else if(recev == -1)
	{
		perror("Receive failed");
	}

	free(sd);

	return 0;
}
