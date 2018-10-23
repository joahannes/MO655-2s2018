#!/bin/bash
# Joahannes Costa <joahannes@lrc.ic.unicamp.br>

#Estaticos
# (0) UDP
./run_gerencia.sh 33 1 0 0 && ./run_gerencia.sh 33 2 0 0 && ./run_gerencia.sh 33 4 0 0 && ./run_gerencia.sh 33 6 0 0
# (1) TCP
./run_gerencia.sh 33 1 1 0 && ./run_gerencia.sh 33 2 1 0 && ./run_gerencia.sh 33 4 1 0 && ./run_gerencia.sh 33 6 1 0
# (2) UDP/TCP
./run_gerencia.sh 33 1 2 0 && ./run_gerencia.sh 33 2 2 0 && ./run_gerencia.sh 33 4 2 0 && ./run_gerencia.sh 33 6 2 0

#Moveis
# (0) UDP
./run_gerencia.sh 33 1 0 1 && ./run_gerencia.sh 33 2 0 1 && ./run_gerencia.sh 33 4 0 1 && ./run_gerencia.sh 33 6 0 1
# (1) TCP
./run_gerencia.sh 33 1 1 1 && ./run_gerencia.sh 33 2 1 1 && ./run_gerencia.sh 33 4 1 1 && ./run_gerencia.sh 33 6 1 1
# (2) UDP/TCP
./run_gerencia.sh 33 1 2 1 && ./run_gerencia.sh 33 2 2 1 && ./run_gerencia.sh 33 4 2 1 && ./run_gerencia.sh 33 6 2 1

#email
#python email_proc.py
