import numpy as np
from sklearn import datasets, linear_model
from sklearn.cross_validation import train_test_split
from sklearn.linear_model import Ridge

thedata = np.genfromtxt(
    'communities.csv',           # file name
    skip_header=0,          # lines to skip at the top
    skip_footer=0,          # lines to skip at the bottom
    delimiter=',',          # column delimiter
    filling_values=-1,       # fill missing values with -1-
    usecols = range(5,128)    # columns to read
    )    
"""                       
print("\n Printing the data from the csv file.\n")
for row in thedata:
    print(row)
"""
#print thedata.shape
thedatamedian = thedata
thedatarandom = thedata
for i in xrange(0,123):
	mean = count = stddev = 0
	listval = []
	for j in xrange(0,1994):
		if thedata[j][i]>0 :
			count+=1
			mean +=thedata[j][i]
			stddev += thedata[j][i]*thedata[j][i]
			listval.append(thedata[j][i])
	listval.sort()
	if count%2 ==1:
		median = listval[(count-1)/2]
	else:
		median = (listval[count/2] + listval[count/2-1])/2
	mean/=count
	stddev/=count
	stddev-=mean*mean
	stddev = np.sqrt(stddev)
	for j in xrange(0,1994):
		if thedata[j][i]<0 :
			thedata[j][i] = mean
			thedatamedian[j][i] = median
			thedatarandom[j][i] = mean -stddev+2*stddev*np.random.rand()
np.savetxt(
    'Q4data.csv',           # file name
    thedata,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )     
X = thedata[:,0:122]
y = thedata[:,122]
np.savetxt(
    'DS2.csv',           # file name
    X,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )
np.savetxt(
    'DS2_labels.csv',           # file name
    y,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )    
X_trainlist = [] 
X_testlist = [] 
y_trainlist = [] 
y_testlist = []
regrcoeff = np.zeros((5,122))
avgerror = 0
for i in xrange(0,5):
	X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
	X_trainlist.append(X_train)
	X_testlist.append(X_test)
	y_trainlist.append(y_train)
	y_testlist.append(y_test)
	regr = linear_model.LinearRegression()
	regr.fit(X_train, y_train)
	reserror = np.mean((regr.predict(X_test) - y_test) ** 2)
	regrcoeff[i] = regr.coef_
	#print "coefficients of linear regression are:"
	#print regrcoeff 
	avgerror += reserror    
	print "The residual error on best fit is %f" %(reserror)
print "The average residual error on best fit over 5 splits is %f" %(avgerror/5)
#print regrcoeff
np.savetxt(
    'Q5coeff.csv',           # file name
    regrcoeff,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character
    comments='# '          # character to use for comments
    )  
for i in xrange(0,5):
	totaltrain = np.zeros((1595,123))
	totaltrain[:,0:122] = X_trainlist[i][:,:]
	totaltrain[:,122] = y_trainlist[i][:]
	totaltest = np.zeros((399,123))
	totaltest[:,0:122] = X_testlist[i][:,:]
	totaltest[:,122] = y_testlist[i][:]
	np.savetxt(
	    'CandC-train<%d>.csv'%(i+1),           # file name
	    totaltrain,                # array to save
	    fmt='%.2f',             # formatting, 2 digits in this case
	    delimiter=',',          # column delimiter
	    newline='\n',           # new line character
	    comments='# '          # character to use for comments
	    )
	np.savetxt(
	    'CandC-test<%d>.csv'%(i+1),           # file name
	    totaltest,                # array to save
	    fmt='%.2f',             # formatting, 2 digits in this case
	    delimiter=',',          # column delimiter
	    newline='\n',           # new line character
	    comments='# '          # character to use for comments
	    )  
#print y_trainlist[0]
#print X_testlist[i].shape
#Q6
rdgecoeff = np.zeros((5,122))
avgerror = 0
for i in xrange(0,5):
	X_train = X_trainlist[i]
	X_test = X_testlist[i]
	y_train = y_trainlist[i]
	y_test = y_testlist[i]
	minerror = 1000000;
	minalpha = 0
	print "split number %d:" %(i)
	for j in xrange(0,100):
		rdge = Ridge(alpha = j*0.1)
		rdge.fit(X_train, y_train)
		reserror = np.mean((rdge.predict(X_test) - y_test) ** 2)
		if(reserror<minerror):
			rdgecoeff[i] = rdge.coef_
			minalpha = j*0.1 
			minerror = reserror    
	print "with alpha = %lf, error is %lf" %(minalpha,minerror)	
	avgerror+=minerror
print "The average residual error on doing l1 regularized linear regression over 5 splits is %f" %(avgerror/5)
np.savetxt(
    'Q6coeff.csv',           # file name
    rdgecoeff,                # array to save
    fmt='%.2f',             # formatting, 2 digits in this case
    delimiter=',',          # column delimiter
    newline='\n',           # new line character]
    comments='# '          # character to use for comments
    )
 