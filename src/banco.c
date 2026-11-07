#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../include/banco.h"
#include "../include/util.h"

static sqlite3 *db = NULL;

int banco_inicializar(const char *caminho_banco) {
    if (!caminho_banco) return 0;

    int rc = sqlite3_open(caminho_banco, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao abrir banco de dados SQLite: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Ativar verificacao de chaves estrangeiras
    char *err_msg = NULL;
    rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao ativar chaves estrangeiras: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // DDL para inicializacao do Schema
    const char *sql_schema =
        "CREATE TABLE IF NOT EXISTS produtos ("
        "   codigo INTEGER PRIMARY KEY,"
        "   nome TEXT NOT NULL,"
        "   descricao TEXT,"
        "   categoria TEXT NOT NULL,"
        "   preco REAL NOT NULL CHECK(preco >= 0.0),"
        "   quantidade INTEGER NOT NULL CHECK(quantidade >= 0),"
        "   estoque_minimo INTEGER NOT NULL CHECK(estoque_minimo >= 0)"
        ");"
        "CREATE TABLE IF NOT EXISTS movimentacoes ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   codigo_produto INTEGER NOT NULL,"
        "   tipo INTEGER NOT NULL CHECK(tipo IN (1, 2)),"
        "   quantidade INTEGER NOT NULL CHECK(quantidade > 0),"
        "   data_hora TEXT NOT NULL,"
        "   observacao TEXT,"
        "   FOREIGN KEY (codigo_produto) REFERENCES produtos(codigo) ON DELETE CASCADE"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_produtos_nome ON produtos(nome);"
        "CREATE INDEX IF NOT EXISTS idx_mov_produto ON movimentacoes(codigo_produto);"
        "CREATE INDEX IF NOT EXISTS idx_mov_data ON movimentacoes(data_hora);";

    rc = sqlite3_exec(db, sql_schema, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao criar tabelas: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }

    return 1;
}

void banco_fechar(void) {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

int banco_carregar_produtos(Produto *vetor, int *total, int max_capacidade) {
    if (!db || !vetor || !total) return 0;

    const char *sql = "SELECT codigo, nome, descricao, categoria, preco, quantidade, estoque_minimo "
                      "FROM produtos;";
    sqlite3_stmt *stmt = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao preparar carga de produtos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (count >= max_capacidade) {
            fprintf(stderr, "[AVISO] Capacidade maxima de produtos em memoria atingida (%d).\n", max_capacidade);
            break;
        }

        vetor[count].codigo = sqlite3_column_int(stmt, 0);

        const unsigned char *nome = sqlite3_column_text(stmt, 1);
        strncpy(vetor[count].nome, nome ? (const char *)nome : "", sizeof(vetor[count].nome) - 1);
        vetor[count].nome[sizeof(vetor[count].nome) - 1] = '\0';

        const unsigned char *desc = sqlite3_column_text(stmt, 2);
        strncpy(vetor[count].descricao, desc ? (const char *)desc : "", sizeof(vetor[count].descricao) - 1);
        vetor[count].descricao[sizeof(vetor[count].descricao) - 1] = '\0';

        const unsigned char *cat = sqlite3_column_text(stmt, 3);
        strncpy(vetor[count].categoria, cat ? (const char *)cat : "", sizeof(vetor[count].categoria) - 1);
        vetor[count].categoria[sizeof(vetor[count].categoria) - 1] = '\0';

        vetor[count].preco = sqlite3_column_double(stmt, 4);
        vetor[count].quantidade = sqlite3_column_int(stmt, 5);
        vetor[count].estoque_minimo = sqlite3_column_int(stmt, 6);

        count++;
    }

    sqlite3_finalize(stmt);
    *total = count;
    return 1;
}

int banco_inserir_produto(const Produto *p) {
    if (!db || !p) return 0;

    const char *sql = "INSERT INTO produtos (codigo, nome, descricao, categoria, preco, quantidade, estoque_minimo) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao preparar INSERT: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, p->codigo);
    sqlite3_bind_text(stmt, 2, p->nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p->descricao, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, p->categoria, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 5, p->preco);
    sqlite3_bind_int(stmt, 6, p->quantidade);
    sqlite3_bind_int(stmt, 7, p->estoque_minimo);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "[ERRO BANCO] Falha na execucao do INSERT (Codigo duplicado ou violacao de restricao): %s\n", sqlite3_errmsg(db));
        return 0;
    }

    return 1;
}

int banco_atualizar_produto(const Produto *p) {
    if (!db || !p) return 0;

    const char *sql = "UPDATE produtos SET nome = ?, descricao = ?, categoria = ?, preco = ?, "
                      "quantidade = ?, estoque_minimo = ? WHERE codigo = ?;";
    sqlite3_stmt *stmt = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao preparar UPDATE: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, p->nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, p->descricao, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p->categoria, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, p->preco);
    sqlite3_bind_int(stmt, 5, p->quantidade);
    sqlite3_bind_int(stmt, 6, p->estoque_minimo);
    sqlite3_bind_int(stmt, 7, p->codigo);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE);
}

int banco_excluir_produto(int codigo) {
    if (!db) return 0;

    const char *sql = "DELETE FROM produtos WHERE codigo = ?;";
    sqlite3_stmt *stmt = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao preparar DELETE: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, codigo);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE);
}

int banco_registrar_movimentacao_transacional(int codigo_produto, TipoMovimentacao tipo, int quantidade, const char *observacao, int novo_saldo) {
    if (!db || quantidade <= 0) return 0;

    char data_hora[TAM_DATA];
    obter_data_hora_atual(data_hora, sizeof(data_hora));

    char *err_msg = NULL;
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO TRANSACAO] Falha ao iniciar transacao: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }

    // 1. Inserir historico na tabela movimentacoes
    const char *sql_mov = "INSERT INTO movimentacoes (codigo_produto, tipo, quantidade, data_hora, observacao) "
                          "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt_mov = NULL;
    rc = sqlite3_prepare_v2(db, sql_mov, -1, &stmt_mov, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 0;
    }

    sqlite3_bind_int(stmt_mov, 1, codigo_produto);
    sqlite3_bind_int(stmt_mov, 2, (int)tipo);
    sqlite3_bind_int(stmt_mov, 3, quantidade);
    sqlite3_bind_text(stmt_mov, 4, data_hora, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt_mov, 5, observacao ? observacao : "", -1, SQLITE_STATIC);

    int rc_mov = sqlite3_step(stmt_mov);
    sqlite3_finalize(stmt_mov);

    // 2. Atualizar saldo na tabela produtos
    const char *sql_prod = "UPDATE produtos SET quantidade = ? WHERE codigo = ?;";
    sqlite3_stmt *stmt_prod = NULL;
    rc = sqlite3_prepare_v2(db, sql_prod, -1, &stmt_prod, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 0;
    }

    sqlite3_bind_int(stmt_prod, 1, novo_saldo);
    sqlite3_bind_int(stmt_prod, 2, codigo_produto);

    int rc_prod = sqlite3_step(stmt_prod);
    sqlite3_finalize(stmt_prod);

    // Validacao da transacao atomica
    if (rc_mov == SQLITE_DONE && rc_prod == SQLITE_DONE) {
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return 1;
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        fprintf(stderr, "[ERRO TRANSACAO] Falha na operacao atomica. ROLLBACK executado com sucesso.\n");
        return 0;
    }
}

int banco_carregar_matriz_movimentacoes(int ano, int matriz[MAX_PRODUTOS][MESES_ANO], const Produto *produtos, int total_produtos) {
    if (!db || !produtos || total_produtos <= 0) return 0;

    // Inicializar matriz bidimensional com zeros
    for (int i = 0; i < total_produtos; i++) {
        for (int m = 0; m < MESES_ANO; m++) {
            matriz[i][m] = 0;
        }
    }

    // Consulta para extrair mes e quantidade de movimentacoes filtradas pelo ano
    const char *sql = "SELECT codigo_produto, CAST(strftime('%m', data_hora) AS INTEGER) AS mes, "
                      "tipo, quantidade FROM movimentacoes WHERE CAST(strftime('%Y', data_hora) AS INTEGER) = ?;";
    sqlite3_stmt *stmt = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao preparar matriz: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, ano);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int codigo = sqlite3_column_int(stmt, 0);
        int mes = sqlite3_column_int(stmt, 1); // 1 a 12
        int tipo = sqlite3_column_int(stmt, 2);
        int qtd = sqlite3_column_int(stmt, 3);

        if (mes < 1 || mes > 12) continue;

        // Localizar a linha correspondente ao produto no array em RAM
        for (int i = 0; i < total_produtos; i++) {
            if (produtos[i].codigo == codigo) {
                // Fluxo liquido: Entrada adiciona, Saida subtrai
                if (tipo == MOV_ENTRADA) {
                    matriz[i][mes - 1] += qtd;
                } else {
                    matriz[i][mes - 1] -= qtd;
                }
                break;
            }
        }
    }

    sqlite3_finalize(stmt);
    return 1;
}

int banco_listar_historico_movimentacoes(int codigo_filtro) {
    if (!db) return 0;

    sqlite3_stmt *stmt = NULL;
    int rc = SQLITE_OK;

    if (codigo_filtro > 0) {
        const char *sql = "SELECT m.id, m.codigo_produto, p.nome, m.tipo, m.quantidade, m.data_hora, m.observacao "
                          "FROM movimentacoes m LEFT JOIN produtos p ON m.codigo_produto = p.codigo "
                          "WHERE m.codigo_produto = ? ORDER BY m.id DESC;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, codigo_filtro);
        }
    } else {
        const char *sql = "SELECT m.id, m.codigo_produto, p.nome, m.tipo, m.quantidade, m.data_hora, m.observacao "
                          "FROM movimentacoes m LEFT JOIN produtos p ON m.codigo_produto = p.codigo "
                          "ORDER BY m.id DESC LIMIT 50;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    }

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[ERRO BANCO] Falha ao consultar historico: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\n========================================================================================================\n");
    printf("%-5s | %-8s | %-24s | %-8s | %-5s | %-19s | %-20s\n",
           "ID", "COD_PROD", "NOME PRODUTO", "TIPO", "QTD", "DATA/HORA", "OBSERVACAO");
    printf("--------------------------------------------------------------------------------------------------------\n");

    int total_exibido = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int cod = sqlite3_column_int(stmt, 1);
        const unsigned char *nome = sqlite3_column_text(stmt, 2);
        int tipo = sqlite3_column_int(stmt, 3);
        int qtd = sqlite3_column_int(stmt, 4);
        const unsigned char *data = sqlite3_column_text(stmt, 5);
        const unsigned char *obs = sqlite3_column_text(stmt, 6);

        const char *tipo_str = (tipo == MOV_ENTRADA) ? "ENTRADA" : "SAIDA";

        printf("%-5d | %-8d | %-24.24s | %-8s | %-5d | %-19s | %-20.20s\n",
               id, cod, nome ? (const char *)nome : "N/D",
               tipo_str, qtd,
               data ? (const char *)data : "",
               obs ? (const char *)obs : "");
        total_exibido++;
    }

    printf("========================================================================================================\n");
    printf("Total de registros exibidos: %d\n", total_exibido);

    sqlite3_finalize(stmt);
    return total_exibido;
}
