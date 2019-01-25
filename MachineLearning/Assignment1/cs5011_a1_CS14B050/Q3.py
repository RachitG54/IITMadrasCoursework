import numpy as np
from sklearn.decomposition import PCA
from sklearn import datasets, linear_model
from sklearn.metrics import accuracy_score
from sklearn import metrics
#from sklearn.lda import LDA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

X_test = np.genfromtxt(
    '/home/rachit/Documents/Sem5/CS5011/ML-PA1/datasets/DS3/test.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
y_test = np.genfromtxt(
    '/home/rachit/Documents/Sem5/CS5011/ML-PA1/datasets/DS3/test_labels.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
X_train = np.genfromtxt(
    '/home/rachit/Documents/Sem5/CS5011/ML-PA1/datasets/DS3/train.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
y_train = np.genfromtxt(
    '/home/rachit/Documents/Sem5/CS5011/ML-PA1/datasets/DS3/train_labels.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
pca = PCA(n_components=1)
X_new = pca.fit_transform(X_train)
regr = linear_model.LinearRegression()
regr.fit(X_new, y_train)
X_test_new = pca.transform(X_test)
predictdata = regr.predict(X_test_new)
prediction = predictdata
for x in xrange(0,predictdata.size):
	if(predictdata[x]>=0.5):
		prediction[x] = 1
	else:
		prediction[x] = 0
acc = accuracy_score(y_test, prediction)
prec = metrics.precision_score(y_test, prediction)
rec = metrics.recall_score(y_test, prediction)
f1 =  metrics.f1_score(y_test, prediction)  
print "LCA Analysis : %lf %lf %lf %lf" %(acc,prec,rec,f1)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in xrange(0,2000):
	if y_train[i] == 1:
		ax.scatter(X_train[i,0],X_train[i,1],X_train[i,2],c='r', marker='o')
	else:
		ax.scatter(X_train[i,0],X_train[i,1],X_train[i,2],c='b', marker='o')
#print X_train[:,0]
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')
sklearn_transf = pca.fit_transform(X_train)
A =sklearn_transf[0:200]
A[0:200] = 1
fig2 = plt.figure()
plt.plot(sklearn_transf[0:200],A,'o', markersize=7, color='blue', alpha=0.5, label='class1')
plt.plot(sklearn_transf[200:400],A, '^', markersize=7, color='red', alpha=0.5, label='class2')
plt.show()
#print X_train.shape
"""

for c, i in zip("rb", [1, 2]):
    plt.scatter(X_train[y_train == i, 0], X_train[y_train == i, 1], X_train[y_train == i, 2],c=c)
"""
#plt.legend()
plt.show()

clf = LinearDiscriminantAnalysis(n_components=1)
clf.fit(X_train, y_train)
X_test_new2 = clf.transform(X_test)
y_predict = clf.predict(X_test)
#y_predict[0]=2
acc = accuracy_score(y_test, y_predict)
prec = metrics.precision_score(y_test, y_predict)
rec = metrics.recall_score(y_test, y_predict)
f1 =  metrics.f1_score(y_test, y_predict)  
print "LDA Analysis : %lf %lf %lf %lf" %(acc,prec,rec,f1)


sklearn_transf = clf.transform(X_train)
A =sklearn_transf[0:200]
A[0:200] = 1
fig3 = plt.figure()
plt.plot(sklearn_transf[0:200],A,'o', markersize=7, color='blue', alpha=0.5, label='class1')
plt.plot(sklearn_transf[200:400],A, '^', markersize=7, color='red', alpha=0.5, label='class2')
plt.show()