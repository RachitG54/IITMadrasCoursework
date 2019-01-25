/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

#define BUFSIZE 1024
#define  h_addr h_addr_list[0]

using namespace std;

class TCPClient {
  private:
    int sockfd, portno;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;

  public:
    TCPClient(int port, char *name) {
        /* connect: create a connection with the server */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
                /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(name);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host as %s\n", hostname);
            exit(0);
        }
        portno = port;
        hostname = (char*)malloc(sizeof(name));
        strcpy(hostname,name);
    }
    void TCPconnect() {
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
        (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(portno);
        if (connect(sockfd, (const sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) 
          error("ERROR connecting");

        printf("Server address is: ");
        for(int i=0;i<server->h_length;i++) {
            printf("%d",((char*)(&serveraddr.sin_addr.s_addr))[i]);
            if(i!=server->h_length-1)printf(".");
        }
        printf("\nport is : %d\n",portno);
    }
    void TCPread(string s) {
        char buf[BUFSIZE];
        bzero(buf, BUFSIZE);
        strcpy(buf,s.c_str());
        /* send the message line to the server */
        int n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
            error("ERROR writing to socket");

        /* print the server's reply */
        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Echo from server: %s", buf);
    }
    ~TCPClient() {
      close(sockfd);
    }
    void error(string msg) {
      perror(msg.c_str());
      exit(1);
    }
};