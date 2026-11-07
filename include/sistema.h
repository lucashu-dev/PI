#ifndef SISTEMA_H
#define SISTEMA_H

#include "modelos.h"

/**
 * @brief Inicializa os recursos do sistema:
 * Abre banco SQLite, carrega schema e restaura registros em memória RAM para o array de structs.
 */
void inicializar_sistema(void);

/**
 * @brief Finaliza os subsistemas e fecha conexões ativas.
 */
void finalizar_sistema(void);

/**
 * @brief Executa o laço principal de interação do console (Menu de opções).
 */
void menu_principal(void);

/**
 * @brief Operação de cadastro de novo produto com validações contra IDs duplicados.
 */
void sistema_cadastrar_produto(void);

/**
 * @brief Operação para alteração de dados cadastrais de um produto existente.
 */
void sistema_alterar_produto(void);

/**
 * @brief Operação de remoção de produto do sistema com confirmação explícita.
 */
void sistema_excluir_produto(void);

/**
 * @brief Lista todos os produtos carregados em memória formatados em tabela.
 */
void sistema_listar_produtos(void);

/**
 * @brief Pesquisa produto por código utilizando estritamente Busca Binária Manual.
 */
void sistema_pesquisar_por_codigo(void);

/**
 * @brief Pesquisa produto por nome ou termo parcial utilizando Busca Linear Manual.
 */
void sistema_pesquisar_por_nome(void);

/**
 * @brief Permite ao usuário escolher o algoritmo manual (Insertion, Bubble, Selection)
 * e o critério (Nome, Preço, Quantidade, Código) para ordenar o array em RAM.
 */
void sistema_ordenar_produtos(void);

/**
 * @brief Registra entrada ou saída de mercadoria com integridade transacional no banco.
 */
void sistema_registrar_movimentacao(void);

/**
 * @brief Consulta e exibe produtos com estoque atual menor ou igual ao estoque mínimo.
 */
void sistema_alerta_estoque_baixo(void);

/**
 * @brief Calcula e exibe o valor financeiro consolidado do estoque (Soma de Preço * Quantidade).
 */
void sistema_calcular_valor_total(void);

/**
 * @brief Processa e exibe a Matriz 2D de movimentações (Produtos x 12 Meses) para análise de densidade.
 */
void sistema_matriz_movimentacoes_mensais(void);

/**
 * @brief Exibe o histórico de transações/movimentações registradas no SQLite.
 */
void sistema_historico_movimentacoes(void);

#endif /* SISTEMA_H */
