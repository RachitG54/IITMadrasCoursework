import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import linear_model, datasets
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import accuracy_score
from sklearn import cross_validation
from sklearn.decomposition import PCA

data = np.genfromtxt('Dataset3', delimiter=',',dtype = np.int)

Y_train2 = data[:,1:6]
Y_train  =  Y_train2[:,0]
for i in xrange(data.shape[0]):
	for j in xrange(5):
		if Y_train2[i][j] == 1:
			Y_train[i] = j+1;
print Y_train
"""
X_train = []
flag = 1
for i in xrange(data.shape[0]):
	temp = np.genfromtxt('./Group_7/'+str(i+1)+'.feats', delimiter=' ')
	temp = np.reshape(temp,(512,1)).T;
	if(flag == 1):
		X_train = temp;
		flag = -1;
	else:
		X_train = np.concatenate((X_train, temp));
print X_train.shape
np.savetxt('Dataset3_x.txt', X_train, delimiter=',')
"""
X_train = np.genfromtxt('Dataset3_x.txt', delimiter=',')

plt.ioff()
acc = []
cm = []
d = []
"""
for k in xrange(1):
	d.append(1);
	cvval = 3
	pca = PCA(n_components=10);
	pca.fit(X_train);
	poly = PolynomialFeatures(3)
	logreg = linear_model.LogisticRegression()
	scores = cross_validation.cross_val_score(logreg, poly.fit_transform(pca.transform(X_train)), Y_train, cv=cvval)
	
	cm2 = confusion_matrix(Y_train, cross_validation.cross_val_predict(logreg,poly.fit_transform(pca.transform(X_train)),Y_train,cv = cvval));
	print cm2
	
	if(len(cm)==0):
		cm = cm2
	else:
		cm = np.concatenate((cm, cm2));
	
	scores = np.sum(scores);
	scores = scores/cvval;
	acc.append(scores)

print acc
"""
acc = [0.7619,0.9285,0.9642,0.8809,0.9109]
d = [1,5,10,50,100]
fig = plt.figure()
plt.plot(d,acc,c='r',label = 'cross validated set')
plt.xlabel("Number of feature maps in layer 1")
plt.ylabel("accuracy")
plt.title('Accuracy for dataset2 using CNN')
plt.legend(loc='upper left')
plt.ylim([0,1.1])
plt.savefig("2Models.png", bbox_inches='tight')
plt.close(fig)
