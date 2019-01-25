import numpy as np
import math
import matplotlib.pyplot as plt
from sklearn import datasets, linear_model
from sklearn.metrics import accuracy_score
from sklearn.multiclass import OneVsRestClassifier
from sklearn import metrics
from sklearn import svm
from sklearn.svm import SVC

def read_d(file_name):
    f = open(file_name, "r")
    lines = f.readlines()
    f.close()
    nums = [int(a) for a in lines[1].split()]
    m = nums[0];
    #print(m);
    n = nums[1];
    #print(n);
    X = np.zeros((m,n))
    for j in xrange(0,n):
        for i in xrange(0,m):
            X[i][j] = (int)(lines[2+i+m*j])
            #print X[i][j]
    f.close()
    return X

def sigmoid(x):
    e = math.exp(-x)
    return 1/(1+e)

def firstlayer(alphaconst,alpha,X):
    n = len(X[:,0])
    m = len(alpha[:,0])
    p = len(alpha[0,:])
    Z = np.random.rand(n,m)
    Z = np.dot(X,alpha.transpose())
    for i in xrange(n):
        for j in xrange(m):
            Z[i][j] = sigmoid(Z[i][j])
    return Z


def deltafunc(y,actualy,beta,zoh,x1p,k,m,p):
    tau = np.random.rand(k,k)
    ydash = np.reshape(y,(k,1))
    y = np.reshape(y,(1,k))
    actualy = np.reshape(actualy,(1,k))
    tau = -np.dot(ydash,y)
    for i in xrange(k):
            tau[i][i] = y[0][i]*(1-y[0][i])
    deltao = np.subtract(y,actualy)
    deltao = np.dot(deltao,tau)
    deltao = deltao.transpose()
    betat = beta.transpose()
    deltah =  np.dot(betat,deltao)
    zoh2 = []
    zoh2[:] = 1-zoh[:]
    zoh = np.reshape(zoh,(1,m))
    zoh2 = np.reshape(zoh2,(1,m))
    temp = np.multiply(zoh,zoh2)
    temp = temp.transpose()
    deltah = np.multiply(deltah, temp)
    zoht = zoh.transpose()
    deltaot = deltao.transpose()
    gb = np.dot(zoht,deltaot)
    x1p = np.reshape(x1p,(1,p))
    x1pt = x1p.transpose()
    ga = np.dot(x1pt,deltah.transpose())    
    vallist = []
    vallist.append(gb)
    vallist.append(ga)

    return vallist


def softmax(Tarray):
    k = len(Tarray)
    output = Tarray
    sum1 = 0
    for i in xrange(k):
        sum1 += math.exp(Tarray[i])
    for i in xrange(k):
        output[i] = math.exp(Tarray[i])/sum1
    return output

def secondlayer(betaconst, beta, Z):
    k = len(beta[:,0])
    m = len(beta[0,:])
    n = len(Z[:,0])
    fk = np.random.rand(n,k)
    betat = beta.transpose()
    fk = np.dot(Z,betat)
    for i in xrange(n):
        fk[i][:] = softmax(fk[i][:])
    return fk
def measures(y_test,yout):
    acc = metrics.accuracy_score(y_test,yout)
    prec = metrics.precision_score(y_test,yout,average = "macro")
    rec = metrics.recall_score(y_test,yout,average = "macro")
    f1 =  metrics.f1_score(y_test,yout,average = "macro") 
    print "ANN Analysis, poly : acc,prec,rec,f1 are: %lf %lf %lf %lf" %(acc,prec,rec,f1)

def evaluate(yout):
    n = len(yout[:,0])
    k = len(yout[0,:])
    for i in xrange(n):
            maxim = -1
            maxin = -1
            for j in xrange(k):
                if(maxim<yout[i][j]):
                    maxin = j
                    maxim = yout[i][j]
            for j in xrange(k):
                if maxin!=j:
                    yout[i][j] = 0
                else:
                    yout[i][j] = 1
    return yout

def backpropagate( X, y, m, y_test,Xtest):
    n = len(X[:,0])
    a = X[:,0]
    p = len(X[0,:])
    k = len(y[0,:])
    rate_decent1 = 0.05
    rate_decent2 = 0.03
    alphaconst = np.random.rand(m)
    alpha = np.random.rand(m,p)
    beetaconst = np.random.rand(k)
    beta = np.random.rand(k,m)
    alphaconst[:] *= 1.4
    alphaconst[:] -=0.7
    beetaconst[:] *= 1.4
    beetaconst[:] -=0.7
    alpha[:,:] *= 1.4
    alpha[:,:] -=0.7
    beta[:,:] *= 1.4
    beta[:,:] -=0.7
    epochs = 0
    trainingerror = []
    epochslist = []
    while(epochs<=1000):
        Z = firstlayer(alphaconst,alpha,X)
        yout = secondlayer(beetaconst,beta,Z)
        diff = yout - y
        for i in xrange(n):
            anslist = deltafunc(yout[i,:],y[i,:],beta,Z[i][:],X[i,:],k,m,p)
            gbt = anslist[0].transpose()
            gbt[:,:] = gbt[:,:]*rate_decent2
            beta = np.subtract(beta,gbt)
            gat = anslist[1].transpose()
            gat[:,:] = gat[:,:]*rate_decent1
            alpha = np.subtract(alpha,gat)
        epochs+=40
        Z = firstlayer(alphaconst,alpha,X)
        yout = secondlayer(beetaconst,beta,Z)
        yout2 = yout
        yout2 = evaluate(yout2)
        acc = metrics.accuracy_score(y,yout2)
        if acc>0.9:
            break
        trainingerror.append(acc)
        epochslist.append(epochs)
    Z = firstlayer(alphaconst,alpha,Xtest)
    yout = secondlayer(beetaconst,beta,Z)
    yout = evaluate(yout)
    measures(y_test,yout)
    plt.plot(epochslist, trainingerror)
    plt.xlabel('epochs')
    plt.ylabel('accuracy')
    plt.show()


def backpropagatewithlambda( X, y, m, y_test,Xtest,lambdaval):
    n = len(X[:,0])
    a = X[:,0]
    p = len(X[0,:])
    k = len(y[0,:])
    rate_decent1 = 0.05
    rate_decent2 = 0.03
    alphaconst = np.random.rand(m)
    alpha = np.random.rand(m,p)
    beetaconst = np.random.rand(k)
    beta = np.random.rand(k,m)
    alphaconst[:] *= 1.4
    alphaconst[:] -=0.7
    beetaconst[:] *= 1.4
    beetaconst[:] -=0.7
    alpha[:,:] *= 1.4
    alpha[:,:] -=0.7
    beta[:,:] *= 1.4
    beta[:,:] -=0.7
    epochs = 0
    trainingerror = []
    epochslist = []
    while(epochs<=1000):
        Z = firstlayer(alphaconst,alpha,X)
        yout = secondlayer(beetaconst,beta,Z)
        diff = yout - y
        for i in xrange(n):
            anslist = deltafunc(yout[i,:],y[i,:],beta,Z[i][:],X[i,:],k,m,p)
            gbt = anslist[0].transpose()
            gbt[:,:] = gbt[:,:]*rate_decent2
            beta = np.subtract(beta,gbt)
            gat = anslist[1].transpose()
            gat[:,:] = gat[:,:]*rate_decent1
            alpha = np.subtract(alpha,gat)

        lambdamat = np.full(beta.shape,2*lambdaval*rate_decent2)
        lambdamat = np.multiply(lambdamat,beta)
        beta = np.subtract(beta,lambdamat)

        lambdamat = np.full(alpha.shape,2*lambdaval*rate_decent1)
        lambdamat = np.multiply(lambdamat,alpha)
        alpha = np.subtract(alpha,lambdamat)
        epochs+=1
        Z = firstlayer(alphaconst,alpha,X)
        yout = secondlayer(beetaconst,beta,Z)
        yout2 = yout
        yout2 = evaluate(yout2)
        acc = metrics.accuracy_score(y,yout2)
        if acc>0.9:
            break
        trainingerror.append(acc)
        epochslist.append(epochs)
    Z = firstlayer(alphaconst,alpha,Xtest)
    yout = secondlayer(beetaconst,beta,Z)
    yout = evaluate(yout)
    measures(y_test,yout)
    plt.plot(epochslist, trainingerror)
    plt.xlabel('epochs')
    plt.ylabel('accuracy')
    plt.show()



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



len1 = len(y_train2)
len2 = len(y_test2)
y_trainkary = np.zeros((len1,4))
y_testkary  = np.zeros((len2,4))

for x in xrange(len1):
    y_trainkary[x][y_train2[x]-1] = 1

for x in xrange(len2):
    y_testkary[x][y_test2[x]-1] = 1

X_trainnorm = X_train
X_testnorm = X_test
col = len(X_train[0,:])
row = len(X_train[:,0])
for x in xrange(col):
    mean = sum(X_train[:,x])/row
    column = X_train[:,x]
    a = [i*i for i in column]
    std = (sum(a)/row-(mean*mean))**(1/2.0)
    X_trainnorm[:,x] = [(i-mean)/(std) for i in column]

    mean = sum(X_test[:,x])/row
    column = X_test[:,x]
    b = [i*i for i in column]
    std = (sum(b)/row-(mean*mean))**(1/2.0)
    X_testnorm[:,x] = [(i-mean)/(std) for i in column]


#print y_trainkary
anslist = []
#backpropagate(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm)
backpropagatewithlambda(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm,0.01)
backpropagatewithlambda(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm,0.1)
backpropagatewithlambda(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm,1)
backpropagatewithlambda(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm,10)
backpropagatewithlambda(X_trainnorm,y_trainkary,40,y_testkary,X_testnorm,100)