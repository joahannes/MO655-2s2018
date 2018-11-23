# MC655-2018

### Observações importantes ###

* Criar pasta resultados em ns-3.29/
* Criar pastas UDP, TCP e AMBOS (UDP e TCP) em resultados/

### Executando simulação ###

Colocar o script "run_all.sh" na pasta scratch do NS3 e executar:

	$ ./run_all.sh

O script "run_all.sh" chama o "run_gerencia.sh" que conta com os parâmetros seed, clientes, trafego e mobilidade, que são passados dentro de "run_all.sh", como por exemplo:

	$ ./run_gerencia.sh 33 5 0 0
	
* Onde 33 é o número de seeds, 5 é o número de clientes, 0 o tráfego (0 = UDP, 1 = TCP e 2 = UDP/TCP) e 0 a mobilidade (0 = Sem e 1 = Com).

### Executando os scripts ###

Processar os arquivos "results-x.txt" gerados:

	$ python process_gerencia.py
	
* Alterar variavel "mobilidade" para "mov" quando processar o cenário com mobilidade ou "est" para o cenário sem mobilidade. 
Formato do arquivo "summary" de saída:

0	1	1	0.51693	0.0021522	0	7899
1	1	1	0.51693	0.00215035	0	7899
2	1	1	0.51693	0.00214993	0	7899
3	1	1	0.51693	0.00215102	0	7899
4	1	1	0.51693	0.00215222	0	7899
5	1	1	0.51693	0.00215034	0	7899
6	1	1	0.51693	0.00215208	0	7899
7	1	1	0.51693	0.00214983	0	7899
8	1	1	0.51693	0.00215074	0	7899
9	1	1	0.51693	0.00215382	0	7899


Plotar os graficos (para os cenários com e sem mobilidade):

	$ python plot_estatico.py
	$ python plot_mobilidade.py
