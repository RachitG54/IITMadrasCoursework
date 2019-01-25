import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model, datasets
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import accuracy_score

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

plt.ioff()


"""
iris = datasets.load_iris()
X = iris.data[:, :2]  # we only take the first two features.
Y = iris.target
print X.shape
print Y.shape
print Y
"""
accva = []
acctr = []
accte = []
cm = []
d = []
for k in xrange(10):
	d.append(k);
	poly = PolynomialFeatures(k)
	logreg = linear_model.LogisticRegression()
	logreg.fit(poly.fit_transform(X_train),Y_train);
	cm2 = confusion_matrix(Y_test, logreg.predict(poly.fit_transform(X_test)));
	print cm2
	if(len(cm)==0):
		cm = cm2
	else:
		cm = np.concatenate((cm, cm2));

	acc2 = accuracy_score(Y_train, logreg.predict(poly.fit_transform(X_train)));
	acctr.append(acc2);
	acc2 = accuracy_score(Y_test, logreg.predict(poly.fit_transform(X_test)));
	accte.append(acc2);
	acc2 = accuracy_score(Y_val, logreg.predict(poly.fit_transform(X_val)));
	accva.append(acc2);

	h = 0.01

	x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
	y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
	xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
	Z = logreg.predict(poly.fit_transform(np.c_[xx.ravel(), yy.ravel()]))
	xr = xx.ravel()
	yr = yy.ravel()

	Z1 = []
	Z2 = []
	Z3 = []
	X1 = []
	X2 = [] 
	X3 = []
	Y1 = []
	Y2 = []
	Y3 = []

	for i in xrange(Z.shape[0]):
		if(Z[i]==1):
			Z1.append(Z[i])
			X1.append(xr[i])
			Y1.append(yr[i])
		elif(Z[i]==2):
			Z2.append(Z[i])
			X2.append(xr[i])
			Y2.append(yr[i])
	
	X1 = np.asarray(X1);
	X2 = np.asarray(X2);

	Y1 = np.asarray(Y1);
	Y2 = np.asarray(Y2);


	# Put the result into a color plot
	fig = plt.figure(1, figsize=(16, 9))

	plt.scatter(X1,Y1,c = 'w', facecolor='0.5', lw = 0,label = 'class 1');
	plt.scatter(X2,Y2,c = 'c', facecolor='0.5', lw = 0,label = 'class 2');


	plt.scatter(train1[:,0],train1[:,1],c = 'k',label = 'train class 1');
	plt.scatter(train2[:,0],train2[:,1],c = 'r',label = 'train class 2');

	plt.xlabel('Feature 1')
	plt.ylabel('Feature 2')
	plt.title('Dataset1, No of Classes:2,Model:1(b),degree:'+str(k))
	plt.legend(loc='upper left')

	plt.xlim(xx.min(), xx.max())
	plt.ylim(yy.min(), yy.max())
	plt.xticks(())
	plt.yticks(())
	plt.savefig("1(b)"+str(k)+".png", bbox_inches='tight')
	plt.close(fig)

print "accuracy on test sets are:"
print accte
print "accuracy on train sets are:"
print acctr
print "accuracy on val sets are:"
print accva

print "confusion_matrixes on train are:"
print cm

fig = plt.figure()
plt.plot(d,accte,c='r',label = 'test')
plt.plot(d,accva,c='g',label = 'val')
plt.plot(d,acctr,c='b',label = 'train')

plt.xlabel("degree")
plt.ylabel("accuracy")
plt.title('Accuracy for dataset1(b)')
plt.legend(loc='upper left')
plt.ylim([0,1.1])
plt.savefig("1(b)Models.png", bbox_inches='tight')
plt.close(fig)
