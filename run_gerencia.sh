#!/bin/bash
#Autor: Joahannes B.D. Costa <joahannes@gmail.com>
#Data: 23.09.2018
#Nota: script para rodar simulacao de MO655 - Gerencia de Redes - 2018
#Rodar: ./run_gerencia.sh numero_seeds

seeds=$1

echo

cd ..

for ((i=0; i<$seeds; i++))
do
echo "Executando seed número [$i]...";
./waf --run "scratch/gerencia2018 --seed=$i --nWifi=5 --nApWifi=1 --verbose=true --tracing=false --trafego=0" > resultados/results-udp-$i.txt 2>&1
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
