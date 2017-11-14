#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


using namespace std;

void move(int sock, char *buf) {
	int n;
	char aux[5];
	while(buf[0] != '\0') {
 	 	/* Send message to the server */
		sprintf(aux,"%c",buf[0]);
   		n = write(sock, aux, 1);
   		strcpy(buf,buf+1);
  	 	if (n < 0) {
      			perror("ERROR writing to socket");
   	        	exit(1);
   	 	}
		sleep(1); // delay in miliseconds
	}
}

void sendCommand(char *hostname,char *server_s,int portno)
{
   int sockfd;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   char buffer[256];
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", hostname);
      exit(0);
   }
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   server = gethostbyname(server_s);
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   /* Now ask for a message from the user, this message
      * will be read by server
   */
   	printf("Please enter the message: ");
   	bzero(buffer,256);
   	fgets(buffer,255,stdin);
  	move(sockfd,buffer);
}
int main(int argc, char *argv[])
{
  sendCommand(argv[0],argv[1],argv[2]);
   	return 0;
}
