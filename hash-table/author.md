# Identificação Pessoal

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: Tobias dos Santos Neto
- Email: tobias.neto.106@ufrn.edu.br
- Turma: DIM0119 - T01

- Nome: Wisla Alves Argolo
- Email: wisla.argolo.133@ufrn.edu.br
- Turma: DIM0119 - T01

# Indique quais métodos das classes abaixo foram implementados

## Implementação do Hash Table

**Special members (14 credits)**
- [x] Regular constructor (2 credits)
- [x] Destructor (2 credits)
- [x] Copy constructor (3 credits)
- [x] Constructor from initialize list (3 credits)
- [x] Assignment operator `operator=(list)` (2 credits)
- [x] Assignment operator `operator=(initializer list)` (2 credits)

**Other methods (101 credits)**
- [x] `insert()` (12 credits)
- [x] `erase()` (12 credits)
- [x] `retrieve()` (12 credits)
- [x] `clear()` (8 credits)
- [x] `empty()` (3 credits);
- [x] `size()` (3 credits);
- [x] `operator<<()` (8 credits)
- [x] `at()` (11 credits)
- [x] `operator[]()` (11 credits)
- [x] `count()` (6 credits)
- [x] `rehash()` (15 credits);

# Como executar os testes

Para executar os testes propostos, é necessário instalar o GOOGLE TESTS. Feito isso, basta rodar o seguinte código:

~~~bash
cmake -S source -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
~~~

Feito isso, o processo de compilação foi adicionado à pasta *build*. Para executá-lo, execute:

~~~bash
cd build
./run_tests
~~~

Além disso, para testar o operador <<, execute:

~~~bash
./driver_hash
~~~

--------
&copy; DIMAp/UFRN 2021.
