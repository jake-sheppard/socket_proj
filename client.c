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

	

	FILE *fp;
	long size;


	fp = fopen ( "nums.csv" , "rb" );
	if( !fp ){
		perror("nums.csv");
		exit(1);
	}
	
	fseek( fp , 0L , SEEK_END);
	size = ftell( fp );
	rewind( fp );


	int buffer[size];
	
	if( 1!=fread( buffer , size, 1 , fp) )
  	fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

  	int last = size/sizeof(int);
  	buffer[last] = '\0';
	
	/* do your work here, buffer is a string contains the whole text */
  	if (send(sockfd, (void*)buffer, size, 0) == -1){
		perror("send");
		exit(1);
	} 
	printf("The client has sent %d numbers to AWS.\n", size/sizeof(int));


	fclose(fp);
	//free(buffer);


}