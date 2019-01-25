/* 
 * SlottedAloha.cpp - CS14B050
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
    int maxpackets = -1;
    double packetgen = -1;
    int windowsize = 2;
    int numberofusers = -1;
    int gout = -1;
    for(int j=1;j<argc;j++) {
        if(strcmp(argv[j],"-p") == 0) {
            if(j+1==argc) break;
            sscanf(argv[++j],"%lf",&packetgen);
            //cout<<packetgen<<"\n";
        }
        else if(strcmp(argv[j],"-N") == 0) {
            if(j+1==argc) break;
            numberofusers = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-W") == 0) {
            if(j+1==argc) break;
            windowsize = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-M") == 0) {
            if(j+1==argc) break;
            maxpackets = atoi(argv[++j]);
        }
        else if(strcmp(argv[j],"-G") == 0) {
            gout = 1;
        }
    }
    if(numberofusers == -1 || maxpackets == -1 || packetgen == -1 || windowsize == -1) {
        cout<<"Enter the requiered fields\n";
        exit(0);
    }
    ////cout<<"Users "<<numberofusers<<"maxpackets is "<<maxpackets<<"packetgen is "<<packetgen<<"windowsize is"<<windowsize<<"\n";  
    int SimTime = 0;
    int N = numberofusers;
    vi dynamwindowsize(N,windowsize);
    vector< deque<ii> > buffer(N);
    vector< deque<int> > gensimtime(N);
    vi trieuser(N,0);
    random_device rd;
    mt19937 mt(rd());
    int packno = 0;
    uniform_real_distribution<double> dist(0.0, 1.0);
    int success = 0;
    float sumdelay = 0;
    int flag = 1;
    while(success<maxpackets) {
    	int tries = 0;
    	REP(i,0,N-1) {
			double p = dist(mt);
			if(p<packetgen) {
				if(buffer[i].size()<2) {
					ii temp;
					temp.first = packno++;
					temp.second = -1;
					buffer[i].pb(temp);
					gensimtime[i].pb(SimTime);
				}
			}
    	}
    	int nodetrying = -1;
    	REP(i,0,N-1) {
			if(!buffer[i].empty()) {
				ii temp = buffer[i].front();
				if (temp.second == -1 || temp.second == 0) {
					nodetrying = i;
					tries++;
				}
			}
    	}
    	if(tries>1) {
    		//cout<<"Collision\n";
    		////cout<<SimTime<<"\n";
    		srand(time(0));
    		REP(i,0,N-1) {
				if(!buffer[i].empty()) {
					ii temp = buffer[i].front();
					if (temp.second == -1 || temp.second == 0) {
						buffer[i].front().second = rand()%dynamwindowsize[i];
						dynamwindowsize[i] = min(256,(int)(dynamwindowsize[nodetrying]*(2)));
                        trieuser[i]++;
                        if(trieuser[i]==10000) {
                            flag = -1;
                            break;
                        }
					}
					else {
						buffer[i].front().second--;
					}
				}
	    	}
            if(flag==-1) {
                break;
            }
    	}
    	else if(tries==1) {
    		//cout<<"Packetsuccess\n";
    		success++;
            //cout<<nodetrying<<"\n";
    		sumdelay += SimTime - gensimtime[nodetrying].front();
    		buffer[nodetrying].pop_front();
    		dynamwindowsize[nodetrying] = max(2,(int)(dynamwindowsize[nodetrying]*(0.75)));
    		gensimtime[nodetrying].pop_front();
            trieuser[nodetrying] = 0;
    		REP(i,0,N-1) {
				if(!buffer[i].empty() && buffer[i].front().second >0) {
					buffer[i].front().second--;
				}
    		}
    	}
        else {
            //cout<<"No one trying\n";
            REP(i,0,N-1) {
                if(!buffer[i].empty() && buffer[i].front().second >0) {
                    buffer[i].front().second--;
                }
            }   
        }
    	SimTime++;
        REP(i,0,N-1) {
            ////cout<<dynamwindowsize[i]<<" ";
            if(!buffer[i].empty()) {
                ii temp = buffer[i].front();
                //cout<<i<<":"<<temp.second<<" ";
            }
        }
        //cout<<"\n";
        REP(i,0,N-1) {
            ////cout<<dynamwindowsize[i]<<" ";
            if(buffer[i].size()>1) {
                ii temp = buffer[i].back();
                //cout<<i<<":"<<temp.second<<" ";
            }
        }
        //cout<<"\n";
        //sleep(1);
    }
    if(gout==-1) {
        cout<<"Number of nodes are "<<numberofusers<<"\nWindow size for each node is:\n";
        REP(i,0,N-1) {
            cout<<dynamwindowsize[i]<<" ";
        }
        cout<<"\nProbability of packet generation is "<<packetgen<<"\n";
        cout<<"Mean delay per packet is "<<sumdelay/success<<"\n";
        cout<<"Average utilization per slot (throughput) is "<<success*1.0/SimTime<<"\n";
        cout<<"Total Simulation Time is "<<SimTime<<"\n";    
    }
    else {
        cout<<success*100.0/SimTime;
    }
   return 0;
}
