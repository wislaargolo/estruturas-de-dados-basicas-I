/*!
 * This is a template code to demonstrate how to measure runtime of part of your code.
 * I'm using the chrono C++ library.
 * @date September 8th, 2020.
 * @author Selan
 */

#include <chrono>   // timing
#include <cstring>  // memset()
#include <iomanip>  // setw, setfixed
#include <iostream> // cout
#include <vector>   // vector
#include <fstream>

#include "searching.h"

constexpr int max_sz{10000000}; // 10^7

int main( void ){

    int *vec = new int[max_sz]; // aloca o array a partir do qual os intervalos de busca serão testados
    memset(vec, 1, max_sz); //inicializa o array com 1

    // controlam a progressão do tamanho da amostra
    constexpr size_t sample_step{20000}; // 20k
    constexpr size_t initial_sz{1000};   // 10^3
    size_t sample_sz{initial_sz};

    //armazena o tamanho da amostra e tempo de execução
    std::vector<std::pair<size_t, std::chrono::duration<double>>> times;

    // mede o tempo de execução até que chegue ao tamanho máximo da amostra
    while (sample_sz < max_sz) {
        auto start = std::chrono::steady_clock::now();
        //incrementa o tamanho da amostra a cada passo
        sa::bsearch(vec, vec + sample_sz, 2); // realiza a busca no pior caso 
        auto end = std::chrono::steady_clock::now();

        times.emplace_back(sample_sz, (end - start)); //adiciona o tam da amostra e tempo em times 
        
        sample_sz += sample_step; //incrementa o tamanho da amostra
    }

    std::ofstream file("../../data/bsearch_it/bi10.txt"); // abre o arquivo 

    for (const auto &[sz, diff] : times) {
    file << std::setw(7) << sz << " " << std::setw(5) << std::fixed
              << std::chrono::duration<double, std::milli>(diff).count()
              << '\n';
  }

    file.close();
    
    delete [] vec; //libera memória

    return EXIT_SUCCESS;     

}
