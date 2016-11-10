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

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(21955);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, &server, sizeof(server)) == -1){
		perror("bind");
		exit(1);
	}
	printf("The Server A is up and running using UDP on port 21955.\n");

	while(1){
		numbytes = recvfrom(sockfd, buffer, 1024, 0, &origin, &origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		printf("size of received for A: %d\n", strlen(buffer));
	}
}