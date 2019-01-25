#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <bitset>
#include <random>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <openssl/dh.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tcpserver.h"

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef long long ll;
typedef vector<ll> vlli;
typedef unsigned char uchar;

#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)

#define B 5					//Bucket Size
#define S 5					//S records

#define tau 256
#define n(x) 1*x

typedef struct record {
	int label;
	int value;
}record;

uchar* genrandomkey(int len) {

    len = tau/8;
    uchar* key = (uchar*)malloc(sizeof(uchar)*len);
    int rc = RAND_bytes(key, len);
    //unsigned long err = ERR_get_error();

    if(rc != 1) {
        exit(1);
        /* RAND_bytes failed */
        /* `err` is valid    */
    }

    return key;
}
void initializing (vector< set<int> > &Free) {
    for(int i = 0 ; i < B; i++) {
        for(int j = 0; j<S; j++) {
            Free[i].insert(i+1);
        }
    }
}

void handleErrors()
{
  ERR_print_errors_fp(stderr);
  abort();
}

unsigned char* PRF(const unsigned char* seed, int sizeofseed, const char* input, unsigned int &len) {
    unsigned char* result;
    result = (unsigned char*)malloc(sizeof(char) * len);
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
 
    // You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
    HMAC_Init_ex(&ctx, seed, sizeofseed, EVP_sha256(), NULL);
    HMAC_Update(&ctx, (const unsigned char*)input, strlen(input));
    HMAC_Final(&ctx, result, &len);
    HMAC_CTX_cleanup(&ctx);
 
    printf("HMAC digest: ");
 
    for (unsigned int i = 0; i != len; i++)
        printf("%02x", (unsigned int)result[i]);
    printf("\n");
    return result;
}

BIGNUM* PRF_p(const unsigned char* seed, int sizeofseed, const char* input, unsigned int &len,BIGNUM* p) {
    unsigned char* result;
    result = (unsigned char*)malloc(sizeof(char) * len);
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
 
    // You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
    HMAC_Init_ex(&ctx, seed, sizeofseed, EVP_sha256(), NULL);
    HMAC_Update(&ctx, (const unsigned char*)input, strlen(input));
    HMAC_Final(&ctx, result, &len);
    HMAC_CTX_cleanup(&ctx);
 

    BIGNUM *a = BN_bin2bn(result, len, NULL);
    if(!a) {
        handleErrors();
    }
    BIGNUM *rem = BN_new();
    BN_CTX *ctxbn;
    ctxbn = BN_CTX_new();
    if(!ctxbn) {
        handleErrors();
    }

    if(BN_mod(rem, a, (const BIGNUM *)p, ctxbn)!=1) {
        handleErrors();
    }

    printf("HMAC digest modulo p: \n");
    printf("HMAC digest: ");

    BN_print_fp(stdout,rem);
    printf("\n");
    BN_free(a);
    BN_CTX_free(ctxbn);                       //see start, end ctx
    return rem;
}
BIGNUM* generatenosmodp1(BN_CTX *ctxbn, DH* dhDB) {
    int count = 0;
    while(1) {
        count++;
        int length = 2048;                                      //change, with prime, be careful
        uchar* rho = genrandomkey(length);
        BIGNUM *a = BN_bin2bn(rho, length, NULL);
        if(!a) {
            handleErrors();
        }
        BIGNUM *rem = BN_new();
        if(BN_mod(rem, a, (const BIGNUM *)dhDB->p, ctxbn)!=1) {
            handleErrors();
        }
        BN_free(a);
        BIGNUM *subtr2 = BN_new();
        BIGNUM *subtr = BN_new();
        BIGNUM *one = BN_new();
        BN_dec2bn(&one, "1");
        BN_sub(subtr, dhDB->p, one);
        //BN_sub(subtr2, subtr, one);
        BN_free(one);
        BIGNUM *r = BN_new();
        BN_gcd(r,subtr,rem,ctxbn);
        BN_print_fp(stdout,r);
        cout<<"\n";
        BN_print_fp(stdout,rem);
        cout<<"\n";
        if(BN_is_one(r)) {
            BN_free(subtr);
            BN_free(r);
            cout<<"Random values are: "<<count<<"\n";
            return rem;
        }
        BN_free(subtr);
        BN_free(r);
        BN_free(rem);
    }
}
DH *get_dh2236()
{
static unsigned char dh2236_p[]={
    0x00,0xA1,0xEE,0x4B,0x16,0x4E,0x56,0x91,0x5F,0x9C,0x1B,0x0A,0x91,0x7C,0xF4,
    0xAE,0x52,0x1E,0xC3,0x01,0x21,0x7D,0xA5,0xC9,0xF3,0x2E,0xD6,0xD0,0x67,0x8B,
    0x9B,0xEE,0x07,0x8E,0x07,0x03,0x22,0xBA,0xB8,0x3C,0x48,0xB9,0xD1,0x38,0xC3,
    0xDB,0x3B,0x80,0xBC,0xB8,0x5A,0xFA,0x27,0x0A,0x83,0xDC,0xCB,0x12,0xF7,0x77,
    0x8F,0x6E,0x1E,0x09,0xE2,0xB5,0xF8,0x8B,0x6B,0xC1,0x00,0x78,0x07,0x4F,0x9E,
    0x77,0xE3,0xD8,0xC5,0xF5,0x11,0x51,0xA3,0x02,0x10,0x6E,0x29,0x48,0x76,0x08,
    0x1B,0xAC,0xA6,0x09,0x2A,0x10,0x8D,0x3D,0x5C,0xF5,0x2A,0xF8,0xFD,0x39,0xEC,
    0xD1,0xB0,0x65,0xFD,0xBB,0x3B,0xCF,0x0A,0xDE,0x31,0xB7,0x0B,0xE1,0x9B,0xAE,
    0x83,0xCB,0xB2,0xCA,0x20,0xC6,0x17,0x50,0x87,0xCD,0xE9,0x81,0x32,0xA5,0xD1,
    0x00,0xA2,0x45,0x5F,0x95,0x3A,0x37,0x29,0x5C,0x36,0xD2,0x5A,0x19,0x7C,0x18,
    0xCC,0x9F,0x73,0x93,0xCD,0xB4,0xAC,0xB4,0x66,0x38,0x17,0x09,0x33,0xBE,0x92,
    0x4D,0x07,0x30,0xEE,0x23,0x6E,0x4B,0xB5,0x8D,0xAD,0xB8,0xAD,0xA8,0x8C,0xF5,
    0x9B,0xE8,0x58,0x1C,0x5A,0xD2,0x82,0xC6,0x0A,0x41,0x32,0x58,0xE4,0x5F,0x87,
    0x52,0xB3,0xA5,0x5F,0xC0,0xFE,0x8E,0x93,0x98,0xEE,0xF0,0x06,0x14,0xB8,0xAC,
    0xD8,0x50,0xF4,0x48,0x00,0x37,0x9B,0x7F,0x14,0x24,0xF9,0xD3,0xBB,0xBD,0x43,
    0x20,0xA4,0xCA,0x35,0x97,0xE2,0x02,0x4D,0x3F,0xE7,0xC7,0x01,0x4E,0x95,0x98,
    0xF7,0x08,0xFC,0xCB,0x77,0x72,0x0C,0x6E,0xC7,0x53,0x8D,0x5F,0x48,0x2F,0xFB,
    0xAA,0x23
    };
static unsigned char dh2236_g[]={
    0x02,
    };
DH *dh;

if ((dh=DH_new()) == NULL) return(NULL);
dh->p=BN_bin2bn(dh2236_p,sizeof(dh2236_p),NULL);
dh->g=BN_bin2bn(dh2236_g,sizeof(dh2236_g),NULL);
if ((dh->p == NULL) || (dh->g == NULL))
    { DH_free(dh); return(NULL); }
return(dh);
}

DH* getDH(){

    DH *privkey = get_dh2236 ();
    int codes;
    //int secret_size;
    /*
    Generate the parameters to be used 
    if(NULL == (privkey = DH_new())) handleErrors();
    if(1 != DH_generate_parameters_ex(privkey, 2048, DH_GENERATOR_2, NULL)) handleErrors();
    */
    if(1 != DH_check(privkey, &codes)) handleErrors();
    if(codes != 0)
    {
        /* Problems have been found with the generated parameters */
        /* Handle these here - we'll just abort for this example */
        printf("DH_check failed\n");
        abort();
    }
    return privkey;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    const RAND_METHOD* rm = RAND_get_rand_method();
    if(rm == RAND_SSLeay())
    {
        printf("Using default generator\n");
    }
    RAND_poll();
    cout<<"Seeded\n";

    DH* dhDB= getDH();

    ifstream fin;
    int d = 4;
    int k = 3;
    map<int, set<string> > DB;
    vvi relDB;  
    map<string, set<int> > DB_inv;
    set<string> W;

    REP(i,1,d) {
        string fname = "./src/Docs/"+to_string(i) + ".txt";
        fin.open(fname,ios::in);
        int k_i;
        vector<int> rel;
        fin>>k_i;
        REP(j,0,k_i-1) {
            int temp;
            fin>>temp;
            rel.pb(temp);
        }
        relDB.pb(rel);
        string w;
        while(fin>>w) {
            DB[i].insert(w);
            DB_inv[w].insert(i);
            W.insert(w);
        }
        fin.close();
    }


    int portno = atoi(argv[1]);

    //TCPServer Server(portno);
    //Server.TCPbind();
    if (1) {
        //Server.TCPConnect();
        //string query = Server.TCPread();
        string query = "words&some";
        
        int attr = 1;
        int sz = query.size();
        vector<string> keywords;
        vector<bool> andor;
        string word = "";
        REP(i,0,sz-1) {
            if(query[i]=='&' || query[i]=='|') {
                keywords.pb(word);
                word = "";
                if(query[i]=='&') {
                    andor.pb(false);
                }
                else {
                    andor.pb(true);
                }
            }
            else {
                word = word+query[i];               //fix this code, O(n^2), no concatenation at each step
            }
        }
        keywords.pb(word);
        
        // Write Code for s-word


        int sizeofkey = tau/8;
        /*
        const uchar* K_s = genrandomkey(sizeofkey);
        const uchar* K_x = genrandomkey(sizeofkey);
        const uchar* K_i = genrandomkey(sizeofkey);
        */

        FILE* fp;
        fp = fopen("../keys.txt","r");
        uchar* temp1 = (uchar*)malloc(sizeofkey*(sizeof(uchar)));
        REP(i,0,sizeofkey-1) {
            fscanf(fp,"%u",&temp1[i]);
        }
        const uchar* K_s = temp1;
        uchar* temp2 = (uchar*)malloc(sizeofkey*(sizeof(uchar)));
        REP(i,0,sizeofkey-1) {
            fscanf(fp,"%u",&temp2[i]);
        }
        const uchar* K_x = temp2;
        uchar* temp3 = (uchar*)malloc(sizeofkey*(sizeof(uchar)));
        REP(i,0,sizeofkey-1) {
            fscanf(fp,"%u",&temp3[i]);
        }
        const uchar* K_i = temp3;
        fclose(fp);
        
        unsigned int lentau = tau;

        
        unsigned char* strap = PRF(K_s,sizeofkey,keywords[0].c_str(),lentau);

        BN_MONT_CTX *montctx;
        montctx = BN_MONT_CTX_new();
        if(!montctx) {
            handleErrors();
        }

        BN_CTX *ctxbn;
        ctxbn = BN_CTX_new();
        if(!ctxbn) {
            handleErrors();
        }
        if (1 != BN_MONT_CTX_set(montctx, dhDB->p, ctxbn)) handleErrors();
        

        vector< BIGNUM* > bxtrap;
        vector< BIGNUM* > blinds;
        sz  = keywords.size();
        REP(i,1,sz-1) {
            BIGNUM* exp1 = PRF_p(K_x,sizeofkey,keywords[i].c_str(),lentau,dhDB->p);
            
            BIGNUM *xtrapi = BN_new();
            if(1 != BN_mod_exp(xtrapi, dhDB -> g, exp1, dhDB->p, ctxbn)) handleErrors();
            BN_free(exp1);

            BIGNUM* rem = generatenosmodp1(ctxbn,dhDB);
            blinds.pb(rem);

            BIGNUM *bxtrapi = BN_new();
            if(1 != BN_mod_exp(bxtrapi, xtrapi, rem, dhDB->p, ctxbn)) handleErrors();
            BN_free(xtrapi);
            bxtrap.pb(bxtrapi);
        }

        fp = fopen("../blindstuff.txt","w");
        fprintf(fp,"%d\n",attr);
        fprintf(fp,"%d\n",attr);
        REP(i,1,sz-1) {
            BN_print_fp(fp,blinds[i-1]);
            fprintf(fp,"\n");
        }
        REP(i,0,lentau-1) {
            fprintf(fp,"%02x", (unsigned int)strap[i]);
        }
        fprintf(fp,"\n");

        REP(i,1,sz-1) {
            BN_print_fp(fp,bxtrap[i-1]);
            fprintf(fp,"\n");
        }
        fclose(fp);

        //Server.TCPwrite("hey");
        
        BN_CTX_free(ctxbn);                       //see start, end ctx
        BN_MONT_CTX_free(montctx);
        REP(i,1,sz-1) {
            BN_free(bxtrap[i-1]);
        }
        
        //Server.TCPConnectclose();

    }
	return 0;
}