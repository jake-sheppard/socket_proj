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
	new_fd = accept(sock_tcp, (struct sockaddr *)&inc_addr, (socklen_t*)&addr_size);

	
	


	// receive from client

	recv(new_fd, func_name, 3, 0);
	int nums[1];
	recv(new_fd, nums, sizeof(int), 0);
	int count = nums[0];
	recv(new_fd, buff_data, count*sizeof(int), 0);

	
	printf("The AWS has received %d numbers from the client using TCP over port 25955. \n", count);


	// divide data buffer into three arrays
	int count_third = count/3;
	int count_2 = 2*count_third;
	int count_3 = 3*count_third;


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
	int temp = inet_aton("127.0.0.1", (in_addr*)&serverA.sin_addr.s_addr);

	// socket for server B
	sock_B = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_B == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&serverB, sizeof(serverB));
	serverB.sin_family = AF_INET;
	serverB.sin_port = htons(22955);
	temp = inet_aton("127.0.0.1", (in_addr*)&serverB.sin_addr.s_addr);

	// socket for server C
	sock_C = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_C == -1){
		perror("socket : ");
		exit(1);
	}
	bzero(&serverC, sizeof(serverC));
	serverC.sin_family = AF_INET;
	serverC.sin_port = htons(23955);
	temp = inet_aton("127.0.0.1", (in_addr*)&serverC.sin_addr.s_addr);




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

 	int origin_length;
 	struct sockaddr_in origin;
 	int resultA[1];
 	int resultB[1];
 	int resultC[1];


 	while(1){
 		// receive from A
 		numbytes = recvfrom(sock_udp, (void*)&resultA, sizeof(int), 0, (sockaddr*)&origin, (socklen_t*)&origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		printf("The AWS received reduction result of %s from Backend-Server A using UDP over port 24955 and it is %d\n", func_name, resultA[0]);

		// receive from B
		numbytes = recvfrom(sock_udp, (void*)&resultB, sizeof(int), 0, (sockaddr*)&origin, (socklen_t*)&origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		printf("The AWS received reduction result of %s from Backend-Server B using UDP over port 24955 and it is %d\n", func_name, resultB[0]);

		// receive from C
		numbytes = recvfrom(sock_udp, (void*)&resultC, sizeof(int), 0, (sockaddr*)&origin, (socklen_t*)&origin_length);
		if (numbytes == -1){
			perror("recvfrom");
		}
		printf("The AWS received reduction result of %s from Backend-Server C using UDP over port 24955 and it is %d\n", func_name, resultC[0]);




		func_name[3] = '\0';
		int min = strcmp(func_name,"min");
		int max = strcmp(func_name,"max");
		int sum = strcmp(func_name,"sum");
		int sos = strcmp(func_name,"sos");
		
		int result;
		if (min == 0){
			result = resultA[0];
			if (resultB[0] < result){
				result = resultB[0];
			}
			if (resultC[0] < result){
				result = resultC[0];
			}
		}
		else if (max == 0){
			result = resultA[0];
			if (resultB[0] > result){
				result = resultB[0];
			}
			if (resultC[0] > result){
				result = resultC[0];
			}
		}
		else if (sum == 0){
			result = resultA[0] + resultB[0] + resultC[0];
		}
		else if (sos == 0){
			result = resultA[0] + resultB[0] + resultC[0];
		}
		printf("The AWS has successfully finished the reduction %s: %d\n", func_name, result);
		// send result to client
		if (send(new_fd, (void*)&result, sizeof(int), 0) == -1){
		perror("send");
		exit(1);
		} 
		printf("The AWS has successfully finished sending the reduction value to client.\n");
 	}



 	
}	


 	
 	


 	

