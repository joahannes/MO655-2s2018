#!/usr/bin/env python

import os

TRAFFICS = (2, 4, 6)
REPLICATIONS = 5

# (0) Replication, (1) Host, (2) Clientes, (3) Vazao, (4) Delay, (5) Perdidos

def build_summary_files():

	for traffic in TRAFFICS:
		summary_file = open("summary-" + str(traffic) + ".txt", "w")

		for replication in range(REPLICATIONS):
			#print replication
			result_file = open( "results-" + str(traffic) + "-est-udp-" + str(replication) + ".txt")
			line = result_file.readline()

			#print line
			while line:
				if "FlowID[" in line:
					line_splited = line.split()

					#Vazao
					if "Clientes" in line:
						host = line_splited[0].split("]")[0][7:]
						clientes = line_splited[2]
						summary_file.write(str(replication) + "\t" + host + "\t" + clientes + "\t")
					#Vazao
					if "Throughput(Mbps)" in line:
						throughput = line_splited[2]
						summary_file.write(throughput + "\t")
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
