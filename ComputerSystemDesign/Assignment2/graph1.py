import matplotlib.pyplot as plt
tries = 6
assocs = [16, 32, 48, 60, 70, 80]
colors = ['b','r','g','c', 'm', 'y']


plt.figure(figsize=(20, 7))
for i in range(tries):
	assoc = assocs[i]
	filename = "assoc-" + str(assoc) + ".txt"
	with open(filename) as f:
		lines = f.readlines()
	lines = [x.strip() for x in lines]
	plt.plot(range(len(lines)),lines, '.', c=colors[i],label = assoc,markersize=2)

plt.ylabel('latency(cycles)')
plt.xlabel('set serial number')
plt.title('latency vs set serial number')
plt.legend(loc='best')
plt.savefig('graph1.png')