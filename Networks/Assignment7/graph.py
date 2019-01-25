import subprocess 
import matplotlib.pyplot as plt
W = 2
p = [0.01,0.02,0.03,0.05,0.1]
val = []
tries = 5
for x in p:
	a = 0
	for i in xrange(tries):
		b = subprocess.check_output(['./SlottedAloha','-N','50','-p',(str)(x), '-W', '2', '-M', '5000','-G'])
		a = a+float(b);
	val.append(a/5.0)
val2 = []
W = 4
for x in p:
	a = 0
	for i in xrange(tries):
		b = subprocess.check_output(['./SlottedAloha','-N','50','-p',(str)(x), '-W', '4', '-M', '5000','-G'])
		a = a+float(b);
	val2.append(a/5.0)


plt.figure()
plt.plot(p,val,c='r',label = 'W=2')
plt.plot(p,val2,c='b',label = 'W=4')
plt.title('Utilization percentage vs probability')
plt.legend(loc='upper left')

plt.show()