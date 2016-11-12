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
	server.sin_port = htons(22955);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, &server, sizeof(server)) == -1){
		perror("bind");
		exit(1);
	}
	printf("The Server B is up and running using UDP on port 22955.\n");

	while(1){
		// receiving function name
		numbytes = recvfrom(sockfd, func_name, 3*sizeof(char), 0, &origin, &origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
	
		// receiving number of integers
		int nums[1];
		numbytes = recvfrom(sockfd, nums, sizeof(int), 0, &origin, &origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}

		// receiving integers
		int cnt = nums[0];
		numbytes = recvfrom(sockfd, buffer, cnt * sizeof(int), 0, &origin, &origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		
		func_name[3] = '\0';
		int min = strcmp(func_name,"min");
		int max = strcmp(func_name,"max");
		int sum = strcmp(func_name,"sum");
		int sos = strcmp(func_name,"sos");
		
		int* int_array = (int*) buffer;
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