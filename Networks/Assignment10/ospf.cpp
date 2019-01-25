/* 
 * udpclient.cpp - A simple UDP client
 * usage: tcpclient <host> <port>
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <time.h>
#include <pthread.h>
#include <ctime> 
#include <deque>
#include <chrono>
#include <set>
#include <queue>
#include <random>
#include <map>
#include <mutex>          // std::mutex

using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point Timestruct;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mis;
typedef std::chrono::duration<float> fsec;
typedef vector<int> vi;
typedef vector<double> vd;
typedef long long ll;
typedef vector<ll> vlli;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef pair<double,int> idd;
typedef vector<idd> vid;
typedef vector<vid> vvid;

#define BUFSIZE 10024
#define  h_addr h_addr_list[0]
#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())
#define Nlimit 100

mutex mtx;
double neighbours[Nlimit][4];
vi adjlist;
vi lastknownseq(Nlimit,-1);
double helloint = 1;
double lsaint = 5;
double spfint = 20;
int nodeid = -1;
int Nmax = -1;
vvid adjlistnet(Nlimit);
string sout;
/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

void *Sender(void *threadarg) {
  int *id= (int *)threadarg;
  //cout<<*id<<" "<<Nmax<<"\n";
  auto t1 = Time::now();
  auto t2 = Time::now();
  int N = adjlist.size();
  int portno, n;
  struct sockaddr_in serveraddr[Nmax];
  struct hostent *server;
  char *hostname;
  char buf[BUFSIZE];
  vi sockfdarr(Nmax);
  hostname = (char *)malloc(sizeof(char)*(strlen("localhost")+1));
  strcpy(hostname,"localhost");
  portno = 10000+(*id);

  /* gethostbyname: get the server's DNS entry */
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host as %s\n", hostname);
    exit(0);
  }
  /* socket: create the socket */
  /* connect: create a connection with the server */
  REP(i,0,N-1) {
    int i2 = adjlist[i];
    //cout<<"i2 is "<<i2<<"\n";
    sockfdarr[i2] = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfdarr[i2] < 0) 
      error("ERROR opening socket"); 


    /* build the server's Internet address */
    bzero((char *) &serveraddr[i2], sizeof(serveraddr[i2]));
    serveraddr[i2].sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
      (char *)&serveraddr[i2].sin_addr.s_addr, server->h_length);
    serveraddr[i2].sin_port = htons(10000+i2);

    if (connect(sockfdarr[i2], (const sockaddr*)&serveraddr[i2], sizeof(serveraddr[i2])) < 0) {
      error("ERROR connecting");
      char temp[BUFSIZE];
      sprintf(temp,"%d",i2);
      error(temp);
    }
    //cout<<"connecting "<<i2<<"\n";
    if (sockfdarr[i2] < 0) {
      error("ERROR opening socket"); 
      char temp[BUFSIZE];
      sprintf(temp,"%d",i2);
      error(temp);
    }
  }
  int seqnumber = 0;
  while(1) {
    fsec fs = Time::now() - t1;
    fsec fs2 = Time::now() - t2;
    if(fs.count() > helloint ) {
      // generate new packet
      //cout<<fs.count()*1000<<"\n";
      REP(i,0,N-1) {
        int i2 = adjlist[i];
        /* get message line from the user */
        bzero(buf, BUFSIZE);
        sprintf(buf,"HELLO %d",nodeid);
        /* send the message line to the server */
        //mtx.lock();
        n = write(sockfdarr[i2], buf, strlen(buf));
        bzero(buf, BUFSIZE);
        n = read(sockfdarr[i2], buf, BUFSIZE);
        //mtx.unlock();
        // if (n < 0) 
        //   cout<<"no connection\n";
        // else
        //   printf("Echo from server: %s\n", buf);
        int temp;
        int neighbourthatsends;
        double val = -1;
        sscanf(buf,"HELLOREPLY %d %d %lf",&neighbourthatsends,&temp,&val);
        if(val!=-1) {
          neighbours[neighbourthatsends][3] = val;
        }
      }
      //reset the clock timers
      t1 = Time::now();
    }
    if(fs2.count() > lsaint) {
      //cout<<fs2.count()*1000<<"\n";
      int cnt = 0;
      stringstream ss;
      mtx.lock();
      adjlistnet[nodeid].clear();
      REP(j,0,N-1) {
        if(neighbours[adjlist[j]][3]!=-1) {
          cnt++;
          idd temp;
          int x = adjlist[j];
          double y = neighbours[adjlist[j]][3];
          temp.first = y;temp.second = x;
          adjlistnet[nodeid].pb(temp);
          ss<<" "<<x<<" "<<y;
        }
      }
      lastknownseq[nodeid] = seqnumber+1;
      mtx.unlock();
      seqnumber++;
      REP(i,0,N-1) {
        int i2 = adjlist[i];
        /* get message line from the user */
        bzero(buf, BUFSIZE);
        sprintf(buf,"LSA %d %d %d%s",nodeid,seqnumber,cnt,ss.str().c_str());
        //cout<<buf<<"\n";
        /* send the message line to the server */
        //mtx.lock();
        n = write(sockfdarr[i2], buf, strlen(buf));
        // if (n < 0) 
        //   cout<<"no connection\n";
        n = read(sockfdarr[i2], buf, BUFSIZE);
        //mtx.unlock();
      }
      //reset the clock timers
      t2 = Time::now(); 
    }
  }

  //cout<<"Heyexited\n";
  REP(i,0,N-1) {
    close(sockfdarr[i]); 
  }
}
void func(vi &a);
void *Reciever(void *threadarg) {
    
  int parentfd; /* parent socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  unsigned int  alen;   /* from-address length    */

  int *id = (int *)threadarg;
  portno = 10000+(*id);
  //cout<<portno<<"\n";
  parentfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");
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
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<> dist(0, 1);

  vi sockfdarr(Nmax);
  func(sockfdarr);
  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
    while (1) {
        bzero(buf, BUFSIZE);
        alen = sizeof(clientaddr);
        if (recvfrom(parentfd, buf, sizeof(buf), 0,
               (struct sockaddr *)&clientaddr, &alen) < 0)
          error("Error recieveing\n");
        //cout<<buf<<"\n";
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
          error("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL)
          error("ERROR on inet_ntoa\n");
        
        int helloid;
        char bufcpy[BUFSIZE];
        strcpy(bufcpy,buf);
        char *msg;
        char *buf2;
        /* get the first token */
        msg = strtok(bufcpy, " ");
       
        /* walk through other tokens */
        if( msg != NULL ) {
          buf2 = strtok(NULL, ",");
        }
        if(strcmp(msg,"HELLO")==0) {
          sscanf(buf2,"%d",&helloid);
          double val = dist(mt);
          double minval = neighbours[helloid][1];
          double maxval = neighbours[helloid][2];
          val = val*(maxval-minval)+minval;
          sprintf(buf,"%s %d %d %lf","HELLOREPLY",*id,helloid,val);
          //cout<<buf<<"\n";
          /* 
           * write: echo the input string back to the client 
           */
          (void) sendto(parentfd, (char *)buf, sizeof(buf), 0,(struct sockaddr *)&clientaddr, sizeof(clientaddr));
        }
        else if(strcmp(msg,"LSA")==0) {
          //cout<<buf<<"\n";
          int seqnumber,noofentries;
          char buf3[BUFSIZE];
          stringstream ss;
          ss<<buf2;
          ss>>helloid;
          ss>>seqnumber;
          ss>>noofentries;
          //cout<<helloid<<":"<<seqnumber<<":"<<noofentries<<"\n";
          (void) sendto(parentfd, (char *)buf, sizeof(buf), 0,(struct sockaddr *)&clientaddr, sizeof(clientaddr)); 
          if(lastknownseq[helloid]<seqnumber) {
            lastknownseq[helloid] = seqnumber;
            mtx.lock();
            adjlistnet[helloid].clear();
            REP(i,0,noofentries-1) {
              idd temp;
              ss>>temp.second;
              ss>>temp.first;
              //cout<<temp.first<<" "<<temp.second<<"\n"; 
              adjlistnet[helloid].pb(temp);
            }
            mtx.unlock();
            REP(i,0,adjlist.size()-1) {
              //cout<<buf<<"\n";
              /* 
               * write: echo the input string back to the client 
               */
              int n = write(sockfdarr[adjlist[i]], buf, strlen(buf));
              //n = read(sockfdarr[adjlist[i]], buf, BUFSIZE);
            }
          }
        }
    }
    REP(i,0,Nmax-1) {
      close(sockfdarr[i]);
    }
    close(parentfd);
    pthread_exit(NULL);
}
void func(vi &sockfdarr2) {
  int *id;id = &nodeid;
  //cout<<*id<<" "<<Nmax<<"\n";
  auto t1 = Time::now();
  auto t2 = Time::now();
  int N = adjlist.size();
  int portno, n;
  struct sockaddr_in serveraddr[Nmax];
  struct hostent *server;
  char *hostname;
  char buf[BUFSIZE];
  
  hostname = (char *)malloc(sizeof(char)*(strlen("localhost")+1));
  strcpy(hostname,"localhost");
  portno = 10000+(*id);

  /* gethostbyname: get the server's DNS entry */
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host as %s\n", hostname);
    exit(0);
  }
  /* socket: create the socket */
  /* connect: create a connection with the server */
  REP(i,0,N-1) {
    int i2 = adjlist[i];
    sockfdarr2[i2] = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfdarr2[i2] < 0) 
      error("ERROR opening socket"); 


    /* build the server's Internet address */
    bzero((char *) &serveraddr[i2], sizeof(serveraddr[i2]));
    serveraddr[i2].sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
      (char *)&serveraddr[i2].sin_addr.s_addr, server->h_length);
    serveraddr[i2].sin_port = htons(10000+i2);

    if (connect(sockfdarr2[i2], (const sockaddr*)&serveraddr[i2], sizeof(serveraddr[i2])) < 0) {
      error("ERROR connecting");
      char temp[BUFSIZE];
      sprintf(temp,"%d",i2);
      error(temp);
    }
    if (sockfdarr2[i2] < 0) {
      error("ERROR opening socket"); 
      char temp[BUFSIZE];
      sprintf(temp,"%d",i2);
      error(temp);
    }
  }

}
void *topology(void *threadarg) {
  auto t0 = Time::now();
  auto t1 = Time::now();

  ofstream outptr2;
  outptr2.open(sout,ios::out);
  while(1) {
    fsec fs = Time::now() - t1;
    if(fs.count() > spfint ) {
      // generate new packet
      //cout<<fs.count()*1000<<"\n";
      mtx.lock();
      /*
      REP(i,0,Nmax-1) {
        REP(j,0,adjlistnet[i].size()-1) {
          idd temp = adjlistnet[i][j];
          cout<<temp.first<<" "<<temp.second<<"::";
        }
        cout<<"\n";
      }
      */
      vd D(Nmax, 987654321); 
      D[nodeid] = 0;
      priority_queue<idd,vid, greater<idd> > Q;
      Q.push(idd(0,nodeid));
      vi nearest(Nmax,-1);
      while(!Q.empty()) {
        idd top = Q.top();
        Q.pop();        
        int v = top.second;
        double d = top.first;
        if(d <= D[v]) {
          REP(i,0,adjlistnet[v].size()-1) {
            int v2 = adjlistnet[v][i].second;
            double cost = adjlistnet[v][i].first;
            if(D[v2] > D[v] + cost) {
              nearest[v2] = v;
              D[v2] = D[v] + cost;
              Q.push(idd(D[v2], v2));
            }
          }
        }
      }
      mtx.unlock();
      vvi path(Nmax);
      REP(i,0,Nmax-1) {
        if(D[i]!=987654321) {
          for(int curr = i;curr!=nodeid;) {
            path[i].pb(curr);
            if(nearest[curr]!=-1)
              curr = nearest[curr];
            else {
              break;
            }
          }
          path[i].pb(nodeid);
        }
      }
      //reset the clock timers
      t1 = Time::now();
      ofstream outptr;
      outptr.open(sout,ios::app);
      outptr<<"Routing table for Node No."<<nodeid<<" at Time "<<((fsec)(t1-t0)).count()<<"\n";
      outptr<<"Destination\t\tPath\t\tCost\n";
      REP(i,0,Nmax-1) {
        outptr<<i<<"\t\t";
        for(int j = path[i].size()-1;j>0;j--) {
          outptr<<path[i][j]<<"-";
        }
        if(path[i].size()>0) {
          outptr<<path[i][0];
          outptr<<"\t\t"<<D[i]<<"\n";
        }
        else {
          outptr<<"\n";
        }
      }
      outptr.close();
    }
  }
}
int main(int argc, char **argv) {
   /* 
   * check command line arguments 
   */
    char* infile = NULL;char* outfile = NULL;
    for(int j=1;j<argc;j++) {
        if(strcmp(argv[j],"-i") == 0) {
            if(j+1==argc) break;
            nodeid = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-h") == 0) {
            if(j+1==argc) break;
            sscanf(argv[j+1],"%lf",&helloint);
        }
        else if(strcmp(argv[j],"-s") == 0) {
            if(j+1==argc) break;
            sscanf(argv[j+1],"%lf",&spfint);
        }
        else if(strcmp(argv[j],"-a") == 0) {
            if(j+1==argc) break;
            sscanf(argv[j+1],"%lf",&lsaint);
        }
        else if(strcmp(argv[j],"-f") == 0) {
            if(j+1==argc) break;
            infile = argv[++j];
        }
        else if(strcmp(argv[j],"-o") == 0) {
            if(j+1==argc) break;
            outfile = argv[++j];
        }
    }
    if(infile == NULL || outfile == NULL || helloint == -1 || lsaint == -1 || spfint == -1) {
        cout<<"Enter the requiered fields\n";
        exit(0);
    }
    REP(i,0,Nlimit-1) {
      neighbours[i][3] = -1;
    }
    ifstream inptr;
    inptr.open(infile,ios::in);

    stringstream ss;
    ss<<outfile;
    ss<<"-"<<nodeid;
    sout;
    ss>>sout;
    int N,m;
    inptr>>N>>m;
    Nmax = N;
    if(nodeid<0 || nodeid>=N) {
        cout<<"Enter correctly\n";
        exit(0);
    }
    REP(i,0,N-1) {
        neighbours[i][0] = -1;
    }
    REP(i,0,m-1) {
        int u,v;
        double c1,c2;
        inptr>>u>>v>>c1>>c2;
        vi temp;
        if(u==nodeid) {
          adjlist.pb(v);
          neighbours[v][0] = 1;
          neighbours[v][1] = c1;
          neighbours[v][2] = c2;
        }
        else if(v==nodeid) {
          adjlist.pb(u);
          neighbours[u][0] = 1;
          neighbours[u][1] = c1;
          neighbours[u][2] = c2;   
        }
    }

    inptr.close();
    pthread_t rcvr;
    pthread_attr_t attr;
    void *status;
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int rc = pthread_create(&rcvr, &attr, Reciever, (void *)&nodeid);
        
    if (rc){
       cout << "Error:unable to create thread reciever," << rc << endl;
       exit(-1);
    }
    //cout<<"Hey\n";


    pthread_t sender;
    pthread_attr_t attr2;
    void *status2;
    pthread_attr_init(&attr2);

    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
    rc = pthread_create(&sender, &attr2, Sender, (void *)&nodeid);
        
    if (rc){
       cout << "Error:unable to create thread sender," << rc << endl;
       exit(-1);
    }
    pthread_t topo;
    pthread_attr_t attr3;
    void *status3;
    pthread_attr_init(&attr3);

    pthread_attr_setdetachstate(&attr3, PTHREAD_CREATE_JOINABLE);
    rc = pthread_create(&topo, &attr3, topology, (void *)&nodeid);
        
    if (rc){
       cout << "Error:unable to create thread sender," << rc << endl;
       exit(-1);
    }

    rc = pthread_join(rcvr, &status);
    if (rc){
     cout << "Error:unable to join," << rc << endl;
     exit(-1);
    }

    //rc = pthread_create(&reciever, &attr, RecieverWindow, (void *)(&sockfd));
    
    return 0;
}