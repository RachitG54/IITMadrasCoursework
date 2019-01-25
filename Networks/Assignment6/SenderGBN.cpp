/* 
 * udpclient.cpp - A simple UDP client
 * usage: tcpclient <host> <port>
 */
#include <iostream>
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
#include <netdb.h> 
#include <time.h>
#include <pthread.h>
#include <ctime> 
#include <deque>
#include <chrono>
#include <set>
#include <random>
#include <map>
#include <mutex>          // std::mutex

using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point Timestruct;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mis;
typedef std::chrono::duration<float> fsec;

#define BUFSIZE 10024
#define  h_addr h_addr_list[0]
#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())

int buffersize = -1;
int packtlength = -1;
int packetgen = -1;
int windowsize = -1;
int maxpackets = -1;
int debugflag = -1;
deque< vector<char> > packque;
vector<float> trnsmtime(128);
vector<Timestruct> senttime(128);
int startwindow = -1;
int endwindow = -1;
int currpacket = -1;
int lastack = -1;
int packetcount = 0;
set<int> ispresent;
map<int, int> transattempt;
vector< vector<char> > packetseq(128);
int successtrans = 0;
auto t0 = Time::now();
float timeout = 0;
int dtime = 0;
float ftime = 100.5;
int totalsent= 0;
int totalacked = 0;
int yetanflag = -1;
mutex mtx;
mutex mtx2;
mutex mtx3;
mutex mt4;
/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

void *SenderWindow(void *threadarg) {
    int *sockfd = (int *)threadarg;
    char buf[BUFSIZE];
    currpacket = startwindow;
    //cout<<startwindow<<" "<<endwindow<<"\n";
    while(successtrans<maxpackets) {
        vector<char> ans;
        int ifgenover = -1;
        if(currpacket!=(endwindow+1)%128) {
            while(successtrans<maxpackets) {
                //mtx.lock();
                bool presentflag;
                mtx2.lock();
                presentflag = present(ispresent,currpacket);
                mtx2.unlock();
                if(presentflag) {
                    ans = packetseq[currpacket];
                    /*cout<<"Packet being sent is ";
                    for(int i=0;i<ans.size();i++) {
                        cout<<ans[i];
                    }
                    cout<<"\n";*/
                    transattempt[currpacket]++;
                    if(transattempt[currpacket]>5) {
                        yetanflag = 1;
                        cout<<currpacket<<"\n";
                        pthread_exit(NULL);
                    }
                    break;
                }
                else if(packque.empty()) {
                    continue;
                }
                else{
                    //cout<<"Size is: "<<packque.size()<<"\n";
                    mtx.lock();
                    if(!packque.empty())
                    {   
                        ans = packque.back();
                        /*cout<<"Packet being sent is ";
                        for(int i=0;i<ans.size();i++) {
                            cout<<ans[i];
                        }
                        cout<<"\n";*/
                        packque.pop_back();
                        packetseq[currpacket] = ans;
                        mtx2.lock();
                        ispresent.insert(currpacket);
                        mtx2.unlock();
                        transattempt[currpacket]++;
                    }
                    mtx.unlock();
                    break;
                }
                //mtx.unlock();
            }
            if(successtrans==maxpackets) {
                break;
            }
            bzero(buf, BUFSIZE);
            for(int i=0;i<ans.size();i++) {
                buf[i+1] = ans[i];
            }
            buf[0] = currpacket;
            //cout<<"Sequence Number is: "<<currpacket<<"\n";
            buf[ans.size()+1] = '\0';
            senttime[currpacket] = Time::now();
            int n = write(*sockfd, buf, strlen(buf));
            totalsent++;
            currpacket = (currpacket+1)%128;
            if (n < 0) 
            error("ERROR writing to socket");
        }
            bool presentflag;
            mtx2.lock();
            presentflag = present(ispresent,startwindow);
            presentflag = true;
            mtx2.unlock();
        if(presentflag) {   
            fsec fs = Time::now()-senttime[startwindow];
            if(successtrans<10) {
                ms d = std::chrono::duration_cast<ms>(fs);
                auto mstoDelay = std::chrono::milliseconds(100);
                if(fs.count()*1000>100) {   
                    mtx3.lock();             
                    startwindow = (lastack+1)%128;
                    currpacket = startwindow;
                    endwindow = (startwindow+windowsize+127)%128;
                    mtx3.unlock();
                }   
            }
            else {
                ms d = std::chrono::duration_cast<ms>(fs);
                int delay = dtime+(2*timeout)/successtrans;
                auto mstoDelay = std::chrono::milliseconds(delay);
                if(fs.count()*1000>(dtime+(2*timeout)/successtrans+ftime)) {
                    mtx3.lock();
                    startwindow = (lastack+1)%128;
                    currpacket = startwindow;
                    endwindow = (startwindow+windowsize+127)%128;
                    mtx3.unlock();
                }   
            }  
        }

    }
    pthread_exit(NULL);
}
void *RecieverWindow(void *threadarg) {
    
    int *sockfd = (int *)threadarg;
    char buf[BUFSIZE];
    while(successtrans<maxpackets  && yetanflag == -1) {
        bzero(buf, BUFSIZE);
        int n = read(*sockfd, buf, BUFSIZE);
        if (n < 0) {
            cout<<"successtrans is "<<successtrans<<"\n";
             error("ERROR reading from socket");
        }
        int ackwnt = buf[0];
        //cout<<"Ack wanted recieved from server is "<<ackwnt<<"\n";
        int pctsent = (ackwnt+255)%128;
        int flag = -1;
        if(startwindow<=endwindow) {
            if(pctsent<=endwindow && pctsent >= startwindow) {
                flag = 1;
            }
        }
        else {
            if(pctsent<=endwindow || pctsent >= startwindow) {
                flag = 1;
            }
        }
        if(flag==1) {
            //mtx.lock();
            bool presentflag;
            mtx2.lock();
            presentflag = present(ispresent,pctsent);
            mtx2.unlock();
            if(presentflag) {
                fsec fs = Time::now() - senttime[pctsent];
                float val = fs.count()*1000;
                if((successtrans<10 && val<=100)||(successtrans>=10 && val <= dtime+2*timeout/successtrans+ftime)) {
                    mtx3.lock();
                    trnsmtime[pctsent] = fs.count()*1000;
                    fsec fs2 = Time::now() - t0;
                    int minis = std::chrono::duration_cast<ms>(fs2).count(); int micros = std::chrono::duration_cast<mis>(fs2).count();
                    micros = micros%1000;
                    timeout += trnsmtime[pctsent];
                    if(debugflag==1) {
                        cout<<"Seq #: "<<pctsent<<"\tTime Generated: "<<minis<<":"<<micros<<"\tRTT: ";
                        cout<<trnsmtime[pctsent]<<"\tNumber of attempts: "<<transattempt[pctsent];//<<"\tData was: ";
                        /*REP(j,0,4) {
                            cout<<packetseq[pctsent][j];
                        }*/
                        cout<<"\n";
                    }
                    lastack = pctsent;
                    successtrans++;
                    totalacked++;
                    startwindow = (startwindow+1)%128;
                    endwindow = (endwindow+1)%128;
                    mtx3.unlock();
                    transattempt[pctsent] = 0;
                    mtx2.lock();
                    ispresent.erase(pctsent);
                    mtx2.unlock();
                }
                else {
                    //cout<<startwindow<<":"<<currpacket<<":"<<endwindow<<"\n";
                    mtx3.lock();
                    startwindow = pctsent;
                    currpacket = startwindow;
                    endwindow = (endwindow+127+windowsize)%128;
                    mtx3.unlock();
                }
                //printf("Answer from server: %d\n", buf[0]);
            }
            //mtx.unlock();
        }    
    }
    pthread_exit(NULL);
}
void *PacketgenThread(void *threadid) {
   long tid;
   tid = (long)threadid;
   //cout << "Hello World! Thread ID, " << threadid << endl;
   auto t0 = Time::now();
    auto t1 = Time::now();
    int secondsToDelay = 1000/packetgen;
    auto mstoDelay = std::chrono::milliseconds(secondsToDelay);
    bool exit = false;
    // generate random seed using time 
    srand((unsigned)time(0)); 
    packetcount = 0;
    int flag = 1;
    while(packetcount<maxpackets) {
        // get delta time in miliseconds
        while(packque.size()>=buffersize) {
            ;
        }
        fsec fs = Time::now() - t1;
        ms d = std::chrono::duration_cast<ms>(fs);
        if(d > mstoDelay ){
            // generate new packet
            vector<char> packetinstance(packtlength);
            for(int j=0;j<packtlength;j++) {
                packetinstance[j] = rand()%26+65;  
            } 
            mtx.lock();
            packque.push_front(packetinstance);
            mtx.unlock();
            /*cout<<"New packet : ";
            for(int j=0;j<packtlength;j++) {
                cout<<packetinstance[j]<<" ";  
            }  
            cout<<"\n";*/
            packetcount++;
            //reset the clock timers
            t1 = Time::now();
        }
    }
    fsec fs = Time::now() - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    pthread_exit(NULL);
}
int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    int i;
    /* check command line arguments */
    string recievername("default");
    debugflag = -1;
    maxpackets = -1;
    packtlength = -1;
    packetgen = -1;
    windowsize = -1;
    buffersize = -1;
    int recieverport = -1;
    portno = -1;
    for(int j=1;j<argc;j++) {
        if(strcmp(argv[j],"-d") == 0) {
          debugflag = 1;
        }
        else if(strcmp(argv[j],"-p") == 0) {
            if(j+1==argc) break;
            portno = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-n") == 0) {
            if(j+1==argc) break;
            maxpackets = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-s") == 0) {
            if(j+1==argc) break;
            recievername = argv[++j];
        }
        else if(strcmp(argv[j],"-l") == 0) {
            if(j+1==argc) break;
            packtlength = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-r") == 0) {
            if(j+1==argc) break;
            packetgen = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-w") == 0) {
            if(j+1==argc) break;
            windowsize = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-b") == 0) {
            if(j+1==argc) break;
            buffersize = atoi(argv[++j]);
        }
    }
    if(portno == -1 || maxpackets == -1 || packtlength == -1 || packetgen == -1 || windowsize == -1 || buffersize == -1 || recievername == "default") {
        cout<<"Enter the requiered fields\n";
        exit(0);
    }
    pthread_t packets;
    int rc = pthread_create(&packets, NULL, PacketgenThread, (void *)1);
        
    if (rc){
       cout << "Error:unable to create thread3," << rc << endl;
       exit(-1);
    }
    hostname = new char[recievername.length() + 1]; 
    strcpy(hostname,recievername.c_str());

    /* socket: create the socket */
    /* connect: create a connection with the server */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) 
          error("ERROR connecting");

    /*printf("Server address is: ");
    for(i=0;i<server->h_length;i++) {
        printf("%d",((char*)(&serveraddr.sin_addr.s_addr))[i]);
        if(i!=server->h_length-1)printf(".");
    }
    printf("\nport is : %d\n",portno);*/
    /* get message line from the user */
    startwindow = 0;
    endwindow = windowsize-1;
    lastack = 127;
    ispresent.clear();
    REP(i,0,127) {
        transattempt[i] = 0;
    }
    t0 = Time::now();
    pthread_t reciever;
    pthread_attr_t attr;
    void *status;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    rc = pthread_create(&reciever, &attr, RecieverWindow, (void *)(&sockfd));
    if (rc){
       cout << "Error:unable to create thread1," << rc << endl;
       exit(-1);
    }
    while(successtrans<maxpackets && yetanflag == -1) {
        
        pthread_t sender;
        int rc = pthread_create(&sender, &attr, SenderWindow, (void *)(&sockfd));
        if (rc){
            cout<<successtrans<<"\n";
           cout << "Error:unable to create thread2," << rc << endl;
           exit(-1);
        }
        rc = pthread_join(sender, &status);
        if (rc){
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
        }
        break;
        /*printf("Enter command: ");
        fgets (buf, BUFSIZE-1, stdin);
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
          error("ERROR writing to socket");

        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("Answer from server: %s\n", buf);*/
    }
    sleep(1);
    cout<<"1. PACKET_GEN_RATE: "<<packetgen<<"\n"<<"2. PACKET_LENGTH: "<<packtlength<<"\n";
    cout<<"3. Retransmission Ratio: Ratio of Total Number of Transmissions (including Retransmissions) to Number of Packets Acknowledged: ";
    cout<<(totalsent*1.0)/totalacked<<"\n4. Average RTT Value for ALL Acknowledged Packets: "<<timeout/successtrans<<"\n";
    close(sockfd);
    return 0;
}