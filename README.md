# MC655-2018

### Executando simulação ###

Colocar o script "run_gerencia.sh" na pasta scratch do NS3 e executar:

	$ ./run_gerencia.sh s c t

* Onde n é o número de seeds, c é o número de clientes e t o tráfego (0 = UDP, 1 = TCP e 2 = UDP/TCP).

### Executando os scripts ###

Processar os arquivos "results-x.txt" gerados:

	$ python process_gerencia.py

Plotar os graficos:

	$ python plot_gerencia.py
