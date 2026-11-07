#ifndef MODELOS_H
#define MODELOS_H

#include <stddef.h>

#define TAM_NOME 100
#define TAM_DESC 255
#define TAM_CATEGORIA 50
#define TAM_DATA 25
#define TAM_OBS 255
#define MAX_PRODUTOS 1000
#define MESES_ANO 12

/**
 * @brief Tipo de movimentação de estoque:
 * MOV_ENTRADA = 1 (Adição de itens ao estoque)
 * MOV_SAIDA   = 2 (Retirada de itens do estoque)
 */
typedef enum {
    MOV_ENTRADA = 1,
    MOV_SAIDA = 2
} TipoMovimentacao;

/**
 * @brief Critério para ordenação manual de produtos em memória RAM.
 */
typedef enum {
    CRITERIO_CODIGO = 1,
    CRITERIO_NOME = 2,
    CRITERIO_PRECO = 3,
    CRITERIO_QUANTIDADE = 4
} CriterioOrdenacao;

/**
 * @brief Algoritmo selecionado para ordenação manual.
 */
typedef enum {
    ALGORITMO_INSERTION = 1,
    ALGORITMO_BUBBLE = 2,
    ALGORITMO_SELECTION = 3
} TipoAlgoritmoOrdenacao;

/**
 * @brief Representação da entidade Produto.
 * Atende ao requisito de Structs e Arrays de Structs da disciplina.
 */
typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char descricao[TAM_DESC];
    char categoria[TAM_CATEGORIA];
    double preco;
    int quantidade;
    int estoque_minimo;
} Produto;

/**
 * @brief Representação da entidade Movimentação de Estoque.
 * Utilizada para persistência histórica e rastreabilidade transacional.
 */
typedef struct {
    int id;
    int codigo_produto;
    TipoMovimentacao tipo;
    int quantidade;
    char data_hora[TAM_DATA];
    char observacao[TAM_OBS];
} Movimentacao;

/**
 * @brief Estrutura agregadora do catálogo em memória RAM.
 */
typedef struct {
    Produto itens[MAX_PRODUTOS];
    int total;
} CatalogoProdutos;

#endif /* MODELOS_H */
