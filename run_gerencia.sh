#!/bin/bash
#Autor: Joahannes B.D. Costa <joahannes@gmail.com>
#Data: 23.09.2018
#Nota: script para rodar simulacao de MO655 - Gerencia de Redes - 2018
#Rodar: ./run_gerencia.sh numero_seeds numero_clientes trafego

seeds=$1
#$2 = clientes
#$3 = trafego

#Vai para diretorio ns-3.29
cd ..

echo
echo " * Número de seeds: $1"
echo " * Número de clientes: $2"
if [ $3 == 0 ]
then
	echo " * Tráfego: UDP"
elif [ $3 == 1 ]
then
	echo " * Tráfego: TCP"
else
	echo " * Tráfego: 50% UDP e 50%TCP"
fi
echo

for ((i=0; i<$seeds; i++))
do
	#Controle para mensagens
	if [ $3 == 0 ]
	then
		echo "Executando seed número [$i] com $2 Clientes com UDP..."
		#Chamada da simulacao
		./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --nApWifi=1 --verbose=true --tracing=false --trafego=$3" > resultados/results-udp-$i.txt 2>&1
	elif [ $3 == 1 ]
	then
		echo "Executando seed número [$i] com $2 Clientes com TCP..."
		#Chamada da simulacao
		./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --nApWifi=1 --verbose=true --tracing=false --trafego=$3" > resultados/results-tcp-$i.txt 2>&1
	else
		echo "Executando seed número [$i] com $2 Clientes com UDP e TCP..."
		#Chamada da simulacao
		./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --nApWifi=1 --verbose=true --tracing=false --trafego=$3" > resultados/results-ambos-$i.txt 2>&1
	fi
done

echo
echo "Fim da simulação! :)"
echo

#cd resultados
#gnuplot Atraso.plt
#gnuplot Vazao.plt

#cd ../../netanim-3.108/
#./NetAnim


#EOF
