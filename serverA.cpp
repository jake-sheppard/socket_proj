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
#include <strings.h>

int main(int argc, char *argv[]){

	int sockfd, origin_length, numbytes, len1, len2;
	struct sockaddr_in server;
	struct sockaddr_in origin, temp1, temp2, aws;
	int buffer[1600];
	char func_name[100];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(21955);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (sockaddr*)&server, sizeof(server)) == -1){
		perror("bind");
		exit(1);
	}
	printf("The Server A is up and running using UDP on port 21955.\n");

	while(1){
		memset(&origin, 0, sizeof(origin));
		// receiving function name
		numbytes = recvfrom(sockfd, func_name, 3*sizeof(char), 0, (sockaddr*)&origin, (socklen_t*)&origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
	
		// receiving number of integers
		memset(&temp1, 0, sizeof(temp1));
		int nums[1];
		numbytes = recvfrom(sockfd, nums, sizeof(int), 0, (sockaddr*)&temp1, (socklen_t*)&len1);
		if (numbytes == -1){
			perror("recvfrom");
		}
		printf("The Server A has received %d numbers\n", nums[0]);

		// receiving integers
		memset(&temp2, 0, sizeof(temp2));
		int cnt = nums[0];
		numbytes = recvfrom(sockfd, buffer, cnt * sizeof(int), 0, (sockaddr*)&temp2, (socklen_t*)&len2);
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
		int i;
		if (min == 0){
			result = int_array[0];
			i = 1;
			for (; i < cnt; i++){
				if (int_array[i] < result){
					result = int_array[i];
				}
			}
		}
		else if (max == 0){
			result = int_array[0];
			i = 1;
			for (; i < cnt; i++){
				if (int_array[i] > result){
					result = int_array[i];
				}
			}
		}
		else if (sum == 0){
			i = 0;
			for (; i < cnt; i++){
				result += int_array[i];
			}
		}
		else if (sos == 0){
			i = 0;
			for (; i < cnt; i++){
				result += int_array[i] * int_array[i];
			}
		}
		printf("The Server A has successfully finished the reduction %s: %d\n",func_name,result );

		bzero(&aws, sizeof(aws));
		aws.sin_family = AF_INET;
		aws.sin_port = htons(24955);
		aws.sin_addr.s_addr = htonl(INADDR_ANY);

 		if ((numbytes = sendto(sockfd, (void*)&result, sizeof(int), 0,(struct sockaddr*)&aws, sizeof(aws)) == -1)){
 			perror("sendto");
 			exit(1);
 		}
 		printf("The Server A has successfully finished sending the reduction value to AWS server. \n");
 		printf("\n");
	}
}