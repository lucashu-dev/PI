#ifndef BANCO_H
#define BANCO_H

#include "modelos.h"

/**
 * @brief Inicializa a conexão com o banco de dados SQLite e cria as tabelas caso não existam.
 * Executa o schema DDL com tratamento de erro.
 * 
 * @param caminho_banco Caminho para o arquivo do banco de dados (ex: "database/estoque.db").
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
int banco_inicializar(const char *caminho_banco);

/**
 * @brief Fecha com segurança a conexão ativa com o banco de dados SQLite.
 */
void banco_fechar(void);

/**
 * @brief Carrega todos os produtos persistidos no banco de dados para a memória RAM.
 * Permite que a aplicação manipule os dados através de structs e arrays de structs.
 * 
 * @param vetor Array de structs Produto a ser preenchido.
 * @param total Ponteiro para armazenar o número de registros carregados.
 * @param max_capacidade Capacidade máxima suportada pelo array.
 * @return 1 se bem-sucedido, 0 se ocorreu erro.
 */
int banco_carregar_produtos(Produto *vetor, int *total, int max_capacidade);

/**
 * @brief Insere um novo produto no banco de dados utilizando consulta estritamente parametrizada.
 * Proibido concatenar strings com entradas do usuário (proteção contra SQL Injection).
 * 
 * @param p Ponteiro para a struct Produto contendo os dados.
 * @return 1 se inserido com sucesso, 0 se falhar (ex: código duplicado).
 */
int banco_inserir_produto(const Produto *p);

/**
 * @brief Atualiza os dados de um produto existente no banco de dados.
 * 
 * @param p Ponteiro para a struct com os dados atualizados.
 * @return 1 se atualizado com sucesso, 0 se falhar.
 */
int banco_atualizar_produto(const Produto *p);

/**
 * @brief Remove logicamente ou fisicamente um produto do banco de dados.
 * 
 * @param codigo Código identificador do produto a excluir.
 * @return 1 se excluído com sucesso, 0 se falhar.
 */
int banco_excluir_produto(int codigo);

/**
 * @brief Registra uma movimentação de estoque e atualiza o saldo do produto atomicamente.
 * REQUISITO DE INTEGRIDADE TRANSACIONAL:
 * Executa BEGIN TRANSACTION, insere o registro na tabela 'movimentacoes',
 * executa UPDATE na tabela 'produtos' com o novo saldo e executa COMMIT.
 * Em caso de qualquer falha, executa ROLLBACK mantendo o estado íntegro.
 * 
 * @param codigo_produto Código do produto movimentado.
 * @param tipo MOV_ENTRADA ou MOV_SAIDA.
 * @param quantidade Quantidade movimentada (> 0).
 * @param observacao Texto descritivo/justificativa.
 * @param novo_saldo Saldo final calculado em RAM a ser persistido.
 * @return 1 se a transação foi confirmada com sucesso, 0 se foi revertida (ROLLBACK).
 */
int banco_registrar_movimentacao_transacional(int codigo_produto, TipoMovimentacao tipo, int quantidade, const char *observacao, int novo_saldo);

/**
 * @brief Preenche a matriz bidimensional (Produtos x 12 Meses) com a soma líquida de movimentações de um ano específico.
 * 
 * @param ano Ano de referência (ex: 2026).
 * @param matriz Matriz 2D [MAX_PRODUTOS][12] a ser preenchida.
 * @param produtos Array de produtos atualmente em memória.
 * @param total_produtos Quantidade de produtos.
 * @return 1 se bem-sucedido, 0 se erro.
 */
int banco_carregar_matriz_movimentacoes(int ano, int matriz[MAX_PRODUTOS][MESES_ANO], const Produto *produtos, int total_produtos);

/**
 * @brief Exibe o histórico de movimentações formatado na tela para um produto específico ou todos (codigo = 0).
 * 
 * @param codigo_filtro Código do produto a filtrar, ou 0 para todos.
 * @return Quantidade de registros exibidos.
 */
int banco_listar_historico_movimentacoes(int codigo_filtro);

#endif /* BANCO_H */
