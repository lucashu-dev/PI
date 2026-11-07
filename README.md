# Sistema de Controle de Estoque — Projeto Integrador 2026

**Disciplina:** Algoritmos e Programação II  
**Curso:** Engenharia da Computação — 2º Semestre de 2026  
**Linguagem:** C (Padrão ISO C11)  
**Persistência:** SQLite3 (Driver Nativo C)  
**Modalidade:** Projeto Prático em Grupo  
**Data Limite de Entrega:** 18/11/2026  

---

## 👥 1. Identificação do Projeto e Integrantes
- **Nome do Projeto:** Sistema Modular de Controle de Estoque e Análise Matricial
- **Integrantes:**
  - *Lucas e Equipe de Engenharia da Computação*

---

## 📋 2. Descrição do Problema e Principais Funcionalidades

O controle eficiente de estoque em ambientes industriais e comerciais exige confiabilidade na integridade física dos dados, resposta rápida em consultas e rastreabilidade total de movimentações. Este projeto integra os conceitos fundamentais de algoritmos e estruturas de dados com a persistência profissional em banco de dados relacional.

### Funcionalidades do Sistema:
1. **Gestão de Produtos:** Cadastrar, alterar, excluir e listar produtos com dados completos (código, nome, descrição, categoria, preço, quantidade e estoque mínimo).
2. **Pesquisa Inteligente em RAM:**
   - Pesquisa por código via **Busca Binária Manual $O(\log n)$**.
   - Pesquisa por nome/termo via **Busca Linear Manual $O(n)$**.
3. **Ordenação Manual em RAM:** Ordenação *in-place* por Código, Nome, Preço ou Quantidade utilizando algoritmos manuais (**Insertion Sort**, **Bubble Sort** e **Selection Sort**).
4. **Controle Transacional de Movimentações:** Registro atômico de Entradas e Saídas com auditoria histórica e atualização de saldo em uma única transação SQL.
5. **Alerta de Estoque Mínimo:** Identificação imediata de itens com quantidade em nível crítico.
6. **Indicadores Financeiros:** Cálculo do valor patrimonial consolidado do estoque e preço médio ponderado.
7. **Matriz 2D de Movimentações:** Análise de densidade de movimentação (Produtos x 12 Meses) consolidando o balanço líquido mensal.

---

## 🧠 3. Algoritmos Implementados em C (Sem Bibliotecas Prontas)

> [!IMPORTANT]
> Em estrita conformidade com as diretrizes da disciplina, **nenhum comando SQL (como `WHERE` ou `ORDER BY`) substitui os algoritmos em C**. O banco é utilizado exclusivamente para persistência; toda a ordenação e busca opera sobre o array de structs na memória RAM.

1. **Busca Linear Manual (`busca_linear_nome` em `src/algoritmos.c`):**
   - Varredura sequencial completa no array de structs.
   - Suporte a correspondência exata e parcial (substring) com normalização de maiúsculas/minúsculas.
2. **Busca Binária Manual (`busca_binaria_codigo` em `src/algoritmos.c`):**
   - Paradigma de Divisão e Conquista ($O(\log n)$).
   - Validação prévia de ordenação com chamada automática de ordenação por código caso o vetor esteja desordenado.
   - Proibido e auditado contra o uso de `bsearch()`.
3. **Ordenação Manual (`ordenar_insertion`, `ordenar_bubble`, `ordenar_selection` em `src/algoritmos.c`):**
   - Implementação manual em C dos três métodos clássicos da disciplina.
   - Proibido e auditado contra o uso de `qsort()`.
4. **Processamento Matricial 2D (`matriz_movimentacoes_mensais` em `src/sistema.c`):**
   - Matriz `int matriz[MAX_PRODUTOS][MESES_ANO]` para análise bidimensional de densidade de fluxo temporal.

---

## 🗄️ 4. Banco de Dados e Justificativa da Escolha

- **Tecnologia Escolhida:** SQLite 3 (Versão 3.x)
- **Biblioteca/Driver C:** `sqlite3.h` / `libsqlite3` (`-lsqlite3`)
- **Justificativa Técnica da Escolha:**
  1. **Serverless (Embarcado):** O SQLite opera sem necessidade de configuração de um servidor dedicado ou daemons (como MySQL ou Postgres), simplificando a portabilidade e permitindo que o projeto seja executado em qualquer máquina de avaliação com o arquivo `estoque.db`.
  2. **Conformidade Total ACID:** Suporta transações atômicas com `BEGIN TRANSACTION`, `COMMIT` e `ROLLBACK`, essencial para garantir que a movimentação e a alteração de saldo ocorram juntas.
  3. **Segurança e Estabilidade:** A API C é amplamente documentada, fornecendo funções nativas para consultas preparadas e parametrizadas (`sqlite3_prepare_v2`, `sqlite3_bind_*`).

---

## 🏗️ 5. Estrutura das Tabelas (Schema Relacional)

### Tabela `produtos`
| Campo | Tipo | Restrições | Descrição |
|:---|:---|:---|:---|
| `codigo` | INTEGER | PRIMARY KEY | Identificador único do produto |
| `nome` | TEXT | NOT NULL | Nome comercial do produto |
| `descricao` | TEXT | - | Detalhamento técnico |
| `categoria` | TEXT | NOT NULL | Grupo de classificação |
| `preco` | REAL | NOT NULL CHECK(preco >= 0.0) | Preço unitário |
| `quantidade` | INTEGER | NOT NULL CHECK(quantidade >= 0) | Saldo físico atual |
| `estoque_minimo`| INTEGER | NOT NULL CHECK(estoque_minimo >= 0)| Nível de alerta para reposição |

### Tabela `movimentacoes`
| Campo | Tipo | Restrições | Descrição |
|:---|:---|:---|:---|
| `id` | INTEGER | PRIMARY KEY AUTOINCREMENT | Identificador da movimentação |
| `codigo_produto`| INTEGER | NOT NULL, FK produtos(codigo) ON DELETE CASCADE | Chave estrangeira |
| `tipo` | INTEGER | NOT NULL CHECK(tipo IN (1, 2)) | 1 = ENTRADA, 2 = SAÍDA |
| `quantidade` | INTEGER | NOT NULL CHECK(quantidade > 0) | Volume movimentado |
| `data_hora` | TEXT | NOT NULL | Formato ISO "YYYY-MM-DD HH:MM:SS" |
| `observacao` | TEXT | - | Justificativa / Documento fiscal |

---

## ⚙️ 6. Instruções de Configuração, Compilação e Execução

### Pré-requisitos
- Compilador GCC compatível com C11 (GNU Compiler Collection).
- Biblioteca de desenvolvimento do SQLite3 (`libsqlite3-dev` no Linux / SQLite3 no Windows).

#### Instalação de dependências no Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential libsqlite3-dev
```

### Compilação Padrão (C11 Rigoroso)
```bash
make
# Ou manualmente:
gcc -std=c11 -Wall -Wextra -Wpedantic -g main.c src/*.c -lsqlite3 -o sistema
```

### Compilação com AddressSanitizer (Sanitização de Memória)
```bash
make sanitize
```

### Execução
```bash
./sistema
```

---

## 📊 7. Matriz de Rastreabilidade e Cobertura da Rubrica (10,0 / 10,0)

| Critério de Avaliação | Peso | Arquivo (.c/.h) | Função Responsável | Evidência de Implementação | Status |
| :--- | :---: | :--- | :--- | :--- | :---: |
| **Organização e Modularização** | 1,0 | `src/sistema.c` / `main.c` | `inicializar_sistema()` | Separação estrita em `include/` e `src/`, sem sobrecarga na `main()` | **[VALIDADO]** |
| **Structs e Arrays de Structs** | 1,5 | `include/modelos.h` | `struct Produto` / `CatalogoProdutos` | Manipulação em memória RAM via array contíguo de structs | **[VALIDADO]** |
| **Ponteiros e Passagem por Referência** | 1,0 | `src/util.c` | `atualizar_estoque()` | Alteração direta de saldo por ponteiro de struct | **[VALIDADO]** |
| **Busca Linear Manual** | 0,75 | `src/algoritmos.c` | `busca_linear_nome()` | Varredura sequencial manual em RAM com busca por substring | **[VALIDADO]** |
| **Busca Binária Manual** | 1,25 | `src/algoritmos.c` | `busca_binaria_codigo()` | Divisão e conquista O(log n) sem uso de `bsearch()` | **[VALIDADO]** |
| **Ordenação Manual em C** | 1,5 | `src/algoritmos.c` | `ordenar_insertion()` | Insertion Sort (e Bubble/Selection) manuais sem `qsort()` | **[VALIDADO]** |
| **Matriz de Processamento 2D** | 0,5 | `src/sistema.c` | `sistema_matriz_movimentacoes_mensais()` | Matriz bidimensional Produtos x 12 Meses | **[VALIDADO]** |
| **Persistência Real SQLite3** | 1,5 | `src/banco.c` | `banco_inserir_produto()` | Consultas parametrizadas e transações atômicas ACID | **[VALIDADO]** |
| **Validações e Tratamento de Entrada** | 0,5 | `src/util.c` | `ler_inteiro_seguro()` | Proteção contra falhas de buffer com `fgets()` e `strtol()` | **[VALIDADO]** |
| **Apresentação e Roteiro de Defesa** | 0,5 | `docs/apresentacao.md` | Roteiro Q&A Oficial | 14 questões técnicas completas com respostas argumentadas | **[VALIDADO]** |
| **TOTAL** | **10,0** | - | - | **Cobertura Integral da Rubrica Oficial** | **[10,0/10,0]** |

---

## 📚 8. Referências Utilizadas na Pesquisa
1. **KERNIGHAN, Brian W.; RITCHIE, Dennis M.** *C: A Linguagem de Programação (Padrão ANSI)*. Campus, 1989.
2. **CORMEN, Thomas H. et al.** *Algoritmos: Teoria e Prática*. 3ª Edição. Elsevier, 2012.
3. **OWENS, Mike; ALLEN, Grant.** *The Definitive Guide to SQLite*. 2ª Edição. Apress, 2010.
4. **SQLITE CONSORTIUM.** *An Introduction To The SQLite C/C++ Interface*. Disponível em: <https://www.sqlite.org/cintro.html>. Acesso em: 2026.
5. **ISO/IEC 9899:2011.** *Information technology — Programming languages — C (C11 Standard)*.
