import numpy as np

sBox = [99, 128, 164, 236, 124, 93, 31, 155, 197, 46, 249, 86, 94, 78, 110, 121, 87, 11, 228, 108, 125, 8, 181, 153, 193, 113, 71, 43, 243, 66, 33, 231, 61, 136, 180, 204, 107, 115, 67, 116, 77, 63, 140, 235, 114, 22, 222, 79, 59, 211, 85, 15, 187, 210, 0, 165, 185, 214, 95, 27, 196, 123, 83, 84, 90, 17, 208, 4, 250, 7, 171, 242, 23, 139, 229, 57, 215, 10, 177, 198, 127, 12, 234, 39, 188, 220, 75, 100, 109, 40, 162, 183, 157, 158, 207, 58, 134, 255, 13, 191, 141, 190, 216, 20, 218, 16, 133, 174, 248, 3, 244, 76, 106, 38, 233, 118, 73, 96, 50, 150, 223, 26, 145, 209, 81, 80, 5, 175, 173, 169, 246, 72, 53, 152, 148, 219, 69, 47, 172, 252, 92, 74, 49, 199, 42, 166, 232, 35, 227, 98, 54, 201, 97, 103, 60, 221, 30, 206, 111, 44, 253, 9, 224, 51, 195, 117, 24, 149, 142, 239, 45, 168, 163, 226, 55, 156, 203, 101, 56, 130, 160, 179, 194, 32, 178, 151, 138, 176, 147, 213, 14, 238, 120, 2, 161, 230, 104, 34, 182, 200, 52, 205, 62, 217, 65, 112, 18, 129, 241, 70, 126, 89, 64, 37, 184, 131, 245, 25, 192, 36, 237, 41, 247, 29, 159, 154, 144, 132, 251, 82, 1, 240, 19, 212, 91, 68, 122, 6, 254, 88, 21, 143, 186, 135, 170, 167, 189, 137, 225, 102, 105, 119, 28, 202, 48, 146]

mat = [1, 0, 0, 0, 1, 1, 1, 1 ,
1, 1, 0, 0, 0, 1, 1, 1 ,
1, 1, 1, 0, 0, 0, 1, 1 ,
1, 1, 1, 1, 0, 0, 0, 1 ,
1, 1, 1, 1, 1, 0, 0, 0 ,
0, 1, 1, 1, 1, 1, 0, 0 ,
0, 0, 1, 1, 1, 1, 1, 0 ,
0, 0, 0, 1, 1, 1, 1, 1]
print "sBox = ",
print sBox

inputbits = 8
outputbits = 8
rows = 2 ** inputbits
cols = outputbits
#binmap
c = np.zeros(shape = (rows,inputbits))
for x in xrange(rows):
	no = x
	j = inputbits-1;
	while(no>0):
		c[x][j] = no%2;
		no/=2;
		j = j-1;
#sbox
balancedness = np.zeros(outputbits);
a = np.zeros(shape=(rows,cols),dtype = np.int)
for x in xrange(rows):
	y = sBox[x];
	no = y
	j = cols-1;
	while(no>0):
		a[x][j] = no%2;
		balancedness[j] += a[x][j];				#calculating balancedness
		no/=2;
		j = j-1;

a1 = []
for x in xrange(rows):
	y = sBox[x];
	a1.append(y);

np.set_printoptions(threshold='nan')
#linanalysis
print("\n\n\nLinear Analysis\n\n\n")
cols = 2 ** outputbits;
lincrypt = np.zeros(shape=(rows,cols));
for x in xrange(rows):
	for y in xrange(cols):
		val = 0
		for z in xrange(rows):
			zdash = x & z;
			no = zdash;
			count = 0;
			while(no>0):
				count += no%2;
				no/=2;
			ydash = a1[z] & y;
			no = ydash;
			while(no>0):
				count += no%2;
				no/=2;
			if(count%2==1):
				val += 1;
		lincrypt[x][y] = val-128;

print lincrypt


print("\n\n\nDiffCrypt Analysis\n\n\n")
#diffcryptanalysis
cols = 2 ** outputbits;
diffcrypt = np.zeros(shape=(rows,cols));
for x in xrange(rows):
	for z in xrange(rows):
		zstar = x^z;
		y = a1[z];
		ystar = a1[zstar];
		diffcrypt[x][y^ystar] += 1;

print diffcrypt

#printing balancedness
print "\n\n\n\nBalancedness property -           ",
print balancedness

#calculating nonlinearity
nonlinearity = np.zeros(outputbits)
for y in xrange(outputbits):
	minvalue = 2*rows+1;
	for aff in xrange(2*rows):
		sum = 0;
		for x in xrange(rows):
			no = aff & (2*x);
			count = 0;
			while(no>0):
				count += no%2;
				no/=2;
			count += aff%2;
			count = count%2;
			sum += a[x][y] ^ count;
		minvalue = min(minvalue,sum);
	nonlinearity[y] = minvalue;
print "Nonlinearity on each output bit - ",
print nonlinearity

#calculating SAC
SAC = np.zeros(outputbits)
SAC2 = np.zeros(outputbits)
for y in xrange(outputbits):
	sum1 = 0;
	sum2 = 0;
	for x in xrange(rows):
		for alpha in xrange(inputbits):
			sum1 += a[x][y] ^ a[x ^ (2**alpha)][y];
			sum2 += (a[x][y] ^ a[x ^ (2**alpha)][y]+1)%2;
	SAC[y] = sum1;
	SAC2[y] = sum2;
print "The Strict Avalanche Criterion  - (The number of zeroes)",
print SAC
print "The Strict Avalanche Criterion  - (The number of ones)",
print SAC2
