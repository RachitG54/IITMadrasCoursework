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

struct state_vars {
  int PC;
  int NPC, opcode, Cond;
  short int A, B, Imm, ALUOutput, R1;
  unsigned short int contents, LMD;
};


unsigned short int progmem[512];
short int registers[16];
bool registerstall[16];
bool stageflush[5];
bool decodestall;

short int forwarded[16];
int register_in_use[16];

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

void fetch(unsigned short int &contents, int &NPC, int PC) {
  contents = progmem[PC];         //fetch
  NPC = PC + 1;
}

void decode(unsigned short int contents, int &opcode, short int &A, short int &B, short int &Imm, short int &R1) {
  opcode = (contents >> 12);                      //decode
  short int R2,R3;
  R1 = ((contents & 0xf00) >> 8);
  R2 = ((contents & 0xf0) >> 4);
  R3 = (contents & 0xf);
  switch(opcode) {
    case ADD:
    case SUB:
    case MUL:
      if(registerstall[R1] || registerstall[R2] || registerstall[R3]) {
        decodestall = true;
        return;
      }
      if (register_in_use[R1] == 0) {
        A = registers[R1];
      }
      else {
        A = forwarded[R1];
      }
      if (register_in_use[R2] == 0) {
        B = registers[R2];
      }
      else {
        B = forwarded[R2];
      }
      if (register_in_use[R3] == 0) {
        Imm = registers[R3];
      }
      else {
        Imm = forwarded[R3];
      }
      register_in_use[R1]++;
      break;  
    case LD:
      if(registerstall[R1] || registerstall[R2] || registerstall[R3]) {
        decodestall = true;
        return;
      }
      if (register_in_use[R1] == 0) {
        A = registers[R1];
      }
      else {
        A = forwarded[R1];
      }
      if (register_in_use[R2] == 0) {
        B = registers[R2];
      }
      else {
        B = forwarded[R2];
      }
      if (register_in_use[R3] == 0) {
        Imm = registers[R3];
      }
      else {
        Imm = forwarded[R3];
      }
      registerstall[R1] = true;
      break;
    case SD:
      if(registerstall[R1] || registerstall[R2] || registerstall[R3]) {
        decodestall = true;
        return;
      }
      if (register_in_use[R1] == 0) {
        A = registers[R1];
      }
      else {
        A = forwarded[R1];
      }
      if (register_in_use[R2] == 0) {
        B = registers[R2];
      }
      else {
        B = forwarded[R2];
      }
      if (register_in_use[R3] == 0) {
        Imm = registers[R3];
      }
      else {
        Imm = forwarded[R3];
      }
      break;
    case ADD_I:
    case SUB_I:
    case MUL_I:
      if(registerstall[R1] || registerstall[R2]) {
        decodestall = true;
        return;
      }
      if (register_in_use[R1] == 0) {
        A = registers[R1];
      }
      else {
        A = forwarded[R1];
      }
      if (register_in_use[R2] == 0) {
        B = registers[R2];
      }
      else {
        B = forwarded[R2];
      }
      register_in_use[R1]++;
      Imm = four2sixteen(R3);
      break;
    case JMP:
      A = R1; B = R2;
      break;
    case BEQZ:
      if(registerstall[R1]) {
        decodestall = true;
        return;
      }
      if (register_in_use[R1] == 0) {
        A = registers[R1];
      }
      else {
        A = forwarded[R1];
      }
      B = R2; Imm = R3;
      break;
  }
}

void execute(short int &ALUOutput, int &Cond, short int A, short int B, short int Imm, int opcode, int PC, int R1) {
  // If memory reference
  short int x;
  switch(opcode) {
    case LD:
      ALUOutput = (B + Imm)/2;
      break;
    case SD:
      ALUOutput = (A + B)/2;
      break;
    case ADD:
      ALUOutput = B + Imm;
      forwarded[R1] = ALUOutput;
      break;
    case SUB:
      ALUOutput = B - Imm;
      forwarded[R1] = ALUOutput;
      break;
    case MUL:
      ALUOutput = B * Imm;
      forwarded[R1] = ALUOutput;
      break;
    case ADD_I:
      ALUOutput = B + Imm;
      forwarded[R1] = ALUOutput;
      break;
    case SUB_I:
      ALUOutput = B - Imm;
      forwarded[R1] = ALUOutput;
      break;
    case MUL_I:
      ALUOutput = B * Imm;
      forwarded[R1] = ALUOutput;
      break;
    case JMP:
      x = (A << 4) + B;
      ALUOutput = PC + ((char)x)/2;
      break;
    case BEQZ:
      x = (B << 4) + Imm;
      ALUOutput = PC + ((char)x)/2;
      Cond = (A == 0);
      break;
  }
}

void memory(int &PC, unsigned short int &LMD, int NPC, short int ALUOutput, int opcode, int Cond, short int Imm) {
  //PC = NPC;
  switch(opcode) {
    case LD:
      LMD = progmem[ALUOutput];
      break;
    case SD:
      progmem[ALUOutput] = Imm;
      break;
    case JMP:
      //PC = ALUOutput;
      stageflush[0] = stageflush[1] = stageflush[2] = true;
      break;
    case BEQZ:
      if (Cond) {
        stageflush[0] = stageflush[1] = stageflush[2] = true;
        //PC = ALUOutput;
      }
    break;
  }
  
}

bool write_back(short int ALUOutput, int opcode, short int R1, unsigned short int LMD) {
  switch(opcode) {
    case ADD:
    case SUB:
    case MUL:
    case ADD_I:
    case SUB_I:
    case MUL_I:
      registers[R1] = ALUOutput;
      register_in_use[R1]--;
      break;  
    case LD:
      registers[R1] = LMD;
      registerstall[R1] = false;
      break;
  }
  if (opcode == HLT)
    return true;
  return false;
}

void simulate_pipeline_real() {
  state_vars buf[5];
  bool done = false;
  REP(i,0,15) {
    registerstall[i] = false;
  }
  REP(i,0,4) {
    stageflush[i] = false;
  }
  decodestall = false;
  
  bool available[5];
  for (int i = 1; i < 5; i++) available[i] = false;
  available[0] = true;
  buf[0].PC = 0;
  while (!done) {
    if (available[0])
      fetch(buf[0].contents, buf[0].NPC, buf[0].PC);
    if (available[2])
      execute(buf[2].ALUOutput, buf[2].Cond, buf[2].A, buf[2].B, buf[2].Imm, buf[2].opcode, buf[2].PC, buf[2].R1);
    if (available[1])
      decode(buf[1].contents, buf[1].opcode, buf[1].A, buf[1].B, buf[1].Imm, buf[1].R1);
    if (available[3])
      memory(buf[3].PC, buf[3].LMD, buf[3].NPC, buf[3].ALUOutput, buf[3].opcode, buf[3].Cond, buf[3].Imm);
    if (available[4])
      done = write_back(buf[4].ALUOutput, buf[4].opcode, buf[4].R1, buf[4].LMD);

    
    
    REP(i,0,2) {
      if(stageflush[i]) {
        available[i] = false;
        REP(j,0,15) {
          registerstall[j] = false;
          register_in_use[j] = 0;
        }
      }
    }
    bool flagstall = false;
    int PC = buf[0].NPC;
    for (int i = 4; i >= 1; i--) {
      if (available[i-1]) {
        if(i==1 && decodestall) {
          decodestall = false;
          available[1] = true;
          buf[0].PC = PC-1;
          flagstall = true;
        }
        else if(i==2 && decodestall) {
          available[2] = false;
        }
        else {
          buf[i] = buf[i-1];
          available[i] = true;
        }
      }
      else {
        if(i==1 && decodestall) {
          decodestall = false;
          available[1] = true;
        }
        else {
          available[i] = false;
        }
      }
    }

    REP(i,0,2) {
      if(stageflush[i]) {
        available[i] = false;
      }
    }

    available[0] = true;
    if(!flagstall) buf[0].PC = PC;
    REP(i,0,2) {
      if(stageflush[i]) {
        stageflush[i] = false;
        buf[0].PC = buf[3].ALUOutput;
      }
    }
    
  }
}

void simulate_pipeline() {
  int PC = 0;
  int NPC, opcode, Cond;
  short int A, B, Imm, ALUOutput, R1;
  unsigned short int contents, LMD;
  bool done = false;
  while (!done) {
    fetch(contents, NPC, PC);
    decode(contents, opcode, A, B, Imm, R1);
    execute(ALUOutput, Cond,  A, B, Imm, opcode, PC, R1);
    memory(PC, LMD, NPC, ALUOutput, opcode, Cond, Imm);
    done = write_back(ALUOutput, opcode, R1, LMD);
  }
  
}

void simulate() {
  int PC = 0;
  bool done = false;
  while (!done) {
    unsigned short int contents = progmem[PC];          //fetch
    PC++;
    int opcode = (contents >> 12);                      //decode
    short int R1, R2, R3;
    R1 = ((contents & 0xf00) >> 8);
    R2 = ((contents & 0xf0) >> 4);
    R3 = (contents & 0xf);
    short int x = 0;
    char jmplabel;
    switch(opcode) {
      case ADD:
        registers[R1] = registers[R2] + registers[R3];      //execute and WB
        break;
      case ADD_I:
        registers[R1] = registers[R2] + four2sixteen(R3);   //execute and WB
        break;
      case SUB:
        registers[R1] = registers[R2] - registers[R3];      //execute and WB
        break;
      case SUB_I:
        registers[R1] = registers[R2] - four2sixteen(R3);   //execute and WB
        break;
      case MUL:
        registers[R1] = registers[R2] * registers[R3];      //execute and WB
        break;
      case MUL_I:
        registers[R1] = registers[R2] * four2sixteen(R3);   //execute and WB
        break;
      case LD:
        // cout << "Loading from " << (registers[R2] + registers[R3])/2 << "\n";
        registers[R1] = progmem[(registers[R2] + registers[R3])/2];         //memaccess and execute
        break;
      case SD:
        progmem[(registers[R1] + registers[R2])/2] = registers[R3];         //execute and memaccess and WB 
        break;
      case JMP:
        x = (R1<<4) + R2;
        jmplabel = x;
        PC = PC + jmplabel/2 - 1;
        break;
      case BEQZ:
        if (registers[R1] == 0) {
          x = (R2<<4) + R3;
          jmplabel = x;
          PC = PC + jmplabel/2 - 1;
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
  simulate_pipeline_real();
  print_memory();  
}
