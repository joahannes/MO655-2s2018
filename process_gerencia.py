#!/usr/bin/env python

import os

ALGORITHMS = ("Flooding", "GEDDAI")
REPLICATIONS = 4

# Replication, Host, Vazao, Delay, Perdidos, Jitter

def build_summary_files():

	summary_file = open("summary.txt", "w")

	for replication in range(REPLICATIONS):
		#print replication
		result_file = open( "results-udp-" + str(replication) + ".txt")
		line = result_file.readline()

		#print line
		while line:
			if "FlowID[" in line:
				line_splited = line.split()

				#Vazao
				if "Throughput(Mbps)" in line:
					host = line_splited[0].split("]")[0][7:]
					throughput = line_splited[2]
					summary_file.write(str(replication) + "\t" + host + "\t" + throughput + "\t")
				#Delay
				if "Delay" in line:
					delay = line_splited[2]
					summary_file.write(delay + "\t")
				#LostPackets
				if "LostPackets" in line:
					lostpackets = line_splited[2]
					summary_file.write(lostpackets + "\n")
				# #Jitter
				# if "Jitter" in line:
				# 	jitter = line_splited[2]
				# 	summary_file.write(jitter + "\n")

			line = result_file.readline()
		result_file.close()
		print "Arquivo", result_file.name ,"lido;\n"
	summary_file.close()

if __name__ == "__main__":
	pass
	build_summary_files()