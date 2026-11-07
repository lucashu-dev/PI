# ==============================================================================
# MAKEFILE - PROJETO INTEGRADOR: ALGORITMOS E PROGRAMAÇÃO II (2026)
# Engenharia da Computação
# ==============================================================================

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude
LDFLAGS = -lsqlite3

SRC = main.c src/sistema.c src/algoritmos.c src/banco.c src/util.c
OBJ = $(SRC:.c=.o)
TARGET = sistema

.PHONY: all clean run sanitize help

# Alvo padrão de compilação rigorosa
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "=========================================================="
	@echo " Compilacao concluida com sucesso: ./$(TARGET)"
	@echo "=========================================================="

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Alvo com sanitizadores de memória (AddressSanitizer e UndefinedBehavior)
sanitize: CFLAGS += -fsanitize=address,undefined
sanitize: LDFLAGS += -fsanitize=address,undefined
sanitize: clean $(TARGET)
	@echo "Binario compilado com AddressSanitizer ativado."

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(TARGET).exe database/estoque.db

help:
	@echo "Comandos disponiveis no Makefile:"
	@echo "  make          - Compila o projeto com C11 e flags rigorosas"
	@echo "  make sanitize - Compila com AddressSanitizer (-fsanitize=address,undefined)"
	@echo "  make run      - Compila e executa o binario"
	@echo "  make clean    - Remove binarios, objetos e arquivo de banco local"
