#!/bin/bash
#Autor: Joahannes B.D. Costa <joahannes@lrc.ic.unicamp.br>
#Data: 23.09.2018
#Nota: script para rodar simulacao de MO655 - Gerencia de Redes - 2018
#Rodar: ./run_gerencia.sh numero_seeds numero_clientes trafego mobilidade

seeds=$1
#$2 = clientes
#$3 = trafego

#Vai para diretorio ns-3.29
cd ..

#Resumo da simulacao
echo
echo " * MO655 - Gerência de Redes - 2018"
echo " * Número de seeds: $1"
echo " * Número de clientes: $2"
if [ $4 == 0 ]
then
	echo " * Mobilidade: Ausente"
else
	echo " * Mobilidade: Presente"
fi
#--
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
	if [ $4 == 0 ]
	then
		#Controle para mensagens
		if [ $3 == 0 ]
		then
			echo "Executando seed [$i] com $2 clientes ESTÁTICOS e tráfego UDP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-est-udp-$i.txt 2>&1
		elif [ $3 == 1 ]
		then
			echo "Executando seed [$i] com $2 clientes ESTÁTICOS e tráfego TCP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-est-tcp-$i.txt 2>&1
		else
			echo "Executando seed [$i] com $2 clientes ESTÁTICOS e tráfegos UDP/TCP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-est-ambos-$i.txt 2>&1
		fi
	else
		#Controle para mensagens
		if [ $3 == 0 ]
		then
			echo "Executando seed [$i] com $2 clientes MÓVEIS e tráfego UDP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-mov-udp-$i.txt 2>&1
		elif [ $3 == 1 ]
		then
			echo "Executando seed [$i] com $2 clientes MÓVEIS e tráfego TCP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-mov-tcp-$i.txt 2>&1
		else
			echo "Executando seed [$i] com $2 clientes MÓVEIS e tráfegos UDP/TCP..."
			#Chamada da simulacao
			./waf --run "scratch/gerencia2018 --seed=$i --nWifi=$2 --trafego=$3 -mobilidade=$4" > resultados/results-mov-ambos-$i.txt 2>&1
		fi
	fi
done

echo
echo "Fim da simulação! :)"
echo

#EOF
