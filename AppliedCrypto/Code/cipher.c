#include "cipher.h"
int rnd;
int main() {
	char input[128], output[128], key[128], input2[128], *outputPtr, *inputPtr;
	int i;

	for(i = 0; i < 128; i++) {
		input[i] = '1';
		key[i] = '1';
	}
	input[10] = '0';input[11] = '0';
	key[24] = '0';key[100] = '0'; 
	for(i = 0; i < 128; i++) {
		printf("%c", input[i]);
	}
	printf("\n");

	inputPtr = (char *) calloc(128, sizeof(char));
	outputPtr = (char *) calloc(128, sizeof(char));
	encrypt(input, outputPtr, key);
	for(i = 0; i < 128; i++) {
		output[i] = outputPtr[i];
		printf("%c", output[i]);
	}
	printf("\n");
	decrypt(inputPtr, output, key);
	for(i = 0; i < 128; i++) {
		printf("%c", inputPtr[i]);
	}
	printf("\n");
	return 0;
}

char* xor32(char s1[32],char s2[32]) {
	char *ans = (char *) calloc(32, sizeof(char));;
	int i=0;
	for(i=0;i<32;i++) {
		ans[i] = (s1[i]-'0') ^ (s2[i] - '0');
		ans[i] += '0'; 
	}
	return ans;
}

char* xor128(char s1[128],char s2[128]) {
	char *ans = (char *) calloc(128, sizeof(char));
	int i=0;
	for(i=0;i<128;i++) {
		ans[i] = (s1[i]-'0') ^ (s2[i] - '0');
		ans[i] += '0'; 
	}
	return ans;
}

char ** splitinto4(char in[128]) {
	char **split = (char **) calloc(4, sizeof(char *));
	int i,j=0;
	for(i=0;i<4;i++) {
		split[i] = (char *) calloc(32, sizeof(char));
		for(j=0;j<32;j++) {
			split[i][j] = in[32*i+j];
		}
	}
	return split;
}

int tobinary(char input[8]) {
	int no = 0;
	int i=0;
	for(i=0;i<8;i++) {
		no = 2*no + input[i] - '0';
	}
	return no;
}

char* encryptionRound(char input[128], char key[128]) {
	char *xoredwithkey;
	xoredwithkey = xor128(input,key);
	//printf("after xoring is %s\n",xoredwithkey);
	char **split;
	split = splitinto4(xoredwithkey);
	//printf("hey%s\nhey%s\nhey%s\nhey%s\n",split[0],split[1],split[2],split[3]);
	char *outputStr, *outputStr1;
	outputStr = (char *) calloc(128, sizeof(char));
	outputStr1 = (char *) calloc(128, sizeof(char));
	//substitute(xoredwithkey, outputStr1);
	
	substitute(split[0], outputStr,0);
	substitute(split[1], outputStr,1);
	substitute(split[2], outputStr,2);
	substitute(split[3], outputStr,3);
	//printf("before permutation is 	%s\n",outputStr);
	permute(outputStr, outputStr1);
	//permute(input, outputStr);
	return outputStr1;
}

char* decryptionRound(char input[128], char key[128]) {
	char *xoredwithkey;
	char *outputStr, *outputStr1;
	outputStr = (char *) calloc(128, sizeof(char));
	outputStr1 = (char *) calloc(128, sizeof(char));
	invPermute(input, outputStr);
	//printf("before permutation should be %s\n",outputStr);
	char **split;
	split = splitinto4(outputStr);
	invSubstitute(split[0], outputStr1,0);
	invSubstitute(split[1], outputStr1,1);
	invSubstitute(split[2], outputStr1,2);
	invSubstitute(split[3], outputStr1,3);
	//printf("after xoring should be %s\n",outputStr1);
	xoredwithkey = xor128(outputStr1,key);
	//invSubstitute(xoredwithkey, outputStr1);
	//invPermute(input, outputStr);
	return xoredwithkey;
}


void encrypt(char plaintext[128], char *ciphertext, char key[128]) {
	//expandKey(key);
	char *intermediate;
	intermediate = (char *) calloc(128, sizeof(char));
	int i;

	for(i = 0; i < 128; i++) {
		intermediate[i] = plaintext[i];
	}
	for(rnd = 0; rnd < 10; rnd++) {
		intermediate = encryptionRound(intermediate, key);
	}
	for(i = 0; i < 128; i++) {
		ciphertext[i] = intermediate[i];
	}
}

void decrypt(char *plaintext, char ciphertext[128], char key[128]) {
	//expandKey(key);
	char *intermediate;
	intermediate = (char *) calloc(128, sizeof(char));
	int i;

	for(i = 0; i < 128; i++) {
		intermediate[i] = ciphertext[i];
	}
	for(rnd = 0; rnd < 10; rnd++) {
		intermediate = decryptionRound(intermediate, key);
	}
	for(i = 0; i < 128; i++) {
		plaintext[i] = intermediate[i];
	}
}

void expandKey(char key[128]) {
	int i;
	for(i = 0; i < NROUNDS; i++) {
		roundKeys[i] = DEFAULT_KEY;
	}
}

void permute(char *inputStr, char *outputStr) {
	int i, j;
	for(i = 0; i < NTEXTBITS; i++) {
		j = pBox[i];
		outputStr[j-1] = inputStr[i];
	}
}

void invPermute(char *inputStr, char *outputStr) {
	int i, j;
	for(i = 0; i < NTEXTBITS; i++) {
		j = invPBox[i];
		outputStr[j-1] = inputStr[i];
	}
}

void substitute(char *inputStr, char *outputStr,int initialvalue) {
	// convert input bits to integer,
	// substitute using sbox
	// convert back to binary number

	int *reqdSBox;
	int minIndex, maxIndex;
	int sBoxNum;
	int numBitsPerBox = (8);
	int inputDecimalForm, outputDecimalForm, o;
	int i;
	int bit;
	
	for(sBoxNum = 0; sBoxNum < NSBOXES; sBoxNum++) {

		reqdSBox = sBoxes[2*rnd%2+(initialvalue/2)];
		minIndex = numBitsPerBox * sBoxNum;
		maxIndex = minIndex + (numBitsPerBox - 1);
		inputDecimalForm = 0;
	
		for(i = minIndex; i <= maxIndex; i++) {
			bit = (int) (inputStr[i] - '0');
			//printf("%d", bit);
			inputDecimalForm = 2 * inputDecimalForm + bit;
		}

		outputDecimalForm = reqdSBox[inputDecimalForm];
		o = outputDecimalForm;
		//printf("o is %d\n",o);
		//printf("max index is %d, minIndex is %d\n",maxIndex,minIndex);
		for(i = maxIndex; i >= minIndex; i--) {
			bit = o%2;
			outputStr[i + initialvalue*32] = (bit +'0');
			o = o/2;
		}
	}
	//printf("\n");
}

void invSubstitute(char *inputStr, char *outputStr, int initialvalue) {
	// convert input bits to integer,
	// substitute using sbox
	// convert back to binary number
	int *reqdSBox;
	int minIndex, maxIndex;
	int sBoxNum;
	int numBitsPerBox = (8);
	int inputDecimalForm, outputDecimalForm, o;
	int i;
	int bit;
	//printf("input check at invSubstitute%s\n",inputStr);
	for(sBoxNum = 0; sBoxNum < NSBOXES; sBoxNum++) {

		reqdSBox = invSBoxes[2*(NROUNDS-1-rnd)%2 + (initialvalue/2)];
		minIndex = numBitsPerBox * sBoxNum;
		maxIndex = minIndex + (numBitsPerBox - 1);
		inputDecimalForm = 0;
	
		for(i = minIndex; i <= maxIndex; i++) {
			bit = (int) (inputStr[i] - '0');
			//printf("%d", bit);
			inputDecimalForm = 2 * inputDecimalForm + bit;
		}
		//printf("i is %d\n",inputDecimalForm);
		outputDecimalForm = reqdSBox[inputDecimalForm];
		o = outputDecimalForm;
		//printf("o is %d\n",o);
		//printf("max index is %d, minIndex is %d\n",maxIndex,minIndex);
		for(i = maxIndex; i >= minIndex; i--) {
			bit = o%2;
			outputStr[i + initialvalue*32] = (bit +'0');
			o = o/2;
		}
	}
	//printf("\n");
}
