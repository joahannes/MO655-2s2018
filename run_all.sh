#!/bin/bash

#Estaticos
./run_gerencia.sh 33 1 0 0 && ./run_gerencia.sh 33 2 0 0 && ./run_gerencia.sh 33 4 0 0 && ./run_gerencia.sh 33 6 0 0 && ./run_gerencia.sh 33 1 1 0 && ./run_gerencia.sh 33 2 1 0 && ./run_gerencia.sh 33 4 1 0 && ./run_gerencia.sh 33 6 1 0

#Moveis
./run_gerencia.sh 33 1 0 1 && ./run_gerencia.sh 33 2 0 1 && ./run_gerencia.sh 33 4 0 1 && ./run_gerencia.sh 33 6 0 1 && ./run_gerencia.sh 33 1 1 1 && ./run_gerencia.sh 33 2 1 1 && ./run_gerencia.sh 33 4 1 1 && ./run_gerencia.sh 33 6 1 1

#email
#python email_proc.py
