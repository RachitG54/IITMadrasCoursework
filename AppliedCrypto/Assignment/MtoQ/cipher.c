#include <stdio.h>
#include <string.h>
char* xor(char a[32],char b[32]) {
	int i = 0;
	char *ans = (char *)calloc(32,sizeof(char));
	for(i = 0;i<32;i++) {
		ans[i] = '0' + ((a[i]-'0')^(b[i]-'0'));
	}
	return ans;
}
char* fbox(char input[32],char key[32]) {
	char *output = (char *)calloc(32,sizeof(char));
	return output;
}
char** spl4(char input[128]) {
	int i,j = 0;
	char **temp = (char **)calloc(128,sizeof(char));
	for(i=0;i<4;i++) {
		for(j=0;j<32;j++) {
			temp[i][j] = input[32*i+j];
		}
	}
}
char *cleffia(char input[128], char key[32]) {
	char **temp = spl4(input);
	char *y3 = (temp[0],key);
	char *y0 = xor(y3,temp[1]);
	char *y1 = (temp[2],key);
	char *y2 = xor(y1,temp[3]);
}
int main() {
	char input[128];
	char temp32[32];
	char temp322[32];
	char key[32];
	int i;
	for(i = 0;i<32;i++) {
		temp32[i] = '1';
		temp322[i] = '0';
		key[i] = '1';
	}
	char *ans;
	ans = xor(temp32,temp322);
	printf("%s\n",ans);
	return 0;
}