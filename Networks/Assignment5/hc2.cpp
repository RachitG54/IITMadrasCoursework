#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <ctime> 
#include <fstream>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef long long ll;
typedef vector<ll> vlli;

#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)

void xorsetf(int currparset, vi &v,set<int> &powerof2) {
	int xorset = 0;
	int k = 0;int mod = 1;
	switch(currparset) {
		case 1: k = 1;mod = 2;break;
		case 2: k = 2;mod = 4;break;
		case 4: k = 4;mod = 8;break;
		case 8: k = 8;mod = 16;break;
		case 16: k = 16;mod = 32;break;
	}
	for(int i=1;i<=31;i++) {
		if(!present(powerof2,i)) {
			if(i%mod>=k) {
				xorset = xorset ^ v[i];
			}
		} 
	}
	v[currparset] = xorset;
}
int xorcheckf(int currparset, vi &v,set<int> &powerof2) {
	int xorset = 0;
	int k = 0;int mod = 1;
	switch(currparset) {
		case 1: k = 1;mod = 2;break;
		case 2: k = 2;mod = 4;break;
		case 4: k = 4;mod = 8;break;
		case 8: k = 8;mod = 16;break;
		case 16: k = 16;mod = 32;break;
	}
	for(int i=1;i<=31;i++) {
		if(!present(powerof2,i)) {
			if(i%mod>=k) {
				xorset = xorset ^ v[i];
			}
		} 
	}
	if((v[currparset] ^ xorset)!=0) {
		return 1;
	}
	else {
		return 0;
	}
}
int main(int argc,char *argv[]) {
	int T;
	//myfile2<<"Enter number of testcases\n";
	ofstream myfile2;
	ifstream myfile;
  	myfile2.open (argv[2], ios::out);
  	myfile.open(argv[1], ios::in);
	if(myfile==NULL||myfile2==NULL) {
		return 0;
	}
	string s;
	while (myfile>>s) {
		vi v(32,-1);
		set<int> powerof2;
		powerof2.insert(1);
		powerof2.insert(2);
		powerof2.insert(4);
		powerof2.insert(8);
		powerof2.insert(16);
		int i=0;
		for(int j=1;j<32;j++) {
			if(!present(powerof2,j)) {
				v[j] = s[i++]-'0'; 
			}
		}
		myfile2<<"Original String: "<<s<<"\n";
		/*for(int j=1;j<32;j++) {
			myfile2<<v[j];
		}
		myfile2<<"\n";*/
		xorsetf(1,v,powerof2);
		xorsetf(2,v,powerof2);
		xorsetf(4,v,powerof2);
		xorsetf(8,v,powerof2);
		xorsetf(16,v,powerof2);
		myfile2<<"Original String with Parity: ";
		for(int j=1;j<32;j++) {
			myfile2<<v[j];
		}
		myfile2<<"\n";
		srand((unsigned)time(0)); 
		int randompos = rand()%31+1;
		vi errorv = v;
		myfile2<<"Corrupted string: ";
		errorv[randompos] = (errorv[randompos]+1)%2;
		for(int j=1;j<32;j++) {
			myfile2<<errorv[j];
		}
		myfile2<<"\n";
		myfile2<<"Error Location(s) is/are: "<<randompos<<"\n";
		vi checkerror;
		int pos = 1;
		int errorpos = 0;
		for(int j=0;j<5;j++) {
			checkerror.pb(xorcheckf(pos,errorv,powerof2));
			pos *=2;
		}
		for(int j=4;j>=0;j--) {
			errorpos = 2*errorpos + checkerror[j];
		} 
		myfile2<<"Number of errors introduced: 1\n";
		myfile2<<"Error Location computed by receiver algorithm is: "<<errorpos<<"\n";
		myfile2<<"Introducing 10 2-bit errors\n";
		for(int n=0;n<10;n++) {
			myfile2<<"Original String: "<<s<<"\n";
			myfile2<<"Original String with Parity: ";
			for(int j=1;j<32;j++) {
				myfile2<<v[j];
			}
			myfile2<<"\n";
			int randompos1 = rand()%31+1;int randompos2 = rand()%31+1;
			vi errorv2 = v;
			myfile2<<"Corrupted string: ";
			errorv2[randompos1] = (errorv2[randompos1]+1)%2;errorv2[randompos2] = (errorv2[randompos2]+1)%2;
			for(int j=1;j<32;j++) {
				myfile2<<errorv2[j];
			}
			myfile2<<"\n";
			myfile2<<"Error Location(s) is/are: "<<randompos1<<" "<<randompos2<<"\n";
			vi checkerror;
			int pos = 1;
			int errorpos = 0;
			vi errorpos2;
			for(int j=0;j<5;j++) {
				checkerror.pb(xorcheckf(pos,errorv2,powerof2));
				if(checkerror[j]==1)  {
					errorpos2.pb(pos);
				}
				pos *=2;
			}
			for(int j=4;j>=0;j--) {
				errorpos = 2*errorpos + checkerror[j];
			} 
			myfile2<<"Number of errors introduced: 2\n";
			if(errorpos2.size()!=0) {
				myfile2<<"Error Detected (Yes/No): Yes\n";	
			}
			else {
				myfile2<<"Error Detected (Yes/No): No\n";	
			}
			myfile2<<"Parity bits that failed are at locations: ";
			for(int j=0;j<errorpos2.size();j++) {
				myfile2<<errorpos2[j]<<" ";
			}
			myfile2<<"\n";
		}
		myfile2<<"\n";
	}
	myfile.close();
	myfile2.close();
	return 0;
}