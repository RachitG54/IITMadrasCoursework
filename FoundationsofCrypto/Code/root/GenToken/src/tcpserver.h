/* 
 * tcpserver.c - A simple TCP echo server 
 * usage: tcpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

#define BUFSIZE 1024

#if 0
/* 
 * Structs exported from in.h
 */

/* Internet address */
struct in_addr {
  unsigned int s_addr; 
};

/* Internet style socket address */
struct sockaddr_in  {
  unsigned short int sin_family; /* Address family */
  unsigned short int sin_port;   /* Port number */
  struct in_addr sin_addr;	 /* IP address */
  unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};

/*
 * Struct exported from netdb.h
 */

/* Domain name service (DNS) host entry */
struct hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* length of address */
  char    **h_addr_list;  /* list of addresses */
}
#endif

class TCPServer {
  private:
    int parentfd; /* parent socket */
    int childfd; /* child socket */
    int portno; /* port to listen on */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
  public:
    TCPServer(int port) {
      parentfd = socket(AF_INET, SOCK_STREAM, 0);
      if (parentfd < 0) {
        error("ERROR opening socket");
      }
      int optval = 1;
      setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
      portno = port;
    }
    void TCPbind() {
      bzero((char *) &serveraddr, sizeof(serveraddr));
      /* this is an Internet address */
      serveraddr.sin_family = AF_INET;
      /* let the system figure out our IP address */
      serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
      /* this is the port we will listen on */
      serveraddr.sin_port = htons((unsigned short)portno);
      if (bind(parentfd, (struct sockaddr *) &serveraddr, 
       sizeof(serveraddr)) < 0) 
      error("ERROR on binding");

      if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
      error("ERROR on listen");
    }
    void TCPConnect() {
      int clientlen; /* byte size of client's address */
      clientlen = sizeof(clientaddr);
      childfd = accept(parentfd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
      if (childfd < 0) 
        error("ERROR on accept");

      struct hostent *hostp; /* client host info */
      hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
      
      if (hostp == NULL)
        error("ERROR on gethostbyaddr");

      char* hostaddrp = inet_ntoa(clientaddr.sin_addr);
      
      if (hostaddrp == NULL)
        error("ERROR on inet_ntoa\n");

      printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);
      printf("Client address is: %s\n",hostaddrp);
      printf("port is : %d\n",clientaddr.sin_port);
    }
    string TCPread() {
      char buf[BUFSIZE]; /* message buffer */
      bzero(buf, BUFSIZE);
      int n = read(childfd, buf, BUFSIZE);
      if (n < 0) 
      error("ERROR reading from socket");
      printf("server received %d bytes: %s\n", n, buf);
      return string(buf);
    }
    void TCPwrite(string s) {
      char buf[BUFSIZE]; /* message buffer */
      bzero(buf, BUFSIZE);
      strcpy(buf,s.c_str());
      /* 
      * write: echo the input string back to the client 
      */
      int n = write(childfd, buf, strlen(buf));
      if (n < 0) 
      error("ERROR writing to socket");
    }
    void TCPConnectclose() {
      close(childfd);
    }
    ~TCPServer() {
      close(parentfd);
    }

    /*
     * error - wrapper for perror
     */
    void error(string msg) {
      perror(msg.c_str());
      exit(1);
    }
};
