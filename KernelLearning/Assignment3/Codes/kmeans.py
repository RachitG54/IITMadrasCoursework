# kmeans.py
import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model, datasets
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import accuracy_score
from sklearn.cluster import KMeans
from sklearn.metrics.cluster import adjusted_rand_score
from sklearn.metrics.cluster import v_measure_score
import random
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


def predict(array,centres):
	size = array.shape
	N = size[0]
	M = centres.shape[0]
	z = np.zeros((N),dtype = np.int)
	for n in xrange(N):
		minindex = 0
		minval = LA.norm(np.subtract(array[n],centres[0])) 
		for i in xrange(M):
			a = LA.norm(np.subtract(array[n],centres[i]))
			if(minval>a):
				minval = a
				minindex = i
		z[n] = minindex
	return z

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

print mu

iter = 0
for iter in xrange(100):
	if(iter == 0 or iter == 2 or iter == 5):
		h = 0.01
		x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
		y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
		xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
		Z = predict(np.c_[xx.ravel(), yy.ravel()],mu)
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
		plt.title('Dataset5, k means clustering,iteration:'+str(iter)+'clusters = '+str(M))
		plt.legend(loc='best')

		plt.xlim(xx.min(), xx.max())
		plt.ylim(yy.min(), yy.max())
		plt.xticks(())
		plt.yticks(())
		plt.savefig("kmeansnormal1(b)"+str(iter)+"-"+str(M)+".png", bbox_inches='tight')
		plt.close(fig)
	z = np.zeros((N,M))
	for n in xrange(N):
		minindex = 0
		minval = LA.norm(np.subtract(X_train[n],mu[0])) 
		for i in xrange(M):
			a = LA.norm(np.subtract(X_train[n],mu[i]))
			if(minval>a):
				minval = a
				minindex = i
		z[n][minindex] = 1

	newmu = np.zeros((M,size[1]))

	flag = 0
	for i in xrange(M):
		count = 0
		sum = np.zeros((size[1]))
		for n in xrange(N):
			if(z[n][i]==1):
				count += 1
				sum = np.add(sum,X_train[n])
		if(count!=0):		
			sum /= count
			newmu[i] = sum
			if (np.array_equal(newmu[i],mu[i]) == False):
				flag = 1
	
	if(flag == 0):
		break;
	iter += 1
	mu = newmu

print mu
print iter

Z = predict(X_val,mu)

ans = confusion_matrix(Y_val-1, Z)

suma = 0

for i in xrange(M):
	maxi = -1;
	for j in xrange(ans.shape[0]):
		maxi = max(maxi,ans[j][i])
	suma += maxi

print (suma*1.0)/X_val.shape[0]


h = 0.01
x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
Z = predict(np.c_[xx.ravel(), yy.ravel()],mu)
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
plt.title('Dataset5, k means clustering,iteration:'+str(iter)+'clusters = '+str(M))
plt.legend(loc='best')

plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.xticks(())
plt.yticks(())
plt.savefig("kmeansnormal1(b)"+str(iter)+"-"+str(M)+".png", bbox_inches='tight')
plt.close(fig)
"""
plots = []
names = []
for score_func in score_funcs:		
    names.append(score_func.__name__)

plt.title("Clustering measures")
plt.xlabel('Number of clusters')
plt.ylabel('Score value')
plt.legend(plots, names)
plt.ylim(ymin=-0.05, ymax=1.05)
plt.show()
"""

