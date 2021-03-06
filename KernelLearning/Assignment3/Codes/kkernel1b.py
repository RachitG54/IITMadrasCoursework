# kmeans.py
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from sklearn import linear_model, datasets
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import accuracy_score
from sklearn.cluster import KMeans
import random
import math
from numpy import linalg as LA

train1 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class1_train.txt', delimiter=' ')
train2 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class2_train.txt', delimiter=' ')

size = np.shape(train1)
Y_train1 = np.zeros([size[0]]);
Y_train1[:] = 1;

size = np.shape(train2)
Y_train2 = np.zeros([size[0]]);
Y_train2[:] = 2;

X_train = train1;
X_train = np.concatenate((X_train, train2));

Y_train = Y_train1;
Y_train = np.concatenate((Y_train, Y_train2));

test1 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class1_test.txt', delimiter=' ')
test2 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class2_test.txt', delimiter=' ')

size = np.shape(test1)
Y_test1 = np.zeros([size[0]]);
Y_test1[:] = 1;

size = np.shape(test2)
Y_test2 = np.zeros([size[0]]);
Y_test2[:] = 2;

X_test = test1;
X_test = np.concatenate((X_test, test2));


Y_test = Y_test1;
Y_test = np.concatenate((Y_test, Y_test2));

val1 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class1_val.txt', delimiter=' ')
val2 = np.genfromtxt('./1.Bivariate/non-linearly_Separable/group7/class2_val.txt', delimiter=' ')

size = np.shape(val1)
Y_val1 = np.zeros([size[0]]);
Y_val1[:] = 1;

size = np.shape(val2)
Y_val2 = np.zeros([size[0]]);
Y_val2[:] = 2;

X_val = val1;
X_val = np.concatenate((X_val, val2));

Y_val = Y_val1;
Y_val = np.concatenate((Y_val, Y_val2));


def predict(array,centres,kmat,zold,value,X_train,couunt):
	size = array.shape
	N = size[0]
	M = centres.shape[0]
	z = np.zeros((N))
	Ndash = kmat.shape[0]
	print(str(N)+" "+str(M)+" "+str(Ndash))

	Xdash = np.tile(X_train,(N,1))
	arr = np.repeat(Ndash,N)
	arraydash = np.repeat(array,arr,axis = 0)

	mods = LA.norm(np.subtract(Xdash,arraydash),axis = 1)
	print mods.shape

	sigma = 0.18;
	mods = mods * (-1.0/sigma)
	kmatnew = np.exp(mods)
	kmatnew = np.reshape(kmatnew,(N,Ndash))
	kmatnew = kmatnew.transpose()
	
	couuntdash = np.tile(couunt,(N,1))
	zoldt = zold.transpose()
	vmat = np.dot(zoldt,kmatnew)
	vmat = vmat.transpose()
	valuedash = np.tile(value,(N,1))
	vmat = np.divide(vmat,couuntdash)
	vmat = np.add(valuedash,(-2)*vmat)

	znew2 = np.argmin(vmat,1)
	z = np.argmin(vmat,1)
	return z


def kernelgaussian(x1,x2):
	sigma = 0.18
	gamma = 2.0/(2*sigma)
	value = LA.norm(np.subtract(x1,x2))*gamma
	return math.exp(-value)

size = np.shape(X_train)
N = size[0]
M = 2

mu = np.zeros((M,size[1]))
for x in xrange(M):
	for j in xrange(size[1]):
		minval = maxval = X_train[0][j]
		for i in xrange(N):	
			minval = min(minval,X_train[i][j])
			maxval = max(maxval,X_train[i][j])
		mu[x][j] = random.uniform(minval,maxval)


z = np.zeros((N,M))
couunt = np.zeros((M))

for n in xrange(N):
	i = np.random.randint(M)
	z[n][i] = 1
	couunt[i] += 1

print couunt

kmat = np.zeros((N,N))

for i in xrange(N):
	for j in xrange(i,N):
		kmat[i][j] = kmat[j][i] = kernelgaussian(X_train[i],X_train[j])


iter = 0
for iter in xrange(100):
		
	value = np.zeros((M))
	for i in xrange(M):
		value[i] = np.dot(np.dot((z.transpose())[i].transpose(),kmat),(z.transpose())[i])
		if(couunt[i]==0):
			value[i] = 0
		else:
			value[i] = value[i] / (couunt[i]*couunt[i])
	
	print value
	"""
	if(iter == 0 or iter == 2 or iter == 5):
		h = 0.01
		x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
		y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
		xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
		Z = predict(np.c_[xx.ravel(), yy.ravel()],mu,kmat,z,value,X_train,couunt)
		np.savetxt('ziters'+str(iter)+'.txt', Z,delimiter=',')
	"""
	couuntdash = np.tile(couunt,(N,1))
	zt = z.transpose()
	valuedash = np.tile(value,(N,1))
	vmat = np.dot(zt,kmat)
	vmat = vmat.transpose()
	vmat = np.divide(vmat,couuntdash)
	vmat = np.add(valuedash,(-2)*vmat)
	znew2 = np.argmin(vmat,1)
	znew = np.zeros((N,M))
	for n in xrange(N):
		znew[n][znew2[n]] = 1
	newmu = np.zeros((M,size[1]))
	flag = 0

	for i in xrange(M):
		count = 0
		sum = np.zeros((size[1]))
		for n in xrange(N):
			if(znew[n][i]==1):
				count += 1
				sum = np.add(sum,X_train[n])
		if(count!=0):		
			sum /= count
			newmu[i] = sum
			if (np.array_equal(newmu[i],mu[i]) == False):
				flag = 1

		couunt[i] = count
	

	print couunt

	if(np.array_equal(znew,z) == True):
		break;
	if(flag == 0):
		break;
	iter += 1
	mu = newmu
	z = znew
	print z
print mu
print iter


value = np.zeros((M))
for i in xrange(M):
	value[i] = np.dot(np.dot((z.transpose())[i].transpose(),kmat),(z.transpose())[i])
	if(couunt[i]==0):
		value[i] = 0
	else:
		value[i] = value[i] / (couunt[i]*couunt[i])


h = 0.075
x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
Z = predict(np.c_[xx.ravel(), yy.ravel()],mu,kmat,z,value,X_train,couunt)
#Z = np.loadtxt(a[q], delimiter=',',dtype = np.int)

print Z.shape
xr = xx.ravel()
yr = yy.ravel()

Zd = []
Xd = []
Yd = []
for i in xrange(M):
	Zd.append([])
	Yd.append([])
	Xd.append([])

for i in xrange(Z.shape[0]):
	Zd[Z[i]].append(Z[i])
	Xd[Z[i]].append(xr[i])
	Yd[Z[i]].append(yr[i])


Xdd = np.array(Xd)
Ydd = np.array(Yd)



# Put the result into a color plot
fig = plt.figure(1, figsize=(16, 9))

colors = ['c','g','w','r','y','m']
for i in xrange(M):
	plt.scatter(Xdd[i],Ydd[i],c = colors[i], facecolor='0.5', lw = 0,label = 'cluster'+str(i+1));


plt.scatter(train1[:,0],train1[:,1],c = 'k',label = 'train class 1');
plt.scatter(train2[:,0],train2[:,1],c = 'r',label = 'train class 2');

plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.title('Dataset5, kernel k means clustering,iteration:'+str(iter)+'number of clusters:'+str(M))
plt.legend(loc='best')

plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.xticks(())
plt.yticks(())
plt.show()
#plt.savefig("kmeanskernel(b)"+str(iter)+"-"+str(M)+".png", bbox_inches='tight')
#plt.close(fig)
"""

a = ['zfinal.txt','ziters0.txt','ziters2.txt','ziters5.txt']
b = [10,0,2,5]

for q in xrange(len(a)):
	h = 0.02
	iter = b[q]
	x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
	y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
	xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
	#Z = predict(np.c_[xx.ravel(), yy.ravel()],mu,kmat,z,value,X_train,couunt)
	Z = np.loadtxt(a[q], delimiter=',',dtype = np.int)

	print Z.shape
	xr = xx.ravel()
	yr = yy.ravel()

	Zd = []
	Xd = []
	Yd = []
	for i in xrange(M):
		Zd.append([])
		Yd.append([])
		Xd.append([])

	for i in xrange(Z.shape[0]):
		Zd[Z[i]].append(Z[i])
		Xd[Z[i]].append(xr[i])
		Yd[Z[i]].append(yr[i])


	Xdd = np.array(Xd)
	Ydd = np.array(Yd)



	# Put the result into a color plot
	fig = plt.figure(1, figsize=(16, 9))

	colors = ['c','g','w','r','y','m']
	for i in xrange(M):
		plt.scatter(Xdd[i],Ydd[i],c = colors[i], facecolor='0.5', lw = 0,label = 'cluster'+str(i+1));


	plt.scatter(train1[:,0],train1[:,1],c = 'k',label = 'train class 1');
	plt.scatter(train2[:,0],train2[:,1],c = 'r',label = 'train class 2');

	plt.xlabel('Feature 1')
	plt.ylabel('Feature 2')
	plt.title('Dataset5, kernel k means clustering,iteration:'+str(iter)+'number of clusters:'+str(M))
	plt.legend(loc='best')

	plt.xlim(xx.min(), xx.max())
	plt.ylim(yy.min(), yy.max())
	plt.xticks(())
	plt.yticks(())
	plt.savefig("kmeanskernel(b)"+str(iter)+"-"+str(M)+".png", bbox_inches='tight')
	plt.close(fig)
"""