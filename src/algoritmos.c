#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/algoritmos.h"
#include "../include/util.h"

int comparar_produtos(const Produto *a, const Produto *b, CriterioOrdenacao criterio) {
    if (!a || !b) return 0;

    switch (criterio) {
        case CRITERIO_CODIGO:
            if (a->codigo < b->codigo) return -1;
            if (a->codigo > b->codigo) return 1;
            return 0;

        case CRITERIO_NOME: {
            char nome_a[TAM_NOME];
            char nome_b[TAM_NOME];
            string_para_minusculo(a->nome, nome_a);
            string_para_minusculo(b->nome, nome_b);
            return strcmp(nome_a, nome_b);
        }

        case CRITERIO_PRECO:
            if (a->preco < b->preco) return -1;
            if (a->preco > b->preco) return 1;
            return 0;

        case CRITERIO_QUANTIDADE:
            if (a->quantidade < b->quantidade) return -1;
            if (a->quantidade > b->quantidade) return 1;
            return 0;

        default:
            return 0;
    }
}

int busca_linear_nome(const Produto *vetor, int total, const char *nome_busca, int indices_encontrados[], int max_resultados) {
    if (!vetor || total <= 0 || !nome_busca || !indices_encontrados || max_resultados <= 0) {
        return 0;
    }

    int encontrados = 0;

    // Varredura sequencial manual elemento por elemento
    for (int i = 0; i < total; i++) {
        if (contem_substring_sem_caso(vetor[i].nome, nome_busca)) {
            if (encontrados < max_resultados) {
                indices_encontrados[encontrados] = i;
                encontrados++;
            } else {
                break; // Limite de armazenamento de índices atingido
            }
        }
    }

    return encontrados;
}

int busca_binaria_codigo(const Produto *vetor, int total, int codigo_busca) {
    if (!vetor || total <= 0) {
        return -1;
    }

    int inicio = 0;
    int fim = total - 1;

    // Algoritmo de divisão e conquista clássico O(log n)
    while (inicio <= fim) {
        // Prevenção contra overflow de inteiros em (inicio + fim) / 2
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio].codigo == codigo_busca) {
            return meio; // Registro localizado
        }

        if (vetor[meio].codigo < codigo_busca) {
            inicio = meio + 1; // Busca na metade superior
        } else {
            fim = meio - 1;    // Busca na metade inferior
        }
    }

    return -1; // Registro não encontrado
}

void ordenar_insertion(Produto *vetor, int total, CriterioOrdenacao criterio) {
    if (!vetor || total <= 1) return;

    for (int i = 1; i < total; i++) {
        Produto pivo = vetor[i];
        int j = i - 1;

        // Desloca elementos maiores que o pivô para a direita
        while (j >= 0 && comparar_produtos(&vetor[j], &pivo, criterio) > 0) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = pivo;
    }
}

void ordenar_bubble(Produto *vetor, int total, CriterioOrdenacao criterio) {
    if (!vetor || total <= 1) return;

    for (int i = 0; i < total - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < total - 1 - i; j++) {
            if (comparar_produtos(&vetor[j], &vetor[j + 1], criterio) > 0) {
                Produto temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                trocou = 1;
            }
        }
        // Otimização: se nenhuma troca ocorreu, o array já está ordenado
        if (!trocou) {
            break;
        }
    }
}

void ordenar_selection(Produto *vetor, int total, CriterioOrdenacao criterio) {
    if (!vetor || total <= 1) return;

    for (int i = 0; i < total - 1; i++) {
        int menor_indice = i;
        for (int j = i + 1; j < total; j++) {
            if (comparar_produtos(&vetor[j], &vetor[menor_indice], criterio) < 0) {
                menor_indice = j;
            }
        }
        if (menor_indice != i) {
            Produto temp = vetor[i];
            vetor[i] = vetor[menor_indice];
            vetor[menor_indice] = temp;
        }
    }
}

int verificar_ordenado_por_codigo(const Produto *vetor, int total) {
    if (!vetor || total <= 1) return 1;

    for (int i = 0; i < total - 1; i++) {
        if (vetor[i].codigo > vetor[i + 1].codigo) {
            return 0; // Desordenado
        }
    }
    return 1;
}
