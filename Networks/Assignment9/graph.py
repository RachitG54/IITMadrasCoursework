import subprocess 
import matplotlib.pyplot as plt
import numpy as np;

#subprocess.check_output('./cw -i 1 -m 1 -n 0.5 -f 0.1 -s 0.01 -T 100 -o o.txt',shell = True)
a = np.loadtxt("o.txt",delimiter = ',');
#print a;
#print a.shape
fig = plt.figure()
plt.title('Congestion window value vs iterations')
plt.xlabel('iterations')
plt.ylabel('congestion window value')
plt.plot(a[:,0],a[:,1])
plt.show()

"""
ki = [1,4]
km = [1,1.5]
kn = [0.5,1]
kf = [0.1,0.3]
ps = [0.0001,0.01]
for a in ki:
	for b in km:
		for c in kn:
			for d in kf:
				for e in ps:
					cmd = './cw -i ' +str(a)+' -m ' +str(b)+' -n ' +str(c)+' -f ' +str(d)+' -s ' +str(e)+' -T 500 -o o.txt'
					subprocess.check_output(cmd,shell = True)
					t = np.loadtxt("o.txt",delimiter = ',');	
					fig = plt.figure()
					plt.title('Congestion window value vs iterations')
					plt.xlabel('iterations')
					plt.ylabel('congestion window value')
					plt.plot(t[:,0],t[:,1],label = cmd)
					plt.legend(loc='best')
					plt.savefig('fig/'+cmd+'.png', bbox_inches='tight')
					plt.close(fig)
"""
