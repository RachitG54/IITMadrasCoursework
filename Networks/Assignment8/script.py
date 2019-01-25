import os
import subprocess 
import shlex
import commands

filearr = ["sample.txt","sample2.txt","sample3.txt"]
def run_command(file):
	addr = '192.168.0.5:10000'
	wo = commands.getstatusoutput("wget -r {}/{}".format(addr,file))
	l = wo[1].split("\n")
	s1 = (wo[1].split("\n"))[len(l)-1]
	words = s1.split(" ")
	word1 = words[len(words)-2]
	word2 = words[len(words)-3]
	throughput = word1.split("(")[1]
	latency = word2.split("s")[0]
	return (throughput,latency)

sack = [0,1]
windowsize = [16,256]
tcptype = [18,3]
ldelay = [2,50]
drop = [0.5,5]
sudoPassword = "5x*M"
command = "tc -s qdisc ls dev eth0"
os.system('echo %s|sudo -S %s' % (sudoPassword, command))

for q1 in sack:
	if(q1==0):
		os.system('echo %s|sudo -S %s' % (sudoPassword, "sudo sysctl -w net.ipv4.tcp_sack=\"0\"" ))
	else:
		os.system('echo %s|sudo -S %s' % (sudoPassword, "sudo sysctl -w net.ipv4.tcp_sack=\"1\"" ))
	for q2 in windowsize:
		if(q2==16):
			os.system('echo %s|sudo -S %s' % (sudoPassword, "sudo sysctl -w net.ipv4.tcp_window_scaling=\"1\"" ))
			command = 'sudo sysctl -w net.core.rmem_max=16384'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			command = 'sudo sysctl -w net.ipv4.tcp_rmem=16384'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			command = 'sudo sysctl -w net.ipv4.tcp_wmem=16384'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			coommand = 'sudo sysctl -p /etc/sysctl.conf'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))

		else:
			os.system('echo %s|sudo -S %s' % (sudoPassword, "sudo sysctl -w net.ipv4.tcp_window_scaling=\"1\"" ))
			command = 'sudo sysctl -w net.core.rmem_max=262144'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			command = 'sudo sysctl -w net.ipv4.tcp_rmem=262144'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			command = 'sudo sysctl -w net.ipv4.tcp_wmem=262144'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			coommand = 'sudo sysctl -p /etc/sysctl.conf'
			os.system('echo %s|sudo -S %s' % (sudoPassword, command))
		for q3 in tcptype:
			if(q3==18):
				command = "sudo bash -c \'echo reno > /proc/sys/net/ipv4/tcp_congestion_control\'"
				os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			else:
				command = "sudo bash -c \'echo cubic > /proc/sys/net/ipv4/tcp_congestion_control\'"
				os.system('echo %s|sudo -S %s' % (sudoPassword, command))
			for q4 in ldelay:
				if(q4==2):
					command = "sudo tc qdisc change dev eth0 root netem delay 2ms 0.2ms 25%"
					os.system('echo %s|sudo -S %s' % (sudoPassword, command))
				else:
					command = "sudo tc qdisc change dev eth0 root netem delay 50ms 5ms 25%"
					os.system('echo %s|sudo -S %s' % (sudoPassword, command))
				for q5 in drop:
					if(q5==0.5):
						command = "sudo tc qdisc change dev eth0 root netem loss 0.5\% 25\%"
						os.system('echo %s|sudo -S %s' % (sudoPassword, command))
					else:
						command = "sudo tc qdisc change dev eth0 root netem loss 5\% 25\%"
						os.system('echo %s|sudo -S %s' % (sudoPassword, command))

					for j in xrange(3):
						out1 = 0
						out2 = 0
						for i in xrange(5):
							(th,lat) = run_command(filearr[j])
							out1 += float(th)
							out2 += float(lat)
						out1 = out1/5.0
						out2 = out2/5.0
						print ("%d,%d,%d,%d,%f,%d,%f,%f" %(q1,q2,q3,q4,q5,j,out1,out2))
