import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import accuracy_score
from sklearn import metrics

def read_d(file_name, m,n):
    X = np.zeros((m,n))
    f = open(file_name, "r")
    lines = f.readlines()
    f.close()
    for j in xrange(0,n):
        for i in xrange(0,m):
            X[i][j] = (int)(lines[2+i+m*j])
            #print X[i][j]
    f.close()
    return X

X_train = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Train_features",518,96)
y_train = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Train_labels",518,1)
X_test = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Test_features",40,96)
y_test = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/Test_labels",40,1)
y_predict = read_d("/home/rachit/Documents/Sem5/CS5011/ML-PA1/Data_LR(DS2)/data_students/result",40,1)

y_train2 = y_train[:,0]
y_test2 = y_test[:,0]
y_predict2 = y_predict[:,0]

logistic = linear_model.LogisticRegression()
logistic.fit(X_train,y_train2)
prediction = logistic.predict(X_test)
acc = accuracy_score(y_test2, prediction)
prec = metrics.precision_score(y_test2, prediction)
rec = metrics.recall_score(y_test2, prediction)
f1 =  metrics.f1_score(y_test2, prediction)  
print "Logistic Regression Analysis : %lf %lf %lf %lf" %(acc,prec,rec,f1)

#print y_predict2
#print y_test2   
acc = accuracy_score(y_test2, y_predict2)
prec = metrics.precision_score(y_test2, y_predict2)
rec = metrics.recall_score(y_test2, y_predict2)
f1 =  metrics.f1_score(y_test2, y_predict2)  
print "Logistic Regression Analysis( L1 regularized) : %lf %lf %lf %lf" %(acc,prec,rec,f1)
