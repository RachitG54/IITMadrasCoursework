/* 
 * tcpserver.cpp - A simple TCP echo server 
 * usage: tcpserver <port>
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>

using namespace std; 
#define BUFSIZE 1024
#define present(c,e) ((c).find(e) != (c).end())

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


struct spool {
      string from;
      string to;
      string date;
      string contents;
};
/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}
void getids(set<int> &s) {
  FILE* fp;
  int temp;
  fp = fopen("LSTU","r");
  if(fp==NULL) return;
  while(fscanf(fp,"%d",&temp)==1) {
    s.insert(temp);
  }
  fclose(fp);
  return;
}
void putids(set<int> &s) {
  FILE* fp;
  fp = fopen("LSTU","w");
  set<int>::iterator it;
  for (it = s.begin(); it != s.end(); ++it) {
      fprintf(fp,"%d ",*it);
  }
  fclose(fp);
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
  

  char output[BUFSIZE]; //Output that is sent to client
  set<int> ids;         //set consisting of user ids
  FILE* curruser = NULL;       //current user parameters
  int curruserid = -1;         //its id 
  char curruserids[BUFSIZE];   //its string
    /* 
   * check command line arguments 
  */

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
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
   * listen: make this socket ready to accept connection requests 
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  getids(ids);
  clientlen = sizeof(clientaddr);
  while (1) {

    /* 
     * accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
    if (childfd < 0) 
      error("ERROR on accept");

    bzero(buf, BUFSIZE);
    n = read(childfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    

    /* 
     * read: read input string from the client
     */
   char *cmdname, *intargs; /* command to be performed  & integer argument in string */
   int intarg;
   if(buf==NULL || n==0) {
    close(childfd);
    continue;
   }
   cmdname = strtok(buf, " ");
   bzero(output, BUFSIZE);
   if(strcmp(cmdname,"LSTU")==0) {
     cout<<"List Users\n";
     set<int>::iterator it;
      for (it = ids.begin(); it != ids.end(); ++it) {
          sprintf(output,"%s%d ",output,*it);
      }
   }
   else if(strcmp(cmdname,"ADDU")==0) {
     intargs = strtok(NULL, " ");
     if(intargs!=NULL) {
        intarg = atoi(intargs);
        if(!present(ids,intarg)) {
            ids.insert(intarg);
            FILE* fp;
            fp = fopen(intargs,"w");
            fclose(fp);
            putids(ids);
        }
        else {
          strcpy(output,"Userid already present");
        }
     }
     else {
      cout<<"Invalid User Entries\n";
     }

   }
   else if(strcmp(cmdname,"USER")==0) {
     cout<<"Setting Users\n";
     intargs = strtok(NULL, " ");
     if(intargs!=NULL) {
        intarg = atoi(intargs);
        if(present(ids,intarg)) {
            curruser = fopen(intargs,"r+");
            curruserid = intarg;
            strcpy(curruserids,intargs);
        }
        else {
          strcpy(output,"Userid invalid");
        }
     }
     else {
      cout<<"Invalid User Entries\n";
     }
   }
   else if(strcmp(cmdname,"READM")==0) {
     cout<<"Reading message\n";
     if(curruser==NULL) {
        strcpy(output,"No More Mail");
     }
     else {
        char tempstr[BUFSIZE];
        if(fscanf (curruser," %[^\n]%*c", tempstr) > 0) {       //Takes the from line
            //cout<<tempstr<<"\n";
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the To line
            //cout<<tempstr<<"\n";
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Date line
            //cout<<tempstr<<"\n";
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Contents line
            //cout<<tempstr<<"\n";
            int hashcount = 0;
            bzero(tempstr, BUFSIZE);
            int ij = 0;
            char tempc;
            while(hashcount<3) {
              fscanf(curruser,"%c", &tempc);
              if(tempc=='#') {
                if(hashcount>0 && output[ij-1]=='#')
                  hashcount++;
                else {
                  hashcount = 1;
                }
              }
              output[ij++] = tempc;
            }
            output[ij] = '\0';
        }
        else {
          strcpy(output,"No More Mail");
          curruser = NULL;
        }
     }
   }
   else if(strcmp(cmdname,"DELM")==0) {
     cout<<"Deleting message\n";
     if(curruser==NULL) {
        strcpy(output,"No More Mail");
     }
     else {
        char tempstr[BUFSIZE];
        long int filepos = ftell(curruser);
        FILE *ft,*ft2;
        ft = fopen("Temp","w+");
        ft2 = fopen(curruserids,"r");
        
        if(fscanf (curruser," %[^\n]%*c", tempstr) > 0) {
          while(filepos != ftell(ft2) && fscanf(ft2," %[^\n]%*c", tempstr) > 0) {
            fprintf(ft, "%s\n", tempstr);
            fscanf (ft2," %[^\n]%*c", tempstr);              //Takes the To line
            fprintf(ft, "%s\n", tempstr);
            fscanf (ft2," %[^\n]%*c", tempstr);              //Takes the Data line
            fprintf(ft, "%s\n", tempstr);
            fscanf (ft2," %[^\n]%*c", tempstr);              //Takes the Contents line
            fprintf(ft, "%s\n", tempstr);
            int hashcount = 0;
            bzero(tempstr, BUFSIZE);
            int ij = 0;
            char tempc;
            while(hashcount<3) {
              fscanf(ft2,"%c", &tempc);
              fprintf(ft,"%c",tempc);
              if(tempc=='#') {
                if(hashcount>0 && tempstr[ij-1]=='#')
                  hashcount++;
                else {
                  hashcount = 1;
                }
              }
              tempstr[ij++] = tempc;
            }
            tempstr[ij] = '\0';
            fprintf(ft,"\n");
          }
          fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the To line
          cout<<tempstr<<endl;
          fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Data line
          cout<<tempstr<<endl;
          fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Contents line
          cout<<tempstr<<endl;
          int hashcount = 0;
          bzero(tempstr, BUFSIZE);
          int ij = 0;
          char tempc;
          while(hashcount<3) {
            fscanf(curruser,"%c", &tempc);
            if(tempc=='#') {
              if(hashcount>0 && tempstr[ij-1]=='#')
                hashcount++;
              else {
                hashcount = 1;
              }
            }
            tempstr[ij++] = tempc;
          }
          tempstr[ij] = '\0';
          while(fscanf(curruser," %[^\n]%*c", tempstr) > 0) {
            fprintf(ft, "%s\n", tempstr);
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the To line
            fprintf(ft, "%s\n", tempstr);
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Data line
            fprintf(ft, "%s\n", tempstr);
            fscanf (curruser," %[^\n]%*c", tempstr);              //Takes the Contents line
            fprintf(ft, "%s\n", tempstr);
            int hashcount = 0;
            bzero(tempstr, BUFSIZE);
            int ij = 0;
            char tempc;
            while(hashcount<3) {
              fscanf(curruser,"%c", &tempc);
              fprintf(ft,"%c",tempc);
              if(tempc=='#') {
                if(hashcount>0 && tempstr[ij-1]=='#')
                  hashcount++;
                else {
                  hashcount = 1;
                }
              }
              tempstr[ij++] = tempc;
            }
            tempstr[ij] = '\0';
            fprintf(ft,"\n");
          }
          remove(curruserids);
          rename("Temp", curruserids);
          strcpy(output,"Message Deleted");
          fclose(ft);
          fclose(ft2);
          fclose(curruser);
          curruser = fopen(curruserids,"r+");
          fseek(curruser,filepos,SEEK_SET);
        }
        else {
          strcpy(output,"No More Mail");
          curruser = NULL;
        }
     }
   }
   else if(strcmp(cmdname,"SEND")==0) {

     intargs = strtok(NULL, " ");
     if(intargs!=NULL) {
        cout<<"Sending message\n";
        intarg = atoi(intargs);
        if(present(ids,intarg)) {
            char *messageuser;
            messageuser = strtok(NULL,"@");
            if(messageuser==NULL) {
              cout<<"Invalid User Entries\n";
            }
            else {
              char tempstr[BUFSIZE];
              FILE* fto;
              fto = fopen(intargs,"a");
              fprintf(fto,"From: %s\n",curruserids);
              fprintf(fto,"To: %s\n",intargs);
              std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
              std::time_t now_c = std::chrono::system_clock::to_time_t(now);
              string now_s = std::ctime(&now_c);
              now_s.resize(now_s.size()-1);
              fprintf(fto,"Date: %s\n",now_s.c_str());
              fprintf(fto,"Contents\n%s\n",messageuser);
              fclose(fto);
            }
        }
        else {
          strcpy(output,"Userid invalid");
        }
     }
     else {
      cout<<"Invalid User Entries\n";
     }
   }
   else if(strcmp(cmdname,"DONEU")==0) {
     cout<<"Done with user\n";
     curruser = NULL;
   }
   else if(strcmp(cmdname,"Donothing")==0) {
     cout<<"Nothing to be done\n";
   }
   else {
     cout<<"Quitting\n";
     close(childfd);
     continue;
   }

    /* 
     * gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n", 
	   hostp->h_name, hostaddrp);
    
    printf("Client address is: %s\n",hostaddrp);
    /*for(i=0;i<server->h_length;i++) {
        printf("%d",((char*)(&serveraddr.sin_addr.s_addr))[i]);
        if(i!=server->h_length-1)printf(".");
    }*/
    printf("port is : %d\n",clientaddr.sin_port);
    

    
    /* 
     * write: echo the input string back to the client 
     */
    n = write(childfd, output, strlen(output));
    if (n < 0) 
      error("ERROR writing to socket");
    close(childfd);
  }
}
