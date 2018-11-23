# -*- coding: UTF-8 -*-

from __future__ import division

import math
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

import numpy as np

TRAFFICS=(1,2,4,6,)
#TRAFFICS=(6,12,24,48,)
ALGORITHMS = ("UDP", "TCP", "AMBOS") 
COLORS = {"UDP" : "k", "TCP" : "r", "AMBOS" : "g"}
MARKS = {"UDP" : "H", "TCP" : "^","AMBOS" : "s"}
LINES = {"UDP" : "-", "TCP" : "--", "AMBOS" : "-."}

mobilidade = "est" #Deixar automatico

X_LEGEND = 'Número de clientes'.decode('utf-8')
formato = '.eps'

LEGEND = ("CBR", "Rajada", "CBR/Rajada")

# (0) Replication, (1) Host, (2) Clientes, (3) Vazao, (4) Delay, (5) Perdidos, (6) Transmitidos

#CONFIDENCE INTERVAL - BAR ERROR
def confidence_interval(values):
	N = len(values)
	Z = 1.96
	std_dev = np.std(values)
	std_error = std_dev / math.sqrt(N)
	return Z * std_error

#DELAY
def plot_delay():

	legend = 0

	for algorithm in ALGORITHMS:
		delay_values = []
		confidence_intervals = []
		delay = float(0)

		for traffic in TRAFFICS:
			values = []
			file_input_name = algorithm + "/summary-" + str(traffic) + "-" + mobilidade + ".txt"

			replication = 0
			delay = 0.0

			for line in open(file_input_name):
				if replication == int(line.split()[0]):
					if float(line.split()[4]) > 0.0000:
						delay += float(line.split()[4])

				else:
					if not delay == 0.0000:
						delay += float(line.split()[4])
						values.append(delay)
						delay = 0.0
						
				while replication != int(line.split()[0]):
					replication += 1
			

			values = np.divide(values,traffic)

			delay_values.append(np.mean(values))
			confidence_intervals.append(confidence_interval(values))
		
		plt.errorbar(TRAFFICS, delay_values, yerr=confidence_intervals, label=LEGEND[legend], color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)
		legend += 1

	plt.xlabel(X_LEGEND)
	plt.ylabel('Atraso (ms)')
	#plt.xlim(0, 7)
	plt.xlim(4, 50) #descomentar para 6 a 48 nos
	plt.xticks(TRAFFICS)
	
	plt.grid()    
	plt.legend(numpoints=1, loc=1)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('delay_estatico'+formato, dpi=200, bbox_inches = 'tight', pad_inches = 0.05)

	print("OK!\n")
	plt.show()

#PERDA
def plot_vazao():

	legend = 0

	for algorithm in ALGORITHMS:

		vazao_values = []
		confidence_intervals = []
		vazao = float(0)

		for traffic in TRAFFICS:
			values = []
			file_input_name = algorithm + "/summary-" + str(traffic) + "-" + mobilidade + ".txt"
			replication = 0
			vazao = 0.0

			for line in open(file_input_name):
				if replication == int(line.split()[0]): 
					if float(line.split()[3]) > 0.001:
						vazao += float(line.split()[3])

				else:
				# CHANGED
					if not vazao == 0.0:
						vazao += float(line.split()[3])
						values.append(vazao)
						vazao = 0.0

				while replication != int(line.split()[0]):
					replication += 1

			values = np.divide(values,traffic)
			vazao_values.append(np.mean(values))
			confidence_intervals.append(confidence_interval(values))
		
		plt.errorbar(TRAFFICS, vazao_values, yerr=confidence_intervals, label=LEGEND[legend], color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)
		legend += 1

	plt.xlabel(X_LEGEND)
	plt.ylabel('Vazão (Mbps)'.decode('utf-8'))
	#plt.xlim(0, 7)
	plt.xlim(4, 50) #descomentar para 6 a 48 nos
	plt.xticks(TRAFFICS)

	plt.grid()    
	plt.legend(numpoints=1, loc=1)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('vazao_estatico'+formato, dpi=200, bbox_inches = 'tight', pad_inches = 0.05)

	print("OK!\n")
	plt.show()

#PERDA
def plot_perda():

	legend = 0

	for algorithm in ALGORITHMS:
		
		perda_values = []
		confidence_intervals = []
		perda = int(0)

		for traffic in TRAFFICS:
			values = []
			file_input_name = algorithm + "/summary-" + str(traffic) + "-" + mobilidade + ".txt"
			replication = 0
			perda = 0

			for line in open(file_input_name):
				if replication == int(line.split()[0]): 
					if int(line.split()[5]) > 0:
						perda += ((int(line.split()[5]) * 100) / int(line.split()[6]))
					else:
						perda += 0.1

				else:
				# CHANGED
					if not perda == 0:
						perda += ((int(line.split()[5]) * 100) / int(line.split()[6]))
						values.append(perda)
						perda = 0

				while replication != int(line.split()[0]):
					replication += 1
			
			values = np.divide(values,traffic)
			perda_values.append(np.mean(values))
			confidence_intervals.append(confidence_interval(values))
		
		plt.errorbar(TRAFFICS, perda_values, yerr=confidence_intervals, label=LEGEND[legend], color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)
		legend += 1

	yticks = np.arange(0, 70, 10)
	plt.xlabel(X_LEGEND)
	plt.ylabel('Pacotes perdidos (%)'.decode('utf-8'))
	plt.xlim(0, 7)
	#plt.xlim(4, 50) #descomentar para 6 a 48 nos
	plt.xticks(TRAFFICS)
	plt.yticks(yticks)
	plt.ylim(-2.5,60)

	plt.grid()    
	plt.legend(numpoints=1, ncol=1, loc=2)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('perda_estatico'+formato, dpi=200, bbox_inches = 'tight', pad_inches = 0.05)

	print("OK!\n")
	plt.show()
							
if __name__ == "__main__":
	pass
	print("DELAY...")
	plot_delay()
	print("VAZAO...")
	plot_vazao()
	print("PERDA...")
	plot_perda()
