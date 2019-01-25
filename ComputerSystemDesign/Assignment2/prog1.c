#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long long start_time, end_time;
unsigned int S = 6291456;            // L3 cache size
unsigned int B = 64;                 // cache line size
unsigned int N = 8192;

int perm[100];
void permute(int nn) {
    for (int i = 0; i < nn; i++) {
        perm[i] = i;
    }
    for (int i = 0; i < nn; i++) {
        int j = rand()%(nn-i) + i;
        int temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }
}


unsigned long long rdtscl()
{
    unsigned long lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}

void printtime() {
    printf("%llu\n", end_time - start_time);
}

void init(char *base, int set,int size) {
    permute(size);
    for(int k = 0;k < size; k++) {
            int i = perm[k];
            if (i == size-1) 
                *((unsigned long *)(base+B*(set+N*i))) = (unsigned long) NULL;
            else
                *((unsigned long *)(base+B*(set+N*i))) = (unsigned long)base+B*(set+N*(i+1));
    }
}
void repeatAccess(char *base, int set, int size) {
    unsigned long* p = (unsigned long*)(base+B*set);
    for(int i = 0; i < size; i++) {
        if (p == NULL) break;
        p = *(unsigned long **)p;
    }
}
int main() {
    srand(time(NULL));
    char *base = malloc(8*S);
    char filename[20];
    int assocs[] = {16, 32, 48, 60, 70, 80};
    for (int k = 0; k < sizeof(assocs)/sizeof(int); k++) {
        int assoc = assocs[k];
        sprintf(filename, "assoc-%d.txt", assoc);
        FILE *fp = fopen(filename, "w");
        printf("Assoc = %d\n", assoc);
        int set = 0;
        while (set < N) {
            init(base, set, assoc);
            int tries = 10000;
            unsigned long long total_time = 0;
            for (int i = 0; i < tries; i++) {
                start_time = rdtscl();
                repeatAccess(base,set,assoc);
                end_time = rdtscl();
                total_time += (end_time - start_time);
            }
            total_time /= tries;
            total_time /= assoc;
            fprintf(fp, "%llu\n",total_time);
            set++;
        }
        fclose(fp);
    }
    return 0;
}
