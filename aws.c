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
	printf("The AWS is up and running.\n");
	// SOCKET FOR TCP
	int sock_tcp, new_fd, addr_size;
	struct sockaddr_in tcp_addr;
	struct sockaddr_storage inc_addr;
	char func_name[100];
	int buff_data[1500];
	


	// create socket
	sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_tcp == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&tcp_addr, sizeof(tcp_addr));
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_port = htons(25955);
	tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// bind socket
	if ((bind(sock_tcp, (struct sockaddr *)&tcp_addr, sizeof(struct sockaddr_in))) == -1){
		perror("bind");
		exit(1);
	}
	// listen
	listen(sock_tcp, 10);
	// accept an incoming connection
	addr_size = sizeof inc_addr;
	new_fd = accept(sock_tcp, (struct sockaddr *)&inc_addr, &addr_size);

	
	


	// receive from client

	recv(new_fd, func_name, 3, 0);
	int nums[1];
	recv(new_fd, nums, sizeof(int), 0);
	int count = nums[0];
	recv(new_fd, buff_data, count*sizeof(int), 0);

	//int count = sizeof(buff_data);

	/*for (int j = 0; j < 300; j++){
    	printf("%d\n",buff_data[j]);
    }
	*/
	printf("The AWS has received %d numbers from the client using TCP over port 25955. \n", count);


	// divide data buffer into three arrays
	int count_third = count/3;
	int count_2 = 2*count_third;
	int count_3 = 3*count_third;

	//printf("count_third=%d , count_2=%d , count_3=%d \n",count_third, count_2, count_3 );

	int buffA[(count_third)*sizeof(int)]; 
	int j = 0;
	for (; j < count_third; j++){
		buffA[j] = buff_data[j];
	}
	
	int buffB[(count_third)*sizeof(int)]; 
	for (; j < count_2; j++){
		buffB[j-count_third] = buff_data[j];
	}

	int buffC[(count_third)*sizeof(int)]; 
	for (; j < count_3; j++){
		buffC[j-count_2] = buff_data[j];
	}


	// SOCKET FOR UDP
	int sock_udp, rv, numbytes, sock_A, sock_B, sock_C;
	struct sockaddr_in udp_addr, serverA, serverB, serverC;
	struct addrinfo hints, *servinfo, *p;

	// socket for itself
	sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_udp == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&udp_addr, sizeof(udp_addr));
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(24955);
	udp_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((bind(sock_udp, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr_in))) == -1){
		perror("bind");
		exit(1);
	}

	// socket for server A
	sock_A = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_A == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&serverA, sizeof(serverA));
	serverA.sin_family = AF_INET;
	serverA.sin_port = htons(21955);
	int temp = inet_aton("127.0.0.1", &serverA.sin_addr.s_addr);

	// socket for server B
	sock_B = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_B == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&serverB, sizeof(serverB));
	serverB.sin_family = AF_INET;
	serverB.sin_port = htons(22955);
	temp = inet_aton("127.0.0.1", &serverB.sin_addr.s_addr);

	// socket for server C
	sock_C = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_C == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&serverC, sizeof(serverC));
	serverC.sin_family = AF_INET;
	serverC.sin_port = htons(23955);
	temp = inet_aton("127.0.0.1", &serverC.sin_addr.s_addr);




	// SEND TO SERVER A

	// sending function name
	if ((numbytes = sendto(sock_udp, func_name, 3*sizeof(char), 0,(struct sockaddr*)&serverA, sizeof(serverA)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending amount of numbers
 	if ((numbytes = sendto(sock_udp, (void*)&count_third, sizeof(int), 0,(struct sockaddr*)&serverA, sizeof(serverA)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending data
 	if ((numbytes = sendto(sock_udp, (void*)buffA, sizeof buffA, 0,(struct sockaddr*)&serverA, sizeof(serverA)) == -1)){
 		perror("sendto");
 		exit(1);
 	}
 	printf("The AWS sent %d numbers to Backend-Server A\n", count_third);




 	// SEND TO SERVER B
	
	// sending function name
 	if ((numbytes = sendto(sock_udp, func_name, 3*sizeof(char), 0,(struct sockaddr*)&serverB, sizeof(serverB)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending amount of numbers
 	if ((numbytes = sendto(sock_udp, (void*)&count_third, sizeof(int), 0,(struct sockaddr*)&serverB, sizeof(serverB)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending data
 	if ((numbytes = sendto(sock_udp, (void*)buffB, sizeof buffB, 0,(struct sockaddr*)&serverB, sizeof(serverB)) == -1)){
 		perror("sendto");
 		exit(1);
 	}
 	printf("The AWS sent %d numbers to Backend-Server B\n", count_third);

 	// SEND TO SERVER C

 	// sending function name
 	if ((numbytes = sendto(sock_udp, func_name, 3*sizeof(char), 0,(struct sockaddr*)&serverC, sizeof(serverC)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending amount of numbers
 	if ((numbytes = sendto(sock_udp, (void*)&count_third, sizeof(int), 0,(struct sockaddr*)&serverC, sizeof(serverC)) == -1)){
 		perror("sendto");
 		exit(1);
 	}

 	// sending data
 	if ((numbytes = sendto(sock_udp, (void*)buffC, sizeof buffC, 0,(struct sockaddr*)&serverC, sizeof(serverC)) == -1)){
 		perror("sendto");
 		exit(1);
 	}
 	printf("The AWS sent %d numbers to Backend-Server C\n", count_third);
}	


 	
 	


 	

