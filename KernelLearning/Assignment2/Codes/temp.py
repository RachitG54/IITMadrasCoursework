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


poly = PolynomialFeatures(2)
logreg = linear_model.LogisticRegression()
logreg.fit(poly.fit_transform(X_train),Y_train);
cm2 = confusion_matrix(Y_test, logreg.predict(poly.fit_transform(X_test)));
np.savetxt('tempb.txt', Y_test, delimiter=',',fmt = '%d')
np.savetxt('temp2b.txt', logreg.predict(poly.fit_transform(X_test)), delimiter=',',fmt = '%d')
