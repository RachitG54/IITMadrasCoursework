import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets, linear_model,grid_search,cross_validation
from sklearn.metrics import accuracy_score
from sklearn.multiclass import OneVsRestClassifier
from sklearn import metrics
from sklearn.preprocessing import normalize
from sklearn import svm
from sklearn.svm import SVC
from sklearn.cross_validation import cross_val_score
from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from scipy import io as sio

models = {}

def read_d(file_name):
    f = open(file_name, "r")
    lines = f.readlines()
    f.close()
    nums = [int(a) for a in lines[1].split()]
    m = nums[0];
    n = nums[1];
    X = np.zeros((m,n))
    for j in xrange(0,n):
        for i in xrange(0,m):
            X[i][j] = (int)(lines[2+i+m*j])
    f.close()
    return X
"""
X_train = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Train_features")
y_train = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Train_labels")
X_test = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Test_features")
y_test = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Test_labels")

y_train2 = y_train[:,0]
y_test2 = y_test[:,0]

DStrain = np.c_[X_train, y_train2]
DStest = np.c_[X_test, y_test2]

np.savetxt(
    'DS2_train.csv',           # file name
    DStrain,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )
np.savetxt(
    'DS2_test.csv',           # file name
    DStest,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )
"""

traindata = np.genfromtxt(
    './Dataset/DS2_train.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
testdata = np.genfromtxt(
    './Dataset/DS2_test.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
col = len(traindata[0,:])
X_train = traindata
X_test = testdata
y_train = X_train[:,col-1]
y_test = X_test[:,col-1]
X_train = np.delete(traindata, col-1, axis=1)
X_test = np.delete(testdata, col-1, axis=1)
y_train2 = y_train
y_test2 = y_test

X_trainnorm = X_train
X_testnorm = X_test
col = len(X_train[0,:])
row = len(X_train[:,0])
for x in xrange(col):
    mean = sum(X_train[:,x])/row
    column = X_train[:,x]
    a = [i*i for i in column]
    std = (sum(a)/row-(mean*mean))**(1/2.0)
    X_trainnorm[:,x] = [(i-mean)/(2*std) for i in column]

    mean = sum(X_test[:,x])/row
    column = X_test[:,x]
    b = [i*i for i in column]
    std = (sum(b)/row-(mean*mean))**(1/2.0)
    X_testnorm[:,x] = [(i-mean)/(2*std) for i in column]

maxacc = 0
degchoice = coefchoice = 0
C = [0.1,1,2,10,100]
C2 = [1]
y = [0.1, 0.01, 0.001, 0.0001, 0.00001]
gamma_range = gamma_range = np.logspace(-9, 3, 13)
tuned_parameters = [{'kernel': ['rbf'], 'gamma': [1e-3, 1e-4],
                     'C': [1, 10, 100, 1000]},
                    {'kernel': ['linear'], 'C': [1, 10, 100, 1000]}]


bestpara = 0
maxacc = 0
# for para1 in C:
#     clf = svm.SVC(decision_function_shape='ovo',C = para1,kernel = 'linear')
#     prediction = cross_validation.cross_val_predict(clf, X_trainnorm,y_train2, cv=4)
#     acc = metrics.accuracy_score(y_train2, prediction)
#     parameters = clf.get_params()

#     if(acc>maxacc):
#         maxacc = acc
#         bestpara = para1

# print "SVM Analysis, linear : %lf, slack is %lf"%(maxacc,bestpara)

# clf = svm.SVC(decision_function_shape='ovo',C = bestpara,kernel = 'linear')
# clf.fit(X_trainnorm, y_train2)
# prediction = clf.predict(X_testnorm)
# acc = metrics.accuracy_score(y_test2, prediction)
# prec = metrics.precision_score(y_test2, prediction,average = "macro")
# rec = metrics.recall_score(y_test2, prediction,average = "macro")
# f1 =  metrics.f1_score(y_test2, prediction,average = "macro")  
# print "SVM Analysis, linear : acc,prec,rec,f1 are: %lf %lf %lf %lf" %(acc,prec,rec,f1)
# models['model1'] = clf.support_vectors_


# degrees  = []
# precisionlist = []


# bestpara = 0


# for deg in xrange(2,14):
#     maxpdegreeacc = degval = 0
#     for para1 in C2:
#         for c0 in [0+0.25 * i for i in range(15)]:
#             clf = svm.SVC(decision_function_shape='ovo',C = para1,kernel = 'poly',degree = deg,coef0 = c0)
#             prediction = cross_validation.cross_val_predict(clf, X_trainnorm,y_train2, cv=4)
#             #acc = accuracy_score(y_test2, prediction)
#             acc = metrics.accuracy_score(y_train2, prediction)
#             parameters = clf.get_params()
#             if(acc>maxacc):
#                 bestpara = para1
#                 maxacc = acc
#                 degchoice = deg
#                 coefchoice = c0
#             if(acc>maxpdegreeacc):
#                 maxpdegreeacc = acc
#                 degval = deg
#     precisionlist.append(maxpdegreeacc)
#     degrees.append(degval)
            
# print "SVM Analysis, poly : %lf, coef is %lf, degree is %lf,para is %lf"%(maxacc,coefchoice,degchoice,bestpara)
# clf = svm.SVC(decision_function_shape='ovo',C = bestpara,kernel = 'poly',degree = degchoice,coef0 = coefchoice)
# clf.fit(X_trainnorm, y_train2)
# prediction = clf.predict(X_testnorm)
# acc = metrics.accuracy_score(y_test2, prediction)
# prec = metrics.precision_score(y_test2, prediction,average = "macro")
# rec = metrics.recall_score(y_test2, prediction,average = "macro")
# f1 =  metrics.f1_score(y_test2, prediction,average = "macro")  
# print "SVM Analysis, poly : acc,prec,rec,f1 are: %lf %lf %lf %lf" %(acc,prec,rec,f1)
# models['model2'] = clf.support_vectors_

# plt.plot(degrees, precisionlist)
# plt.xlabel('Degree of polymonial kernel')
# plt.ylabel('Precision of classification')
# plt.show()

gamma_array = []
precisionlist = []
maxacc=0
# for gam in [0.0005*e for e in range(1, 21)]:
#     maxpgama = 0
#     for para1 in [0.2 * i for i in range(1, 11)]:
#         clf = svm.SVC(decision_function_shape='ovo',C = para1,kernel = 'rbf',gamma = gam)
#         prediction = cross_validation.cross_val_predict(clf, X_trainnorm,y_train2, cv=4)
#         acc = metrics.accuracy_score(y_train2, prediction)
#         parameters = clf.get_params()
#         if(acc>maxacc):
#             maxacc = acc
#             bestpara = para1
#             coefchoice = gam
#         if(acc>maxpgama):
#             maxpgama = acc
#     precisionlist.append(maxpgama)
#     gamma_array.append(gam)

# print "SVM Analysis, rbf : %lf, slack is %lf, gamma is %lf"%(maxacc,bestpara,coefchoice)

plt.plot(gamma_array, precisionlist)
plt.xlabel('log(Gamma) for RBF kernel')
plt.ylabel('Precision of classification')
plt.show()

clf = svm.SVC(decision_function_shape='ovo',C = 1.8,kernel = 'rbf',gamma = 0.0095)
clf.fit(X_trainnorm, y_train2)
prediction = clf.predict(X_testnorm)
acc = metrics.accuracy_score(y_test2, prediction)
prec = metrics.precision_score(y_test2, prediction,average = "macro")
rec = metrics.recall_score(y_test2, prediction,average = "macro")
f1 =  metrics.f1_score(y_test2, prediction,average = "macro")  
print "SVM Analysis, rbf acc,prec,rec,f1 are: %lf %lf %lf %lf" %(acc,prec,rec,f1)
#models['model3'] = clf.support_vectors_



# gamma_array = []
# precisionlist = []
# gammachoice = 0
# maxacc = 0
# for gam in [0.002 * e for e in range(1, 41)]:
#     maxpgama = 0
#     for para1 in C2:
#         for c0 in [0.2 * i for i in range(1, 11)]:
#             clf = svm.SVC(decision_function_shape='ovo',C = para1,kernel = 'sigmoid',gamma = gam,coef0 = c0)
#             # clf.fit(X_trainnorm, y_train2)
#             # prediction = clf.predict(X_testnorm)
#             # acc = accuracy_score(y_test2, prediction)
#             prediction = cross_validation.cross_val_predict(clf, X_trainnorm,y_train2, cv=4)
#             acc = metrics.accuracy_score(y_train2, prediction)
#             parameters = clf.get_params()
#             #prec = metrics.precision_score(y_test2, prediction)
#             #rec = metrics.recall_score(y_test2, prediction)
#             #f1 =  metrics.f1_score(y_test2, prediction)  
#             #print "SVM Analysis : %lf %lf %lf %lf" %(acc,prec,rec,f1)
#             if(acc>maxacc):
#                 maxacc = acc
#                 gammachoice = gam
#                 coefchoice = c0
#             if(acc>maxpgama):
#                 maxpgama =acc
#     precisionlist.append(maxpgama)
#     gamma_array.append(gam)

# print "SVM Analysis, sigmoid : maxacc is %lf, coef is %lf, gamma is %lf"%(maxacc,coefchoice,gammachoice)


# clf  = svm.SVC(decision_function_shape='ovo',C = para1,kernel = 'sigmoid',gamma = gammachoice,coef0 = coefchoice)
# clf.fit(X_trainnorm, y_train2)
# prediction = clf.predict(X_testnorm)
# acc = metrics.accuracy_score(y_test2, prediction)
# prec = metrics.precision_score(y_test2, prediction,average = "macro")
# rec = metrics.recall_score(y_test2, prediction,average = "macro")
# f1 =  metrics.f1_score(y_test2, prediction,average = "macro")  
# print "SVM Analysis, sigmoid acc,prec,rec,f1 are: %lf %lf %lf %lf" %(acc,prec,rec,f1)


# plt.plot(gamma_array, precisionlist)
# plt.xlabel('Gamma for Sigmoid kernel')
# plt.ylabel('Accuracy of classification')
# plt.show()


# models['model4'] = clf.support_vectors_







# sio.savemat('CS14B050', models)


