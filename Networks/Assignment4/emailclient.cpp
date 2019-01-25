/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

using namespace std; 

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
    
    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    int flag = 1;
    while(flag==1) {
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
      if (connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) 
            error("ERROR connecting");

      /*printf("Server address is: ");
      for(int i=0;i<server->h_length;i++) {
          printf("%d",((char*)(&serveraddr.sin_addr.s_addr))[i]);
          if(i!=server->h_length-1)printf(".");
      }
      printf("\nport is : %d\n",portno);*/

      char *input,*cmdname,*intargs;
      int intarg;
      cout<<"Main-Prompt> ";
      scanf (" %[^\n]%*c", buf);
      if(strlen(buf)==0) {
        cout<<"Invalid command\n";
        strcpy(buf,"Gibberish");
      }
      cmdname = strtok(buf, " ");
      
      if(strcmp(cmdname,"Listusers")==0) {
       strcpy(buf,"LSTU");
      }
      else if(strcmp(cmdname,"Adduser")==0) {

       intargs = strtok(NULL, " ");
       if(intargs==NULL) {
          cout<<"Invalid Input\n";
          strcpy(buf,"Donothing");
       }
       else {
          sprintf(buf,"ADDU %s",intargs);
       }
      }
      else if(strcmp(cmdname,"SetUser")==0) {

       intargs = strtok(NULL, " ");
       if(intargs==NULL) {
          cout<<"Invalid Input\n";
          strcpy(buf,"Donothing");
       }
       else {
          sprintf(buf,"USER %s",intargs);
          char curruserid[BUFSIZE];         //Storing current user
          strcpy(curruserid,intargs);
          /* send the message line to the server */
          n = write(sockfd, buf, strlen(buf));
          if (n < 0) 
            error("ERROR writing to socket");

          /* print the server's reply */
          bzero(buf, BUFSIZE);
          n = read(sockfd, buf, BUFSIZE);
          if (n < 0) 
            error("ERROR reading from socket");
          if(strlen(buf)!=0)
            printf("%s\n", buf);
          int flag2 = 1;
          while(flag2==1) {
            cout<<"Sub-Prompt-"<<curruserid<<">";
            // Creating another connection
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) 
                error("ERROR opening socket");
            server = gethostbyname(hostname);
            if (server == NULL) {
                fprintf(stderr,"ERROR, no such host as %s\n", hostname);
                exit(0);
            }
            bzero((char *) &serveraddr, sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;
            bcopy((char *)server->h_addr, 
              (char *)&serveraddr.sin_addr.s_addr, server->h_length);
            serveraddr.sin_port = htons(portno);
            if (connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) 
                  error("ERROR connecting");

            scanf (" %[^\n]%*c", buf);
            if(strlen(buf)==0) {
              cout<<"Invalid command\n";
              strcpy(buf,"Gibberish");
            }
            cmdname = strtok(buf, " ");
            if(strcmp(cmdname,"Read")==0) {
             strcpy(buf,"READM");
            }
            else if(strcmp(cmdname,"Delete")==0) {
             strcpy(buf,"DELM");
            }
            else if(strcmp(cmdname,"Send")==0) {
             intargs = strtok(NULL, " ");
             if(intargs==NULL) {
                cout<<"Invalid Input, enter again\n";
                strcpy(buf,"Donothing");
             }
             else {
                cout<<"Type Message (Must terminate by 3 hashes):";
                char messagesend[BUFSIZE-30];
                scanf (" %[^\n]%*c", messagesend);
                sprintf(buf,"SEND %s %s",intargs,messagesend);
             }
            }
            else if(strcmp(cmdname,"Done")==0) {
              strcpy(buf,"DONEU");
              flag2 = -1;
            }
            else {
              cout<<"Invalid Input, enter again\n";
              strcpy(buf,"Donothing");
            }
            /* send the message line to the server */
            n = write(sockfd, buf, strlen(buf));
            if (n < 0) 
              error("ERROR writing to socket");

            /* print the server's reply */
            bzero(buf, BUFSIZE);
            n = read(sockfd, buf, BUFSIZE);
            if (n < 0) 
              error("ERROR reading from socket");
            if(strlen(buf)!=0)
              printf("%s\n", buf);
          }
          strcpy(buf,"Donothing");
        }
      }
      else if(strcmp(cmdname,"Quit")==0) {
       strcpy(buf,"QUIT");
       flag = -1;
      }
      else {
        cout<<"Invalid command, enter again\n";
        strcpy(buf,"Donothing");
      }
      
      /* send the message line to the server */
      n = write(sockfd, buf, strlen(buf));
      if (n < 0) 
        error("ERROR writing to socket");

      /* print the server's reply */
      bzero(buf, BUFSIZE);
      n = read(sockfd, buf, BUFSIZE);
      if (n < 0) 
        error("ERROR reading from socket");
      if(strlen(buf)!=0)
      printf("%s\n", buf);
    }
    close(sockfd);
    return 0;
}
