#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "modelos.h"

/**
 * @brief Realiza busca linear manual percorrendo o array de structs por nome.
 * Não utiliza funções de busca pré-fabricadas.
 * Permite encontrar correspondências exatas e parciais (substring).
 * 
 * @param vetor Array de produtos em memória RAM.
 * @param total Quantidade atual de produtos cadastrados.
 * @param nome_busca Termo ou nome a ser pesquisado.
 * @param indices_encontrados Array preenchido com os índices dos registros encontrados.
 * @param max_resultados Capacidade do array indices_encontrados.
 * @return Número total de ocorrências encontradas.
 */
int busca_linear_nome(const Produto *vetor, int total, const char *nome_busca, int indices_encontrados[], int max_resultados);

/**
 * @brief Realiza busca binária manual em C pelo código do produto.
 * REGRA ACADÊMICA: O vetor deve estar previamente ordenado por código.
 * Implementa estratégia de divisão e conquista com complexidade O(log n).
 * Proibido expressamente o uso de bsearch().
 * 
 * @param vetor Array de produtos ordenados por código.
 * @param total Quantidade total de elementos no array.
 * @param codigo_busca Código do produto procurado.
 * @return Índice do elemento encontrado no array, ou -1 caso não exista.
 */
int busca_binaria_codigo(const Produto *vetor, int total, int codigo_busca);

/**
 * @brief Compara dois produtos com base no critério selecionado.
 * @param a Ponteiro para o primeiro produto.
 * @param b Ponteiro para o segundo produto.
 * @param criterio Critério de ordenação (Código, Nome, Preço, Quantidade).
 * @return Valor negativo se a < b, 0 se a == b, positivo se a > b.
 */
int comparar_produtos(const Produto *a, const Produto *b, CriterioOrdenacao criterio);

/**
 * @brief Ordenação manual por Inserção (Insertion Sort).
 * Complexidade: O(n) no melhor caso, O(n²) no caso médio/pior caso.
 * Estável e eficiente para pequenos conjuntos de dados.
 * 
 * @param vetor Array de produtos a ser ordenado in-place.
 * @param total Quantidade de produtos.
 * @param criterio Critério de ordenação.
 */
void ordenar_insertion(Produto *vetor, int total, CriterioOrdenacao criterio);

/**
 * @brief Ordenação manual por Flutuação/Bolha (Bubble Sort).
 * Implementado com flag de parada antecipada caso o vetor já esteja ordenado.
 * 
 * @param vetor Array de produtos a ser ordenado in-place.
 * @param total Quantidade de produtos.
 * @param criterio Critério de ordenação.
 */
void ordenar_bubble(Produto *vetor, int total, CriterioOrdenacao criterio);

/**
 * @brief Ordenação manual por Seleção (Selection Sort).
 * Encontra o menor elemento e o posiciona na posição correta a cada iteração.
 * 
 * @param vetor Array de produtos a ser ordenado in-place.
 * @param total Quantidade de produtos.
 * @param criterio Critério de ordenação.
 */
void ordenar_selection(Produto *vetor, int total, CriterioOrdenacao criterio);

/**
 * @brief Verifica se o vetor de produtos está estritamente ordenado por código.
 * @param vetor Array de produtos.
 * @param total Quantidade de produtos.
 * @return 1 se ordenado, 0 caso contrário.
 */
int verificar_ordenado_por_codigo(const Produto *vetor, int total);

#endif /* ALGORITMOS_H */
