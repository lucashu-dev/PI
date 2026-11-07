/**
 * ============================================================================
 * PROJETO INTEGRADOR - ALGORITMOS E PROGRAMAÇÃO II (2026)
 * CURSO: Engenharia da Computação
 * DOMÍNIO: Sistema de Controle de Estoque
 *
 * Arquivo: main.c
 * Função: Ponto de entrada modular da aplicação (Orquestrador)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "include/sistema.h"

int main(void) {
    // Inicialização de subsistemas (SQLite e Carga em RAM)
    inicializar_sistema();

    // Loop de controle da interface de console
    menu_principal();

    // Finalização e desalocação de conexões ativas
    finalizar_sistema();

    return EXIT_SUCCESS;
}
