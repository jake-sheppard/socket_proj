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
		int result = 0;
		if (min == 0){
			result = int_array[0];
			for (int i = 1; i < cnt; i++){
				if (int_array[i] < result){
					result = int_array[i];
				}
			}
			printf("min = %d\n", result);
		}
		else if (max == 0){
			result = int_array[0];
			for (int i = 1; i < cnt; i++){
				if (int_array[i] > result){
					result = int_array[i];
				}
			}
			printf("max = %d\n", result);
		}
		else if (sum == 0){
			for (int i = 0; i < cnt; i++){
				result += int_array[i];
			}
			printf("sum = %d\n", result);
		}
		else if (sos == 0){
			for (int i = 0; i < cnt; i++){
				result += int_array[i] * int_array[i];
			}
			printf("sos = %d\n", result);
		}


	}
}