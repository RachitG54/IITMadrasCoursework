import matplotlib.pyplot as plt
tries = 1
assocs = [48]
colors = ['b','r','g','c', 'm', 'y']
doublescan = ['-up','-down']


plt.figure(figsize=(20, 7))
for i in range(tries):
	assoc = assocs[i]
	filename = "assoc-" + str(assoc) + "-up.txt"
	with open(filename) as f:
		lines = f.readlines()
	lines = [x.strip() for x in lines]
	plt.plot(range(len(lines)),lines, '.', c=colors[i],label = str(assoc)+doublescan[0])
	filename = "assoc-" + str(assoc) + "-down.txt"
	with open(filename) as f:
		lines = f.readlines()
	lines = [x.strip() for x in lines]
	plt.plot(range(len(lines)),lines, '.', c=colors[i+1],label = str(assoc)+doublescan[1])

plt.ylabel('latency(cycles)')
plt.xlabel('set serial number')
plt.title('latency vs set serial number')
plt.legend(loc='best')
plt.savefig('graph2.png')