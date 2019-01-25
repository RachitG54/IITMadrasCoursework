/* 
 * cw.cpp - CS14B050
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
 #include <fstream>

using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point Timestruct;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mis;
typedef std::chrono::duration<float> fsec;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;

#define BUFSIZE 10024
#define  h_addr h_addr_list[0]
#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())


/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

   

int main(int argc, char **argv) {
    /* check command line arguments */
    double Ki = -1;
    double Km = -1;
    double Kn = -1;
    double Kf = -1;
    double Ps = -1;
    int totsegments = -1;
    char *outfile = NULL;
    for(int j=1;j<argc;j++) {
        if(strcmp(argv[j],"-i") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&Ki);
        }
        else if(strcmp(argv[j],"-m") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&Km);
        }
        else if(strcmp(argv[j],"-n") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&Kn);
        }
        else if(strcmp(argv[j],"-f") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&Kf);
        }
        else if(strcmp(argv[j],"-s") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&Ps);
        }
        else if(strcmp(argv[j],"-T") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%d",&totsegments);
        }
        else if(strcmp(argv[j],"-o") == 0) {
            if(j+1==argc) break;
            outfile = argv[++j];
        }
    }
    if(Ki == -1 || Km == -1 || Kn == -1 || Kf == -1|| Ps == -1|| totsegments == -1 || outfile == NULL) {
        cout<<"Enter the requiered fields\n";
        exit(0);
    }
    Ps = 1 - Ps;
    int MSS = 1;
    double RWS = 1024;
    double cwval = Ki * MSS;
    double ceilN = ceil(cwval/MSS); 
    double threshhold = RWS/2;
    int N = (int)ceilN;
    int updatenumber = 1;
    int currsegments = 0;
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);
    ofstream myfile;
    myfile.open (outfile,ios::out);
    while(currsegments<totsegments) {
        ceilN = ceil(cwval/MSS);
        N = (int)ceilN;
        REP(i,0,N) {
            double p = dist(mt);
            if(p<Ps) {
                if(cwval<threshhold) {
                    cwval = min(RWS,cwval+Km*MSS);
                }
                else {
                    cwval = min(RWS,(cwval+Kn*MSS*MSS/cwval));
                }
                myfile<<updatenumber<<","<<cwval<<"\n";
                currsegments += 1;
                updatenumber += 1;
            }
            else {
                threshhold = cwval/2;
                cwval = max((double)1,Kf*cwval);
                currsegments += 1;
                myfile<<updatenumber<<","<<cwval<<"\n";
                updatenumber += 1;
            }
            if(currsegments>totsegments) {
                myfile.close();
                return 0;
            }
        }    
    }
    myfile.close();
   return 0;
}
