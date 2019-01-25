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
    char filename[256];
    char filename1[256];
    char noofb[20];
    int i;
    FILE *fp;
    
    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
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
    printf("Enter filename:(max 255 chars)");
    scanf("%s",filename);
    strcpy(filename1,filename);
    strcat(filename1,"1");
    printf("Enter number of bytes:(max 1000) ");
    scanf("%s",noofb);
    strcat(filename,"@");
    strcat(filename,noofb);
    /* send the message line to the server */
    n = write(sockfd, filename, strlen(filename));
    if (n < 0) 
      error("ERROR writing to socket");

    /* print the server's reply */
    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    if(strcmp(buf,"SORRY!")==0) {
        printf("Server says the file does not exist.\n");
    }
    else {
        fp = fopen(filename1,"w");
        fprintf(fp, "%s", buf);
        fclose(fp);
    }
    /*printf("Echo from server: %s", buf);*/
    close(sockfd);
    return 0;
}