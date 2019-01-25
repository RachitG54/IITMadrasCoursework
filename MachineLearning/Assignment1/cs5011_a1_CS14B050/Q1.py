#from scipy.stats import multivariate_normal
#var = multivariate_normal(mean=[0,0], cov=[[1,0],[0,1]])
#var.pdf([1,0])
#print var.pdf([1,0])
import sys
import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import accuracy_score
from sklearn import metrics
from sklearn.neighbors import KNeighborsClassifier

mean1 = np.zeros(20)
mean2 = np.full(20,1)
covariance = np.random.rand(20,20)
covariance[0][1] = covariance[1][0] = covariance[2][2] = 0.5
data1 = np.random.multivariate_normal(mean1,covariance,2000)
data2 = np.random.multivariate_normal(mean2,covariance,2000)
#print data1
#print "data 2 starts \n"
#print data2
counttrain = counttest = 0
traindata1 = np.zeros((1400,20))
traindata2 = np.zeros((1400,20))
testdata1 = np.zeros((600,20))
testdata2 = np.zeros((600,20))
totaltraindata = np.zeros((2800,20))
totaltargettraindata = np.zeros(2800)
totaltestdata = np.zeros((1200,20))
totaltargettestdata = np.zeros(1200)

index1 = index2 = 0
for i in xrange(0,2000):
	x = np.random.rand()
	if(x>=0.7):
		testdata1[counttest] = data1[i]
		testdata2[counttest] = data2[i]
		counttest+=1
		if(counttest==600):
			break
	else:
		traindata1[counttrain] = data1[i]
		traindata2[counttrain] = data2[i]
		counttrain+=1
		if(counttrain==1400):
			break
#print "%d %d" %(counttest,i)
if counttest<600:
	for x in xrange(i+1,2000):
		testdata1[counttest] = data1[x]
		testdata2[counttest] = data2[x]
		counttest+=1

if counttrain<1400:
	for x in xrange(i+1,2000):
		traindata1[counttrain] = data1[x]
		traindata2[counttrain] = data2[x]
		counttrain+=1

#Q2.py starts
for x in xrange(0,600):
	totaltestdata[x] = testdata1[x]
	totaltargettestdata[x] = 0
for x in xrange(600,1200):
	totaltestdata[x] = testdata2[x-600]
	totaltargettestdata[x] = 1
regr = linear_model.LinearRegression()
for x in xrange(0,1400):
	totaltraindata[x] = traindata1[x]
	totaltargettraindata[x] = 0
for x in xrange(1400,2800):
	totaltraindata[x] = traindata2[x-1400]
	totaltargettraindata[x] = 1
regr.fit(totaltraindata, totaltargettraindata)
#totaltestdata = np.concatenate((testdata1,testdata2),0)
predictdata = np.zeros(1200)
prediction = np.zeros(1200)
predictdata = regr.predict(totaltestdata)
print "Coefficients for linear regression are: "
print regr.coef_ 
for x in xrange(0,1200):
	if(predictdata[x]>=0.5):
		prediction[x] = 1
	else:
		prediction[x] = 0
acc = accuracy_score(totaltargettestdata, prediction)
#print prediction
#print acc
prec = metrics.precision_score(totaltargettestdata, prediction)
rec = metrics.recall_score(totaltargettestdata, prediction)
f1 =  metrics.f1_score(totaltargettestdata, prediction)
print "accuracy precision recall  f-measure"  
print "%lf %lf %lf %lf" %(acc,prec,rec,f1)


#Q3 starts here

knnprediction = np.zeros(1200)
minkerror = 0
minkvalue = 0
for k in xrange(1,300):
	#print knn
	knn = KNeighborsClassifier(n_neighbors=k)
	knn.fit(totaltraindata, totaltargettraindata)
	knnpredictdata = knn.predict(totaltestdata)
	for x in xrange(0,1200):
		if(knnpredictdata[x]>=0.5):
			knnprediction[x] = 1
		else:
			knnprediction[x] = 0
	acc = accuracy_score(totaltargettestdata, knnprediction)
	prec = metrics.precision_score(totaltargettestdata, knnprediction)
	rec = metrics.recall_score(totaltargettestdata, knnprediction)
	f1 =  metrics.f1_score(totaltargettestdata, knnprediction)
	if(minkerror>acc):
		minkerror = acc
		minkvalue = k  
	#print "with k = %d, %lf %lf %lf %lf" %(k,acc,prec,rec,f1)
print "value of k for which error was minimum is %d" %(minkvalue)

f = open("DS1.csv", 'w')
sys.stdout = f
for x in xrange(0,600):
	for y in xrange(0,19):
		f.write("%.2f," %(testdata1[x][y]))
	f.write("%.2f\n" %(testdata1[x][19]))
for x in xrange(0,600):
	for y in xrange(0,19):
		f.write("%.2f," %(testdata2[x][y]))
	f.write("%.2f\n" %(testdata2[x][19]))
f.close()

f = open("DS1_labels.csv", 'w')
sys.stdout = f
for x in xrange(0,600):
	print "0"
for x in xrange(600,1200):
	print "1"
f.close()
"""
thedata = np.genfromtxt(
    'DS1.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1       # fill missing values with -1-
    #usecols = range(5,128)    # columns to read
    )
np.savetxt(
    'newDS1.csv',           # file name
    thedata,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )
""" 