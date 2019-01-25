import numpy as np
import matplotlib.pyplot as plt

d = [0.5, 5]
print d

a = np.loadtxt('o',delimiter = ',')
time0 = count0 = 0
time1 = count1 = 0
for i in xrange(96):
	if a[i][4] == 0.5:
		time0 += a[i][7]
		count0 += 1
	else:
		time1 += a[i][7]
		count1 += 1
print a.shape

latency = [time0*1.0/count0,time1*1.0/count1]

fig = plt.figure()
plt.scatter(d[0],latency[0],c='r',label = 'Drop% = 0.5')
plt.scatter(d[1],latency[1],c='b',label = 'Drop% = 5')
plt.xlabel("Drop percentage")
plt.ylabel("Latency")
plt.title('Effect of latency on whether drop percentage is 0.5% or 5%')
plt.legend(loc='upper left')
plt.savefig("dropl.png", bbox_inches='tight')
plt.close(fig)
