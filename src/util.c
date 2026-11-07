#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/util.h"

int ler_inteiro_seguro(const char *mensagem, int *saida, int min, int max) {
    char buffer[128];
    char *endptr = NULL;
    long valor = 0;

    if (!saida) return 0;

    while (1) {
        printf("%s", mensagem);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            clearerr(stdin);
            return 0;
        }

        // Remover newline
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Linha vazia não é aceita
        if (strlen(buffer) == 0) {
            printf("[ERRO] Entrada não pode ser vazia. Tente novamente.\n");
            continue;
        }

        valor = strtol(buffer, &endptr, 10);

        // Validar se toda a string foi convertida
        if (endptr == buffer || *endptr != '\0') {
            printf("[ERRO] Valor numérico inválido. Digite apenas dígitos inteiros.\n");
            continue;
        }

        // Validar limites permitidos
        if (valor < min || valor > max) {
            printf("[ERRO] O valor deve estar no intervalo entre %d e %d.\n", min, max);
            continue;
        }

        *saida = (int)valor;
        return 1;
    }
}

int ler_double_seguro(const char *mensagem, double *saida, double min, double max) {
    char buffer[128];
    char *endptr = NULL;
    double valor = 0.0;

    if (!saida) return 0;

    while (1) {
        printf("%s", mensagem);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            clearerr(stdin);
            return 0;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strlen(buffer) == 0) {
            printf("[ERRO] Entrada não pode ser vazia. Tente novamente.\n");
            continue;
        }

        // Suporte a vírgula como separador decimal (padrão brasileiro)
        for (size_t i = 0; i < strlen(buffer); i++) {
            if (buffer[i] == ',') {
                buffer[i] = '.';
            }
        }

        valor = strtod(buffer, &endptr);

        if (endptr == buffer || *endptr != '\0') {
            printf("[ERRO] Valor decimal inválido. Utilize ponto ou vírgula.\n");
            continue;
        }

        if (valor < min || valor > max) {
            printf("[ERRO] O valor deve estar no intervalo entre %.2f e %.2f.\n", min, max);
            continue;
        }

        *saida = valor;
        return 1;
    }
}

void ler_string_segura(const char *mensagem, char *destino, size_t tamanho) {
    if (!destino || tamanho == 0) return;

    while (1) {
        printf("%s", mensagem);
        if (!fgets(destino, (int)tamanho, stdin)) {
            clearerr(stdin);
            destino[0] = '\0';
            return;
        }

        destino[strcspn(destino, "\r\n")] = '\0';

        if (strlen(destino) == 0) {
            printf("[ERRO] O campo não pode ficar em branco. Digite novamente.\n");
            continue;
        }
        break;
    }
}

int confirmar_acao(const char *mensagem) {
    char buffer[32];
    while (1) {
        printf("%s (S/N): ", mensagem);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 0;
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) == 0) continue;

        char c = (char)toupper((unsigned char)buffer[0]);
        if (c == 'S') return 1;
        if (c == 'N') return 0;

        printf("[AVISO] Opção inválida. Digite 'S' para Sim ou 'N' para Não.\n");
    }
}

void obter_data_hora_atual(char *buffer, size_t tamanho) {
    if (!buffer || tamanho == 0) return;
    time_t agora = time(NULL);
    struct tm *info_tempo = localtime(&agora);
    if (info_tempo) {
        strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", info_tempo);
    } else {
        snprintf(buffer, tamanho, "2026-11-07 00:00:00");
    }
}

int atualizar_estoque(Produto *p, int quantidade, TipoMovimentacao tipo) {
    if (!p || quantidade <= 0) {
        return 0;
    }

    if (tipo == MOV_ENTRADA) {
        p->quantidade += quantidade;
        return 1;
    } else if (tipo == MOV_SAIDA) {
        if (p->quantidade < quantidade) {
            return 0; // Saldo insuficiente
        }
        p->quantidade -= quantidade;
        return 1;
    }

    return 0;
}

void string_para_minusculo(const char *origem, char *destino) {
    if (!origem || !destino) return;
    size_t i = 0;
    while (origem[i] != '\0') {
        destino[i] = (char)tolower((unsigned char)origem[i]);
        i++;
    }
    destino[i] = '\0';
}

int contem_substring_sem_caso(const char *texto, const char *termo) {
    if (!texto || !termo) return 0;
    char buffer_texto[512];
    char buffer_termo[512];

    string_para_minusculo(texto, buffer_texto);
    string_para_minusculo(termo, buffer_termo);

    return (strstr(buffer_texto, buffer_termo) != NULL);
}

void pausar_console(void) {
    printf("\n[Pressione ENTER para continuar...]");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_tela(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}
