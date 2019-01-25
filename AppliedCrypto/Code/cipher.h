#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NROUNDS 10
#define NKEYBITS 128
#define NROUNDKEYBITS 64
#define NTEXTBITS 128
#define NSBOXES 4
#define DEFAULT_KEY 0x123456789ABCDEF0

long roundKeys[NROUNDS];

int pBox[] = {
46, 12, 21, 67, 10, 48, 1, 23, 22, 4, 45, 75, 2, 28, 9, 47, 
30, 24, 5, 115, 26, 32, 49, 7, 6, 52, 29, 27, 50, 8, 25, 95, 
14, 44, 33, 99, 42, 16, 53, 55, 54, 36, 13, 107, 34, 56, 41, 15, 
62, 60, 37, 19, 58, 64, 17, 39, 38, 20, 61, 123, 18, 40, 57, 127, 
110, 76, 85, 3, 74, 112, 65, 87, 86, 68, 109, 11, 66, 96, 73, 111, 
94, 92, 69, 51, 90, 88, 113, 71, 70, 116, 93, 63, 82, 72, 89, 31, 
78, 108, 117, 35, 106, 80, 97, 122, 118, 100, 77, 43, 98, 120, 105, 79, 
126, 124, 101, 83, 119, 128, 59, 103, 102, 84, 125, 91, 114, 104, 121, 81
};

int invPBox[] = {
7, 13, 68, 10, 19, 25, 24, 30, 15, 5, 76, 2, 43, 33, 48, 38, 55, 61, \
52, 58, 3, 9, 8, 18, 31, 21, 28, 14, 27, 17, 96, 22, 35, 45, 100, 42, \
51, 57, 56, 62, 47, 37, 108, 34, 11, 1, 16, 6, 23, 29, 84, 26, 39, \
41, 40, 46, 63, 53, 119, 50, 59, 49, 92, 54, 71, 77, 4, 74, 83, 89, \
88, 94, 79, 69, 12, 66, 107, 97, 112, 102, 128, 93, 116, 122, 67, 73, \
72, 86, 95, 85, 124, 82, 91, 81, 32, 78, 103, 109, 36, 106, 115, 121, \
120, 126, 111, 101, 44, 98, 75, 65, 80, 70, 87, 125, 20, 90, 99, 105, \
117, 110, 127, 104, 60, 114, 123, 113, 64, 118
};

int sBox1[] = {
99, 128, 164, 236, 124, 93, 31, 155, 189, 241, 70, 6, 134, 135, 170, 223,
26, 145, 169, 246, 72, 53, 224, 75, 28, 178, 239, 85, 15, 187, 210, 0,
165, 193, 9, 152, 148, 163, 226, 79, 59, 211, 45, 208, 4, 130, 160, 203,
29, 159, 154, 144, 132, 131, 141, 190, 216, 20, 162, 207, 58, 254, 88, 21,
143, 186, 255, 117, 96, 74, 49, 199, 42, 222, 55, 228, 108, 125, 112, 106,
94, 54, 201, 25, 184, 251, 82, 1, 136, 180, 204, 19, 172, 252, 92, 50,
238, 120, 122, 126, 89, 56, 250, 127, 116, 77, 63, 244, 76, 18, 129, 137,
153, 185, 214, 39, 196, 3, 140, 147, 173, 209, 41, 247, 101, 64, 37, 192,
36, 237, 81, 40, 218, 16, 133, 174, 248, 123, 83, 44, 253, 113, 71, 43,
243, 66, 33, 231, 69, 47, 212, 35, 227, 98, 78, 22, 166, 232, 91, 60,
221, 30, 182, 200, 52, 205, 62, 217, 57, 215, 10, 177, 198, 7, 171, 242,
111, 84, 34, 206, 23, 139, 157, 158, 183, 229, 65, 8, 181, 225, 102, 105,
119, 100, 109, 80, 5, 175, 213, 14, 150, 167, 197, 46, 249, 86, 38, 233,
118, 73, 24, 149, 142, 151, 138, 176, 235, 114, 110, 121, 87, 11, 156, 179,
194, 32, 202, 48, 234, 95, 27, 188, 220, 51, 195, 13, 191, 245, 97, 103,
68, 2, 161, 230, 104, 90, 17, 168, 219, 61, 240, 107, 115, 67, 12, 146
};

int invSBox1[] = {
31, 87, 241, 117, 44, 196, 11, 173, 187, 34, 170, 221, 254, 235, 199, 28, 
133, 246, 109, 91, 57, 63, 155, 180, 210, 83, 16, 230, 24, 48, 161, 6, 
225, 146, 178, 151, 128, 126, 206, 115, 131, 122, 72, 143, 139, 42, 203, 149, 
227, 70, 95, 233, 164, 21, 81, 74, 101, 168, 60, 40, 159, 249, 166, 106, 
125, 186, 145, 253, 240, 148, 10, 142, 20, 209, 69, 23, 108, 105, 154, 39, 
195, 130, 86, 138, 177, 27, 205, 220, 62, 100, 245, 158, 94, 5, 80, 229, 
68, 238, 153, 0, 193, 124, 190, 239, 244, 191, 79, 251, 76, 194, 218, 176, 
78, 141, 217, 252, 104, 67, 208, 192, 97, 219, 98, 137, 4, 77, 99, 103, 
1, 110, 45, 53, 52, 134, 12, 13, 88, 111, 214, 181, 118, 54, 212, 64, 
51, 17, 255, 119, 36, 211, 200, 213, 35, 112, 50, 7, 222, 182, 183, 49, 46, 
242, 58, 37, 2, 32, 156, 201, 247, 18, 14, 174, 92, 120, 135, 197, 
215, 171, 25, 223, 89, 188, 162, 184, 84, 113, 65, 29, 231, 8, 55, 236, 
127, 33, 224, 234, 116, 202, 172, 71, 163, 82, 226, 47, 90, 165, 179, 59, 
43, 121, 30, 41, 150, 198, 114, 169, 56, 167, 132, 248, 232, 160, 73, 15, 
22, 189, 38, 152, 75, 185, 243, 147, 157, 207, 228, 216, 3, 129, 96, 26, 
250, 9, 175, 144, 107, 237, 19, 123, 136, 204, 102, 85, 93, 140, 61, 66
};

int sBox2[] = {
99, 106, 138, 238, 243, 81, 137, 75, 61, 232, 112, 62, 77, 190, 135, 180,
203, 247, 226, 60, 125, 100, 117, 24, 236, 195, 139, 123, 231, 26, 220, 25,
121, 215, 192, 46, 181, 94, 227, 169, 105, 47, 32, 74, 168, 252, 59, 107,
31, 250, 184, 4, 219, 15, 2, 88, 96, 198, 173, 218, 154, 22, 19, 53,
148, 233, 229, 42, 6, 235, 213, 240, 244, 71, 242, 196, 157, 0, 104, 186,
52, 1, 253, 174, 127, 84, 175, 234, 64, 228, 191, 18, 160, 128, 162, 176,
120, 66, 212, 101, 224, 12, 167, 150, 217, 63, 216, 170, 204, 225, 153, 179,
221, 140, 109, 156, 149, 124, 241, 97, 83, 185, 145, 207, 68, 87, 10, 36,
249, 29, 202, 98, 246, 119, 40, 54, 49, 39, 92, 211, 115, 155, 131, 7,
126, 193, 187, 161, 21, 182, 251, 45, 16, 144, 90, 80, 28, 95, 118, 189,
34, 122, 114, 14, 151, 76, 43, 147, 255, 158, 165, 166, 3, 205, 116, 141,
248, 136, 222, 41, 163, 37, 108, 9, 129, 55, 164, 51, 23, 134, 33, 223,
188, 183, 110, 57, 91, 197, 8, 20, 35, 239, 102, 69, 194, 30, 111, 172,
79, 142, 93, 70, 103, 208, 214, 85, 58, 254, 11, 177, 237, 86, 159, 48,
178, 72, 152, 38, 201, 199, 56, 206, 209, 67, 65, 113, 171, 89, 245, 210,
230, 143, 200, 82, 44, 133, 132, 17, 5, 78, 27, 73, 13, 50, 130, 146
};

int invSBox2[] = {
77, 81, 54, 172, 51, 248, 68, 143, 198, 183, 126, 218, 101, 252, 163, 53, 152, 247, 91, 62, 199, 148, 61, 188, 23, 31, 29, 250, 156, 129, 205, 48, 42, 190, 160, 200, 127, 181, 227, 137, 134, 179, 67, 166, 244, 151, 35, 41, 223, 136, 253, 187, 80, 63, 135, 185, 230, 195, 216, 46, 19, 8, 11, 105, 88, 234, 97, 233, 124, 203, 211, 73, 225, 251, 43, 7, 165, 12, 249, 208, 155, 5, 243, 120, 85, 215, 221, 125, 55, 237, 154, 196, 138, 210, 37, 157, 56, 119, 131, 0, 21, 99, 202, 212, 78, 40, 1, 47, 182, 114, 194, 206, 10, 235, 162, 140, 174, 22, 158, 133, 96, 32, 161, 27, 117, 20, 144, 84, 93, 184, 254, 142, 246, 245, 189, 14, 177, 6, 2, 26, 113, 175, 209, 241, 153, 122, 255, 167, 64, 116, 103, 164, 226, 110, 60, 141, 115, 76, 169, 222, 92, 147, 94, 180, 186, 170, 171, 102, 44, 39, 107, 236, 207, 58, 83, 86, 95, 219, 224, 111, 15, 36, 149, 193, 50, 121, 79, 146, 192, 159, 13, 90, 34, 145, 204, 25, 75, 197, 57, 229, 242, 228, 130, 16, 108, 173, 231, 123, 213, 232, 239, 139, 98, 70, 214, 33, 106, 104, 59, 52, 30, 112, 178, 191, 100, 109, 18, 38, 89, 66, 240, 28, 9, 65, 87, 69, 24, 220, 3, 201, 71, 118, 74, 4, 72, 238, 132, 17, 176, 128, 49, 150, 45, 82, 217, 168
};

int sBox3[] = {
99, 29, 238, 48, 94, 241, 215, 114, 45, 25, 62, 247, 167, 86, 40, 165,
246, 203, 89, 237, 252, 178, 200, 149, 1, 197, 240, 187, 125, 232, 64, 122,
244, 107, 186, 17, 231, 133, 35, 176, 104, 118, 253, 222, 199, 80, 88, 129,
243, 119, 145, 209, 2, 9, 28, 130, 63, 155, 168, 147, 113, 225, 245, 7,
181, 212, 190, 193, 32, 124, 132, 79, 191, 173, 47, 185, 221, 11, 188, 97,
195, 128, 159, 120, 84, 136, 70, 10, 208, 110, 6, 217, 219, 123, 152, 100,
127, 72, 163, 134, 239, 92, 81, 52, 142, 54, 46, 213, 210, 206, 229, 37,
192, 76, 115, 65, 22, 251, 174, 227, 85, 228, 73, 207, 137, 42, 5, 21,
55, 66, 218, 23, 151, 161, 38, 12, 160, 74, 3, 101, 19, 71, 102, 223,
171, 95, 157, 216, 183, 116, 93, 61, 59, 75, 111, 106, 214, 30, 34, 220,
103, 179, 164, 154, 196, 156, 180, 184, 177, 4, 121, 56, 135, 131, 83, 148,
109, 202, 53, 226, 57, 235, 140, 150, 205, 41, 201, 249, 14, 0, 169, 255,
126, 36, 172, 67, 182, 24, 82, 248, 98, 15, 108, 166, 58, 39, 96, 175,
143, 90, 33, 16, 139, 138, 230, 233, 44, 117, 49, 50, 254, 18, 43, 105,
26, 242, 27, 158, 20, 91, 77, 31, 78, 211, 162, 234, 224, 153, 8, 112,
141, 250, 194, 236, 144, 189, 13, 204, 69, 198, 60, 87, 68, 170, 51, 146
};

int invSBox3[] = {
189, 24, 52, 138, 169, 126, 90, 63, 238, 53, 87, 77, 135, 246, 188, 201, 211, 
35, 221, 140, 228, 127, 116, 131, 197, 9, 224, 226, 54, 1, 157, 231, 68, 210, 
158, 38, 193, 111, 134, 205, 14, 185, 125, 222, 216, 8, 106, 74, 3, 218, 219, 
254, 103, 178, 105, 128, 171, 180, 204, 152, 250, 151, 10, 56, 30, 115, 129, 
195, 252, 248, 86, 141, 97, 122, 137, 153, 113, 230, 232, 71, 45, 102, 198, 
174, 84, 120, 13, 251, 46, 18, 209, 229, 101, 150, 4, 145, 206, 79, 200, 0, 
95, 139, 142, 160, 40, 223, 155, 33, 202, 176, 89, 154, 239, 60, 7, 114, 149, 
217, 41, 49, 83, 170, 31, 93, 69, 28, 192, 96, 81, 47, 55, 173, 70, 37, 99, 
172, 85, 124, 213, 212, 182, 240, 104, 208, 244, 50, 255, 59, 175, 23, 183, 
132, 94, 237, 163, 57, 165, 146, 227, 82, 136, 133, 234, 98, 162, 15, 203, 
12, 58, 190, 253, 144, 194, 73, 118, 207, 39, 168, 21, 161, 166, 64, 196, 
148, 167, 75, 34, 27, 78, 245, 66, 72, 112, 67, 242, 80, 164, 25, 249, 44, 
22, 186, 177, 17, 247, 184, 109, 123, 88, 51, 108, 233, 65, 107, 156, 6, 147, 
91, 130, 92, 159, 76, 43, 143, 236, 61, 179, 119, 121, 110, 214, 36, 29, 215, 
235, 181, 243, 19, 2, 100, 26, 5, 225, 48, 32, 62, 16, 11, 199, 187, 241, 117, 
20, 42, 220, 191
};

int sBox4[] = {
99, 128, 164, 236, 124, 93, 31, 155, 197, 46, 249, 86, 94, 78, 110, 121,
87, 11, 228, 108, 125, 8, 181, 153, 193, 113, 71, 43, 243, 66, 33, 231,
61, 136, 180, 204, 107, 115, 67, 116, 77, 63, 140, 235, 114, 22, 222, 79,
59, 211, 85, 15, 187, 210, 0, 165, 185, 214, 95, 27, 196, 123, 83, 84,
90, 17, 208, 4, 250, 7, 171, 242, 23, 139, 229, 57, 215, 10, 177, 198,
127, 12, 234, 39, 188, 220, 75, 100, 109, 40, 162, 183, 157, 158, 207, 58,
134, 255, 13, 191, 141, 190, 216, 20, 218, 16, 133, 174, 248, 3, 244, 76,
106, 38, 233, 118, 73, 96, 50, 150, 223, 26, 145, 209, 81, 80, 5, 175,
173, 169, 246, 72, 53, 152, 148, 219, 69, 47, 172, 252, 92, 74, 49, 199,
42, 166, 232, 35, 227, 98, 54, 201, 97, 103, 60, 221, 30, 206, 111, 44,
253, 9, 224, 51, 195, 117, 24, 149, 142, 239, 45, 168, 163, 226, 55, 156,
203, 101, 56, 130, 160, 179, 194, 32, 178, 151, 138, 176, 147, 213, 14, 238,
120, 2, 161, 230, 104, 34, 182, 200, 52, 205, 62, 217, 65, 112, 18, 129,
241, 70, 126, 89, 64, 37, 184, 131, 245, 25, 192, 36, 237, 41, 247, 29,
159, 154, 144, 132, 251, 82, 1, 240, 19, 212, 91, 68, 122, 6, 254, 88,
21, 143, 186, 135, 170, 167, 189, 137, 225, 102, 105, 119, 28, 202, 48, 146
};

int invSBox4[] = {
54, 230, 193, 109, 67, 126, 237, 69, 21, 161, 77, 17, 81, 98, 190, 51, 105, 65, 206, 232, 103, 240, 45, 72, 166, 217, 121, 59, 252, 223, 156, 6, 183, 30, 197, 147, 219, 213, 113, 83, 89, 221, 144, 27, 159, 170, 9, 137, 254, 142, 118, 163, 200, 132, 150, 174, 178, 75, 95, 48, 154, 32, 202, 41, 212, 204, 29, 38, 235, 136, 209, 26, 131, 116, 141, 86, 111, 40, 13, 47, 125, 124, 229, 62, 63, 50, 11, 16, 239, 211, 64, 234, 140, 5, 12, 58, 117, 152, 149, 0, 87, 177, 249, 153, 196, 250, 112, 36, 19, 88, 14, 158, 205, 25, 44, 37, 39, 165, 115, 251, 192, 15, 236, 61, 4, 20, 210, 80, 1, 207, 179, 215, 227, 106, 96, 243, 33, 247, 186, 73, 42, 100, 168, 241, 226, 122, 255, 188, 134, 167, 119, 185, 133, 23, 225, 7, 175, 92, 93, 224, 180, 194, 90, 172, 2, 55, 145, 245, 171, 129, 244, 70, 138, 128, 107, 127, 187, 78, 184, 181, 34, 22, 198, 91, 214, 56, 242, 52, 84, 246, 101, 99, 218, 24, 182, 164, 60, 8, 79, 143, 199, 151, 253, 176, 35, 201, 157, 94, 66, 123, 53, 49, 233, 189, 57, 76, 102, 203, 104, 135, 85, 155, 46, 120, 162, 248, 173, 148, 18, 74, 195, 31, 146, 114, 82, 43, 3, 220, 191, 169, 231, 208, 71, 28, 110, 216, 130, 222, 108, 10, 68, 228, 139, 160, 238, 97
};

int *sBoxes[] = {sBox1, sBox2, sBox3, sBox4};
int *invSBoxes[] = {invSBox1, invSBox2, invSBox3, invSBox4};

void expandKey(char key[128]);

void permute(char *inputStr, char *outputStr);
void invPermute(char *inputStr, char *outputStr);

void substitute(char *inputStr, char *outputStr,int initialvalue);
void invSubstitute(char *inputStr, char *outputStr,int initialvalue);

void encrypt(char plaintext[128], char *ciphertext, char key[128]);
void decrypt(char *plaintext, char ciphertext[128], char key[128]);
