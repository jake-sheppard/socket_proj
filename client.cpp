#define _GNU_SOURCE
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
#include <fstream>
#include <string>


int main(int argc, char *argv[]){

	
	if (argc != 2){
		printf("Correct command line entry: ./client <function_name> \n");
		exit(1);
	}
	printf("The client is up and running.\n");
	
	struct addrinfo hints, *res;
	int sockfd;

	// first, load up address structs with getaddrinfo():
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo("localhost", "25955", &hints, &res) != 0){
		printf("error \n");
    	exit(1);
	}
	// make a socket:
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	// connect it to the address and port we passed in to getaddrinfo():
	connect(sockfd, res->ai_addr, res->ai_addrlen);

	// send function name to AWS
	char msg[100];
	strcpy(msg, argv[1]);
	int len = strlen(msg);
	if (send(sockfd, msg, len, 0) == -1){
		perror("send");
		exit(1);
	} 
	printf("The client has sent the reduction type %s to AWS.\n", msg);

	


	// need to read in nums file and store, delimiter is a comma

	

	/*FILE *fp;
	int file_size;


	fp = fopen ( "nums.csv" , "rb" );
	if( !fp ){
		perror("nums.csv");
		exit(1);
	}
	
	fseek( fp , 0L , SEEK_END);
	file_size = ftell( fp );
	rewind( fp );

	char* line = NULL;
	size_t length = 0;
	size_t read;
	*/

	std::ifstream infile("nums.csv");
	std::string line;

	int buffer[1600];
	int i = 0;

	while (std::getline(infile,line)) {
        buffer[i] = std::atoi(line.c_str());
        i++;
    }



    // sending how many ints
    if (send(sockfd, (void*)&i, sizeof(int), 0) == -1){
		perror("send");
		exit(1);
	} 

  	if (send(sockfd, (void*)buffer, i*sizeof(int), 0) == -1){
		perror("send");
		exit(1);
	} 
	printf("The client has sent %d numbers to AWS.\n", i);

	//fclose(fp);

	int result[1];
	recv(sockfd, (void*)&result, sizeof(int), 0);
	printf("The client has received reduction %s: %d\n",msg, result[0]);
	close(sockfd);

}