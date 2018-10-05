# -*- coding: UTF-8 -*-

from __future__ import division

import math
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

import numpy as np

TRAFFICS=(2, 4, 6)
ALGORITHMS = ("UDP","TCP") 
COLORS = {"UDP" : "k", "TCP" : "r"}
MARKS = {"UDP" : "H", "TCP" : "^"}
LINES = {"UDP" : "-", "TCP" : "--"}

mobilidade = "est" #Deixar automatico

X_LEGEND = 'Número de Clientes'.decode('utf-8')
formato = '.eps'

# (0) Replication, (1) Host, (2) Clientes, (3) Vazao, (4) Delay, (5) Perdidos

#CONFIDENCE INTERVAL - BAR ERROR
def confidence_interval(values):
	N = len(values)
	Z = 1.96
	std_dev = np.std(values)
	std_error = std_dev / math.sqrt(N)
	return Z * std_error

#DELAY
def plot_delay():

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
					if line.split()[4] != "0":
						#print line.split()[3]
						delay += float(line.split()[4])
						#print delay
						#values.append(delay)

				else:
				# CHANGED
					if not delay == 0.0:
						values.append(delay)
						delay = 0.0
				#		print "aqui"

				while replication != int(line.split()[0]):
					replication += 1
			
			if algorithm == "TCP":
				values = np.divide(values,traffic*2)
			else:
				values = np.divide(values,traffic)
			#values = np.multiply(values,60)
			#print values

			delay_values.append(np.mean(values))
			#print(algorithm, " - mean:", np.mean(values))
			confidence_intervals.append(confidence_interval(values))
			#print(" - confidence: ", confidence_interval(values))
		
		plt.errorbar(TRAFFICS, delay_values, yerr=confidence_intervals, label=algorithm, color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)

	#yticks = np.arange(50, 110, 10)
	plt.xlabel(X_LEGEND)
	plt.ylabel('Atraso (s)')
	plt.xlim(1, 7)
	#plt.ylim(0,1.5)
	plt.xticks(TRAFFICS)
	#plt.yticks(yticks)
	#plt.xticks((60, 100, 150))
	plt.title('Cenário sem Mobilidade'.decode('utf-8'))
	
	#plt.yscale("log")
	
	plt.grid()    
	plt.legend(numpoints=1, loc=2)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('delay'+formato, dpi=200)

	print("OK!\n")
	plt.show()

#PERDA
def plot_vazao():

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
					if line.split()[3] != "NA":
						#print line.split()[3]
						vazao += float(line.split()[3])
						#print delay
						#values.append(delay)

				else:
				# CHANGED
					if not vazao == 0.0:
						values.append(vazao)
						vazao = 0.0
				#		print "aqui"

				while replication != int(line.split()[0]):
					replication += 1
				
			if algorithm == "TCP":
				values = np.divide(values,traffic*2)
			else:
				values = np.divide(values,traffic)
			#print values

			vazao_values.append(np.mean(values))
			#print(algorithm, " - mean:", np.mean(values))
			confidence_intervals.append(confidence_interval(values))
			#print(" - confidence: ", confidence_interval(values))
		
		plt.errorbar(TRAFFICS, vazao_values, yerr=confidence_intervals, label=algorithm, color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)

	#yticks = np.arange(50, 110, 10)
	plt.xlabel(X_LEGEND)
	plt.ylabel('Vazao (Mbps)')
	plt.xlim(1, 7)
	#plt.ylim(0,1.5)
	plt.xticks(TRAFFICS)
	#plt.yticks(yticks)
	#plt.xticks((60, 100, 150))
	plt.title('Cenário sem Mobilidade'.decode('utf-8'))
	
	#plt.yscale("log")
	
	plt.grid()    
	plt.legend(numpoints=1, loc=2)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('vazao'+formato, dpi=200)

	print("OK!\n")
	plt.show()

#PERDA
def plot_perda():

	for algorithm in ALGORITHMS:
		perda_values = []
		confidence_intervals = []

		perda = float(0)

		for traffic in TRAFFICS:
			values = []
			file_input_name = algorithm + "/summary-" + str(traffic) + "-" + mobilidade + ".txt"
			#print file_input_name
			replication = 0
			perda = 0.0

			for line in open(file_input_name):
				if replication == int(line.split()[0]): 
					if line.split()[5] != "0":
						#print line.split()[3]
						perda += float(line.split()[5])
						#print file_input_name, line, algorithm, replication, perda
						#values.append(delay)

				else:
				# CHANGED
					if not perda == 0.0:
						values.append(perda)
						perda = 0.0
						#print "aqui"

				while replication != int(line.split()[0]):
					replication += 1
			
			#values = np.divide(values,traffic)
			# if algorithm == "TCP":
			# 	values = np.divide(values,traffic*2)
			# else:
			# 	values = np.divide(values,traffic)
			#print values

			perda_values.append(np.mean(values))
			#print(algorithm, " - mean:", np.mean(values))
			confidence_intervals.append(confidence_interval(values))
			#print(" - confidence: ", confidence_interval(values))
		
		plt.errorbar(TRAFFICS, perda_values, yerr=confidence_intervals, label=algorithm, color=COLORS[algorithm], marker=MARKS[algorithm], linestyle=LINES[algorithm], markersize = 8, linewidth = 2)

	#yticks = np.arange(50, 110, 10)
	plt.xlabel(X_LEGEND)
	plt.ylabel('Perda de pacotes #')
	plt.xlim(1, 7)
	#plt.ylim(0,1.5)
	plt.xticks(TRAFFICS)
	#plt.yticks(yticks)
	#plt.xticks((60, 100, 150))
	plt.title('Cenário sem Mobilidade'.decode('utf-8'))
	
	#plt.yscale("log")
	
	plt.grid()    
	plt.legend(numpoints=1, loc=2)
	
	fig = plt.gcf()
	fig.set_size_inches(6.8, 5.5)
	fig.savefig('perda'+formato, dpi=200)

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
