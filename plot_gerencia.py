# -*- coding: UTF-8 -*-

from __future__ import division

import math
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

import numpy as np

TRAFFICS=(100, 200)

ALGORITHMS = ("UDP","TCP") #
#
X_LEGEND = 'Density (vehicles/km$^2$)'.decode('utf-8')
x_max = 750
formato = '.eps'

#CONFIDENCE INTERVAL - BAR ERROR
def confidence_interval(values):
	N = len(values)
	Z = 1.96
	std_dev = np.std(values)
	std_error = std_dev / math.sqrt(N)
	return Z * std_error

#DELAY
def plot_delay():
	delay_values = []
	confidence_intervals = []

	delay = float(0)

	values = []
	file_input_name =  "summary.txt"

	replication = 0
	delay = 0.0

	for line in open(file_input_name):
		if replication == int(line.split()[0]): 
			if line.split()[3] != "NA":
				#print line.split()[3]
				delay += float(line.split()[3])
				print delay
				values.append(delay)

		#else:
		#	# CHANGED
		#	if not delay == 0.0:
		#		values.append(delay)
		#		#delay = 0.0
		#		print "aqui"

		while replication != int(line.split()[0]):
			replication += 1
		
	values = np.divide(values,20)
	#print values

	delay_values.append(np.mean(values))
	#print(algorithm, " - mean:", np.mean(values))
	confidence_intervals.append(confidence_interval(values))
	print(" - confidence: ", confidence_interval(values))
	
	plt.errorbar(100, delay_values, yerr=confidence_intervals, label="UDP", color="r", marker="s", linestyle="-", markersize = 8, linewidth = 2)

	#yticks = np.arange(50, 110, 10)
	#plt.xlabel(X_LEGEND)
	plt.ylabel('Delay (s)')
	plt.xlim(50, x_max)
	#plt.ylim(0,1.5)
	#plt.xticks(TRAFFICS)
	#plt.yticks(yticks)
	#plt.xticks((60, 100, 150))
	
	#plt.yscale("log")
	
	plt.grid()    
	plt.legend(numpoints=1, loc=2)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('delay'+formato, dpi=200)

	print("OK!\n")
	plt.show()

#
							
if __name__ == "__main__":
	pass
	print("DELAY...")
	plot_delay()