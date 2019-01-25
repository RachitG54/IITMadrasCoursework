/* 
 * udpserver.c - A simple UDP server 
 * usage: tcpserver <port>
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <ctime>
#include <random>
#include <chrono>

using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point Timestruct;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mis;
typedef std::chrono::duration<float> fsec;


#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)
#define BUFSIZE 10024

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

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  int i;
  unsigned int  alen;   /* from-address length    */
  char ans[BUFSIZE];
  /* 
   * check command line arguments 
   */
  string portnos;
  int debugflag = -1;
  int maxpackets = -1;
  float randomdropprob = -1;
  portno = -1;
  for(int j=1;j<argc;j++) {
    if(strcmp(argv[j],"-d") == 0) {
      debugflag = 1;
    }
    else if(strcmp(argv[j],"-p") == 0) {
        if(j+1==argc) break;
        portno = atoi(argv[j+1]);
    }
    else if(strcmp(argv[j],"-n") == 0) {
        if(j+1==argc) break;
        maxpackets = atoi(argv[j+1]);
    }
    else if(strcmp(argv[j],"-e") == 0) {
        if(j+1==argc) break;
        sscanf(argv[j+1],"%f",&randomdropprob);
        //randomdropprob = atoi(argv[j+1]);
    }
  }
  if(portno == -1 || maxpackets == -1 || randomdropprob == -1 ) {
    cout<<"Enter the requiered fields\n";
    exit(0);
  }
  
  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  auto t0 = Time::now();
  clientlen = sizeof(clientaddr);
  int expectedack = 0;
  int packetssent = 0;
  while (1) {

    /* 
     * gethostbyaddr: determine who sent the message 
     */

    bzero(buf, BUFSIZE);
    alen = sizeof(clientaddr);
    if (recvfrom(parentfd, buf, sizeof(buf), 0,
           (struct sockaddr *)&clientaddr, &alen) < 0)
      error("Error recieveing\n");
     bzero(ans, BUFSIZE);
     int seqnumber = buf[0];

     /*Decide if we have to drop the packet*/
     random_device rd;
     mt19937 mt(rd());
     uniform_real_distribution<double> dist(0.0, 1.0);
     float r = dist(mt);
     //cout<<r<<"\n";
     if(r<randomdropprob) {
        if(debugflag==1) {
          fsec fs2 = Time::now() - t0;
          int minis = std::chrono::duration_cast<ms>(fs2).count(); int micros = std::chrono::duration_cast<mis>(fs2).count();
          micros = micros%1000;
          cout<<"Seq #: "<<seqnumber<<"\tTime Recieved: "<<minis<<":"<<micros<<"\tPacket dropped: true\n";
        }
        continue;
     }
     if(seqnumber==expectedack) {
        packetssent++;
        //cout<<packetssent<<"\n";
        expectedack = (expectedack+1)%128;
        ans[0] = expectedack;
     }
     else {
        ans[0] = expectedack;
     }
     if(debugflag==1) {
        fsec fs2 = Time::now() - t0;
        int minis = std::chrono::duration_cast<ms>(fs2).count(); int micros = std::chrono::duration_cast<mis>(fs2).count();
        micros = micros%1000;
        cout<<"Seq #: "<<seqnumber<<"\tTime Recieved: "<<minis<<":"<<micros<<"\tPacket dropped: false\n";
        
     }
     
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    
    

    /* 
     * read: read input string from the client
     */
    /*n = read(childfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("server received %d bytes: %s", n, buf);
    */
    /* 
     * write: echo the input string back to the client 
     */
    (void) sendto(parentfd, (char *)ans, sizeof(ans), 0,
      (struct sockaddr *)&clientaddr, sizeof(clientaddr));

    /*close(childfd);*/
  }
}