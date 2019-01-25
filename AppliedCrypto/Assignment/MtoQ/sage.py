import numpy as np;
K.<x> = GF(2^8,modulus = 'random',repr = 'int');
print K.modulus()
x.multiplicative_order()
a = []
for i,x in enumerate(K):
    a.append(x)
print a
S = mq.SBox(a)
mat = [1, 0, 0, 0, 1, 1, 1, 1 ,
1, 1, 0, 0, 0, 1, 1, 1 ,
1, 1, 1, 0, 0, 0, 1, 1 ,
1, 1, 1, 1, 0, 0, 0, 1 ,
1, 1, 1, 1, 1, 0, 0, 0 ,
0, 1, 1, 1, 1, 1, 0, 0 ,
0, 0, 1, 1, 1, 1, 1, 0 ,
0, 0, 0, 1, 1, 1, 1, 1]
mat = np.asarray(mat);
mat = np.reshape(mat,(8,8));
print mat
a = [];
xor = [0,1,1,0,0,0,1,1];
for i in xrange(2^8):
    c = S.to_bits(S[i]);
    cdash = np.asarray(c)
    cdash = cdash.transpose()
    cdash = np.reshape(cdash,(8,1));
    affine = np.dot(mat,cdash);
    affine = affine.transpose()
    #print affine
    d = [0,0,0,0,0,0,0,0];
    for b in xrange(8):
        d[b] = ((affine[0][b] + xor[b])%2);
    #print d
    a.append(S.from_bits(d));
    #print a
print a
S = mq.SBox(a)
#print S.difference_distribution_matrix().str();
#print S.is_apn()
#print S.is_almost_bent()
print S.is_balanced()
#print S.is_bent()
print S.is_permutation()
print S.nonlinearity()
print S.max_degree()