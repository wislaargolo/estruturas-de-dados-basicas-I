# Identificação Pessoal

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: Tobias dos Santos Neto
- Email: tobias.neto.106@ufrn.edu.br
- Turma: DIM0119 - T01

- Nome: Wisla Alves Argolo
- Email: wisla.argolo.133@ufrn.edu.br
- Turma: DIM0119 - T01

# Indique quais métodos das classes abaixo foram implementados

## Implementação de List

**Special members (20 credits)**
- [x] Regular constructor (3 credits)
- [x] Constructor size (3 credits)
- [x] Destructor (3 credits)
- [x] Copy constructor (3 credits)
- [x] Constructor from range (3 credits)
- [x] Constructor from initialize list (2 credits)
- [x] Assignment operator `operator=(list)` (2 credits)
- [x] Assignment operator `operator=(initializer list)` (2 credits)

**Iterator methods (4 credits)**
- [x] `begin()` (1 credits)
- [x] `end()` (1 credits)
- [x] `cbegin()` (1 credits)
- [x] `cend()` (1 credits)

**Access operations (6 credits)**
- [x] `empty()` (1 credits);
- [x] `size()` (1 credits);
- [x] `front()` (2 credits);
- [x] `back()` (2 credits);

**Modifiers methods (21 credits)**
- [x] `clear()` (1 credits)
- [x] `push_front()` (3 credits)
- [x] `push_back()` (3 credits)
- [x] `pop_front()` (3 credits)
- [x] `pop_back()` (3 credits)
- [x] `assign(range)` (3 credits)
- [x] `assign(initializer list)` (3 credits)

**Operator overloading (4 credits)**
- [x] `operator==()` (2 credits)
- [x] `operator!=()` (2 credits)

**Methods that require iterators (19 credits)**
- [x] `insert(iterator,value)` (4 credits)
- [x] `insert(iterator,range)` (4 credits)
- [x] `insert(iterator,initializer list)` (4 credits)
- [x] `erase(iterator)` (3 credits)
- [x] `erase(range)` (3 credits)

**Utility methods (40 credits)**
- [x] `merge(other)` (8 credits)
- [x] `splice(pos,other)` (8 credits)
- [x] `reverse()` (8 credits)
- [x] `unique()` (8 credits)
- [x] `sort()` (8 credits)

## Implementação do Iterator

**Special members (3 credits)**
- [x] Regular constructor (1 credits)
- [x] Copy constructor (1 credits)
- [x] Assignment operator (1 credits)

**Navigation methods (7 credits)**
- [x] increment operator `++it` and `it++` (2 credits)
- [x] decrement operator `--it` and `it--` (2 credits)
- [x] dereference operator `*it` (1 credits)
- [x] equality/difference operators `it1==it2` and `it1!=it2` (2 credits)

# Como executar os testes

Rode o seguinte código:

~~~bash
cmake -S source -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
~~~

Feito isso, o processo de compilação foi adicionado à pasta *build*. Para executá-lo, execute:

~~~bash
cd build
./tests/all_tests
~~~

--------
&copy; DIMAp/UFRN 2021.
