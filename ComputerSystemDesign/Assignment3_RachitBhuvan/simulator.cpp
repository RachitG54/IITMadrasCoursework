#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
#include <bitset>
#include <cstring>
#include <cstdio>
#include <string>

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


unsigned short int progmem[512];
short int registers[16];

enum opcodes {
  ADD = 0,
  ADD_I = 1,
  SUB = 2,
  SUB_I = 3,
  MUL = 6,
  MUL_I = 7,
  LD = 8,
  SD = 10,
  JMP = 12,
  BEQZ = 13,
  HLT = 14,
};

map<string, unsigned short int> opcode;
map<string, int> labelmap;
short int chartono(char *s) {
  string empty = "";
  short int val;
  bool flag = false;
  REP(i,0,strlen(s)-1) {
    if(flag==true) {
      empty = empty + s[i];
    }
    if(s[i]=='-') flag = true;
  }
  if(flag) {
    val = (short int)-atoi(empty.c_str());
  }
  else {
    val = (short int)atoi(s);
  }
  return val;
}
void addregister(const char* reg, unsigned short int &no, int pos) {
  short int regno;
  sscanf(reg,"R%hd",&regno);
  no += (regno<<pos);
}
void addregisterorno(const char* reg, unsigned short int &no, int pos) {
  short int regno;
  if(reg[0]=='R') {
    sscanf(reg,"R%hd",&regno);
    no += (regno<<pos);
  }
  else {
    short int val;
    char s[10];
    sscanf(reg,"#%s",s);
    val = chartono(s);
    bitset<4> bin_x(val);
    no += 1<<12;
    no += bin_x.to_ulong();
  }
}
void addlabels(const char* label, unsigned short int &no, int pos, int PC) {
  short int labelno = (labelmap[string(label)]-PC)*2;   
  bitset<8> bin_x(labelno);
  no += (bin_x.to_ulong()<<pos);
}
void addmemloc(const char* memloc, unsigned short int &no, int pos) {
  short int memlocno;
  short int memvalno;
  sscanf(memloc,"R%hd[R%hd]",&memlocno, &memvalno);
  no += (memlocno<<pos);
  no += (memvalno<<(pos-4));
}

short int four2sixteen(short int x) {
  if(x>7) {
    return x-16;
  }
  return x;
}


void simulate() {
  int PC = 0;
  bool done = false;
  while (!done) {
    unsigned short int contents = progmem[PC];
    int opcode = (contents >> 12);
    short int R1, R2, R3;
    R1 = ((contents & 0xf00) >> 8);
    R2 = ((contents & 0xf0) >> 4);
    R3 = (contents & 0xf);
    short int x = 0;
    char jmplabel;
    switch(opcode) {
      case ADD:
        registers[R1] = registers[R2] + registers[R3];
        PC++;
        break;
      case ADD_I:
        registers[R1] = registers[R2] + four2sixteen(R3);
        PC++;
        break;
      case SUB:
        registers[R1] = registers[R2] - registers[R3];
        PC++;
        break;
      case SUB_I:
        registers[R1] = registers[R2] - four2sixteen(R3);
        PC++;
        break;
      case MUL:
        registers[R1] = registers[R2] * registers[R3];
        PC++;
        break;
      case MUL_I:
        registers[R1] = registers[R2] * four2sixteen(R3);
        PC++;
        break;
      case LD:
        // cout << "Loading from " << (registers[R2] + registers[R3])/2 << "\n";
        registers[R1] = progmem[(registers[R2] + registers[R3])/2];
        PC++;
        break;
      case SD:
        progmem[(registers[R1] + registers[R2])/2] = registers[R3];
        PC++;
        break;
      case JMP:
        x = (R1<<4) + R2;
        jmplabel = x;
        PC = PC + jmplabel/2;
        break;
      case BEQZ:
        if (registers[R1] == 0) {
          x = (R2<<4) + R3;
          jmplabel = x;
          PC = PC + jmplabel/2;
        }
        else {
          PC++;
        }
        break;
      case HLT:
        done = true;
        break;
    }
  }
}

void print_memory() {
  for (int i = 0; i < 512; i++) {
    int mem = 2*i;
    int contents = (progmem[i]&(0xffff));
    bitset<16> bin_x(contents);
    string binary = bin_x.to_string();
   
    printf("%04d : %s %s\n", mem, binary.substr(0, 4).c_str(), binary.substr(4, 4).c_str());
    
    mem += 1;
    printf("%04d : %s %s\n", mem, binary.substr(8, 4).c_str(), binary.substr(12, 4).c_str());

  }
}

int main() {
  opcode["ADD"] = 0;
  opcode["SUB"] = 2;
  opcode["MUL"] = 6;
  opcode["ADD_I"] = 1;
  opcode["SUB_I"] = 3;
  opcode["MUL_I"] = 7;
  opcode["LD"] = 8;
  opcode["SD"] = 10;
  opcode["JMP"] = 12;
  opcode["BEQZ"] = 13;
  opcode["HLT"] = 14;
  
  registers[0] = 0;
  
  vector<string> reg_init;
  vector<string> mem_init;
  vector<string> code;
  
  memset(progmem, 0, sizeof(progmem));
  
  string buffer;
  while(getline(cin, buffer)) {
    if (buffer[0] == '#') {
      continue;
    }
    if (buffer[0] == '$') {
      if (buffer[1] == 'R') {
        reg_init.pb(buffer);
      }
      else {
        mem_init.pb(buffer);
      }
    }
    else {
      code.pb(buffer);
    }
  }
  
  for(int i = 0;i<reg_init.size();i++) {
    istringstream iss(reg_init[i]);
    string instruct;
    iss >> instruct;
    short int regno,val;
    char s[10];
    sscanf(instruct.c_str(),"$R%hd$%s",&regno,s);
    string empty = "";
    bool flag = false;
    REP(i,0,strlen(s)-1) {
      if(flag==true) {
        empty = empty + s[i];
      }
      if(s[i]=='-') flag = true;
    }
    if(flag) {
      val = (short int)-atoi(empty.c_str());
    }
    else {
      val = (short int)atoi(s);
    }
    registers[regno] = val;
  } 

  for(int i = 0;i<mem_init.size();i++) {
    istringstream iss(mem_init[i]);
    string instruct;
    iss >> instruct;
    replace(all(instruct), '$', ' ');
    istringstream itokens(instruct);
    int memplace,val;
    string s;
    itokens>>memplace>>s;
    string empty = "";
    bool flag = false;
    REP(i,0,s.size()-1) {
      if(flag==true) {
        empty = empty + s[i];
      }
      if(s[i]=='-') flag = true;
    }
    if(flag) {
      val = -atoi(empty.c_str());
    }
    else {
      val = atoi(s.c_str());
    }
    progmem[memplace/2] = val;
  } 
  int PC = 0;
  for( ;PC < code.size();PC++) {
    istringstream iss(code[PC]);
    string instruct;
    iss >> instruct;
    if(instruct[instruct.size()-1] == ':') {
      labelmap[instruct.substr(0,instruct.size()-1)] = PC;
      code[PC] = iss.str().substr(instruct.size()+1,code[PC].size()-instruct.size()-1);
    }
  }
  for( PC = 0;PC < code.size();PC++) {
    istringstream iss(code[PC]);
    string instruct;
    iss >> instruct;
    progmem[PC] = (opcode[instruct]<<12);
    
    if(instruct == "ADD" || instruct == "SUB" || instruct == "MUL") {
      string reg;
      iss >> reg;
      addregister(reg.c_str(),progmem[PC],8);
      iss >> reg;
      addregister(reg.c_str(),progmem[PC],4);
      iss >> reg;
      addregisterorno(reg.c_str(),progmem[PC],0);
    }
    else if(instruct == "LD" || instruct == "SD") {
      if(instruct == "LD") {
        string reg;
        iss >> reg;
        addregister(reg.c_str(),progmem[PC],8);
        iss >> reg;
        addmemloc(reg.c_str(),progmem[PC],4);
      }
      else if(instruct == "SD") {
        string reg;
        iss >> reg;
        addmemloc(reg.c_str(),progmem[PC],8);
        iss >> reg;
        addregister(reg.c_str(),progmem[PC],0);
      }      
    }
    else if(instruct == "JMP" || instruct == "BEQZ") {
      if(instruct == "BEQZ") {
        string reg;
        iss >> reg;
        addregister(reg.substr(1,reg.size()-2).c_str(),progmem[PC],8);
        iss >> reg;
        addlabels(reg.c_str(),progmem[PC],0,PC);
      }
      else {
        string reg;
        iss >> reg;
        addlabels(reg.c_str(),progmem[PC],4,PC);
      }
    }
    else if(instruct == "HLT") {
      ;
    }
  }
  simulate();
  print_memory();  
}