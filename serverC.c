#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	int sockfd, origin_length, numbytes;
	struct sockaddr_in server;
	struct sockaddr_in origin;
	char buffer[1024];
	char func_name[100];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(23955);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, &server, sizeof(server)) == -1){
		perror("bind");
		exit(1);
	}
	printf("The Server C is up and running using UDP on port 23955.\n");

	while(1){
		numbytes = recvfrom(sockfd, buffer, 1024, 0, &origin, &origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		int num = strlen(buffer);
		printf("size of received for C: %d\n", num);
		func_name[0] = buffer[num - 3];
		func_name[1] = buffer[num - 2];
		func_name[2] = buffer[num - 1];

		int min = strcmp(func_name,"min");
		int max = strcmp(func_name,"max");
		int sum = strcmp(func_name,"sum");
		int sos = strcmp(func_name,"sos");
		
		if (min == 0){
			printf("min\n");
		}
		else if (max == 0){
			printf("max\n");
		}
		else if (sum == 0){
			printf("sum\n");
		}
		else if (sos == 0){
			printf("sos\n");
		}
		

	}
}