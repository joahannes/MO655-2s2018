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

Plotar os graficos (para os cenários com e sem mobilidade):

	$ python plot_estatico.py
	$ python plot_mobilidade.py
