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

#extra
#Estaticos
# (0) UDP
./run_gerencia.sh 33 12 0 0 && ./run_gerencia.sh 33 24 0 0 && ./run_gerencia.sh 33 48 0 0
# (1) TCP
./run_gerencia.sh 33 12 1 0 && ./run_gerencia.sh 33 24 1 0 && ./run_gerencia.sh 33 48 1 0
# (2) UDP/TCP
./run_gerencia.sh 33 12 2 0 && ./run_gerencia.sh 33 24 2 0 && ./run_gerencia.sh 33 48 2 0

#Moveis
# (0) UDP
./run_gerencia.sh 33 12 0 1 && ./run_gerencia.sh 33 24 0 1 && ./run_gerencia.sh 33 48 0 1
# (1) TCP
./run_gerencia.sh 33 12 1 1 && ./run_gerencia.sh 33 24 1 1 && ./run_gerencia.sh 33 48 1 1
# (2) UDP/TCP
./run_gerencia.sh 33 12 2 1 && ./run_gerencia.sh 33 24 2 1 && ./run_gerencia.sh 33 48 2 1

#email
#python email_proc.py
