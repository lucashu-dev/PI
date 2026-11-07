#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include "modelos.h"

/**
 * @brief Lê um número inteiro seguro via fgets e strtol com validação de intervalo.
 * @param mensagem Prompt para o usuário.
 * @param saida Ponteiro onde o valor validado será armazenado.
 * @param min Limite mínimo permitido.
 * @param max Limite máximo permitido.
 * @return 1 se a leitura for bem-sucedida, 0 se cancelada ou inválida.
 */
int ler_inteiro_seguro(const char *mensagem, int *saida, int min, int max);

/**
 * @brief Lê um número de ponto flutuante de dupla precisão seguro via fgets e strtod.
 * @param mensagem Prompt para o usuário.
 * @param saida Ponteiro onde o valor validado será armazenado.
 * @param min Limite mínimo permitido.
 * @param max Limite máximo permitido.
 * @return 1 se bem-sucedido, 0 se inválido.
 */
int ler_double_seguro(const char *mensagem, double *saida, double min, double max);

/**
 * @brief Lê string do teclado removendo quebra de linha (\n e \r) e tratando buffer.
 * @param mensagem Prompt exibido ao usuário.
 * @param destino Buffer de destino.
 * @param tamanho Capacidade do buffer de destino.
 */
void ler_string_segura(const char *mensagem, char *destino, size_t tamanho);

/**
 * @brief Solicita confirmação binária (S/N) ao usuário.
 * @param mensagem Pergunta a ser exibida.
 * @return 1 para Sim/Yes, 0 para Não/No.
 */
int confirmar_acao(const char *mensagem);

/**
 * @brief Obtém data e hora atual formatada em "YYYY-MM-DD HH:MM:SS".
 * @param buffer Vetor de saída.
 * @param tamanho Tamanho máximo do buffer.
 */
void obter_data_hora_atual(char *buffer, size_t tamanho);

/**
 * @brief Atualiza a quantidade de estoque de um produto utilizando ponteiro direto.
 * Demonstração prática do critério "Ponteiros e passagem por referência".
 * @param p Ponteiro para a struct Produto em memória.
 * @param quantidade Quantidade a ser adicionada ou subtraída.
 * @param tipo MOV_ENTRADA ou MOV_SAIDA.
 * @return 1 se a operação foi bem-sucedida, 0 se estoque insuficiente ou parâmetros inválidos.
 */
int atualizar_estoque(Produto *p, int quantidade, TipoMovimentacao tipo);

/**
 * @brief Converte uma cadeia de caracteres para minúsculas.
 * @param origem String de entrada.
 * @param destino String de saída.
 */
void string_para_minusculo(const char *origem, char *destino);

/**
 * @brief Verifica se a string 'texto' contém 'termo' ignorando diferenças entre maiúsculas e minúsculas.
 * @return 1 se contém, 0 caso contrário.
 */
int contem_substring_sem_caso(const char *texto, const char *termo);

/**
 * @brief Pausa a execução do console aguardando pressionar ENTER.
 */
void pausar_console(void);

/**
 * @brief Limpa a tela do console de forma portável (Windows/Linux).
 */
void limpar_tela(void);

#endif /* UTIL_H */
