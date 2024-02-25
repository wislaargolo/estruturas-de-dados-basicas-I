# Identificação Pessoal

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: Tobias dos Santos Neto
- Email: tobias.neto.106@ufrn.edu.br
- Turma: DIM0119 - T01

- Nome: Wisla Alves Argolo
- Email: wisla.argolo.133@ufrn.edu.br
- Turma: DIM0119 - T01

# Indique quais algoritmos foram implementados nos experimentos

- [x] Insertion sort
- [x] Selection sort
- [x] Bubble sort
- [x] Shell sort
- [x] Quick sort
- [x] Merge sort
- [x] Radix sort

# Indique quais dos cenários de dados abaixo você conseguiu simular nos experimentos

- [x] todos randômicos
- [x] parcialmente ordenado (75%)
- [x] parcialmente ordenado (50%)
- [x] parcialmente ordenado (25%)
- [x] dados em ordem não-decrescente
- [x] dados em ordem não-crescente

# Escreva a quantidade de amostras coletadas por experimento

- Utilizei _XXX_ amostras por experimento

# Indique quais das perguntas abaixo foram respondidas no relatório

- [x] O que você descobriu de maneira geral.
- [x] Quais algoritmos são recomendados para quais cenários?
- [x] Como o algoritmo de decomposição de chave (radix) se compara com o melhor algoritmo baseado em _comparação_ de chaves?
- [x] É verdade que o _quick sort_, na prática, é mesmo mais rápido do que o _merge sort_?
- [x] Aconteceu algo inesperado nas medições? (por exemplo, picos ou vales nos gráficos) Se sim, por que?
- [x] Com base nos resultados empíricos, faça uma estimativa para os 7 algoritmos estudados de quanto tempo seria necessário para rodar uma amostra de $10^12$ elementos.
- [x] A análise empírica é compatível com a análise de complexidade matemática? Ou seja, você consegui ajustar uma curva compatível com a complexidade aos dados?

# Como executar os testes

Rode o seguinte código:

~~~bash
cmake -S source -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
~~~

Feito isso, o processo de compilação foi adicionado à pasta *build*. Para executá-lo, execute:

~~~bash
cd build
./sortsuite
~~~

# Limitações

- Radix funciona exlusivamente com números inteiros positivos;
- O array para gerar cenários, na medição de tempo, não foi alocado dinamicamente.

--------
&copy; DIMAp/UFRN 2021.
