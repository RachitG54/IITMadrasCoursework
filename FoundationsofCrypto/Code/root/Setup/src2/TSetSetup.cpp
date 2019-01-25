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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#define lambda 256
#define n(x) 1*x

random_device rd;
mt19937 mt(rd());

typedef struct record {
	int label;
	int value;
}record;

uchar* genrandomkey(int &len) {
    uniform_real_distribution<double> dist(0.0, 256.0);
    len = lambda/8;
    uchar* key = (uchar*)malloc(sizeof(uchar)*len);
    REP(i,0,len-1) {
        key[i] = (int)dist(mt);
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
unsigned char* PRF(const unsigned char* seed, int sizeofseed, const char* input) {
    unsigned char* result;
    unsigned int len = 32;
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

void handleErrors()
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char* result, unsigned char* input, unsigned char *key, unsigned char *iv)
{

  EVP_CIPHER_CTX *ctx;

  int len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, , NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, n)) handleErrors();
        

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len)) handleErrors();
  ciphertext_len += len;
  fwrite(ciphertext, 1, len, ofp);  

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}
int decrypt(Params &params, unsigned char *key, unsigned char *iv)
{

  unsigned char plaintext[BUFSIZE + 128];
  unsigned char ciphertext[BUFSIZE];
  int res;
  FILE *ifp = fopen(params.infile.c_str(), "r");
  FILE *ofp = fopen(params.outfile.c_str(), "w");
  

  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len = 0;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, params.cipher, NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */

  while (!feof(ifp)) {
    int n;
    n = fread(ciphertext, 1, BUFSIZE, ifp);
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, n)) handleErrors();
    plaintext_len += len;
    fwrite(plaintext, 1, len, ofp);
  }

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext, &len)) handleErrors();
  plaintext_len += len;
  fwrite(plaintext, 1, len, ofp);

  fclose(ifp);
  fclose(ofp);

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}
int main() {
    vector< vector <record> > TSet(B, vector<record>(S));
    vector< set<int> > Free(B);
    initializing(Free);


    ifstream fin;
    int d = 4;
    map<int, set<string> > DB;  
    map<string, set<int> > DB_inv;
    set<string> W;

    REP(i,1,d) {
        string fname = "./src/Docs/"+to_string(i) + ".txt";
        fin.open(fname,ios::in);
        string w;
        while(fin>>w) {
            DB[i].insert(w);
            DB_inv[w].insert(i);
            W.insert(w);
        }
        fin.close();
    }

    int sizeofkey;
    const uchar* K_s = genrandomkey(sizeofkey);
    const uchar* K_x = genrandomkey(sizeofkey);
    const uchar* K_i = genrandomkey(sizeofkey);
    const uchar* K_z = genrandomkey(sizeofkey);

    map< string,vector<int> >T;
    set<int> XSet;
    tr(W,it) {
        const char *w = (*it).c_str();
        unsigned char* K_eps = PRF(K_s,sizeofkey,w);
        vector<int> t;
        int c = 0;
        tr(DB_inv[string(w)],it2) {                         // Do it in random order
            unsigned char* xind = PRF(K_i, sizeofkey, to_string(*it2).c_str());
            unsigned char* z = PRF(K_z, sizeofkey, (string(w)+to_string(c)).c_str());
            unsigned char *y = 0
        }
        free(K_e);
    }
	return 0;
}