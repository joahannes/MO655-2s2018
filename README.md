# MC655-2018

### Executando simulação ###

Colocar o script "run_gerencia.sh" na pasta scratch do NS3 e executar:

	$ ./run_gerencia.sh 33 5 0 0

* Onde 33 é o número de seeds, 5 é o número de clientes e 0 o tráfego (0 = UDP, 1 = TCP e 2 = UDP/TCP) e 0 a mobilidade (0 = Sem e 1 = Com).

### Executando os scripts ###

Processar os arquivos "results-x.txt" gerados:

	$ python process_gerencia.py

Plotar os graficos:

	$ python plot_gerencia.py
