/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

#define BUFSIZE 1024
#define  h_addr h_addr_list[0]

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    int i,count=0;
    time_t   now1,now2,finalnow;            /* current time         */
    clock_t start, end;
    double cpu_time_used;
    
    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    (void) time(&now1);
    /*now1 = htonl((unsigned long)(now1 + UNIXEPOCH));*/
    start = clock();
    while(count<1) {
        /* connect: create a connection with the server */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(hostname);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host as %s\n", hostname);
            exit(0);
        }

        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(portno);
        if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0) 
              error("ERROR connecting");

        printf("Server address is: ");
        for(i=0;i<server->h_length;i++) {
            printf("%d",((char*)(&serveraddr.sin_addr.s_addr))[i]);
            if(i!=server->h_length-1)printf(".");
        }
        printf("\nport is : %d\n",portno);
        /*printf("\nhey%s,%d,%d\n",(char*)&serveraddr.sin_addr.s_addr,server->h_length,serveraddr.sin_port);*/
        /* get message line from the user */
        bzero(buf, BUFSIZE);
        strcpy(buf,"My message");
        /* send the message line to the server */
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
          error("ERROR writing to socket");

        /* print the server's reply */
        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("Echo from server: %s", buf);
        close(sockfd);
        count++;
    }
    end = clock();
    (void) time(&now2);
    /*now2 = htonl((unsigned long)(now2 + UNIXEPOCH));
    finalnow = ntohl((unsigned long)now);     put in host order    */
    finalnow = now2-now1;       /* convert UCT to UNIX epoch    */
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\ntime taken is: %f\n", cpu_time_used/count);
    return 0;
}