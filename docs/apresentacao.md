# Roteiro de Apresentação e Defesa Acadêmica

**Projeto:** Sistema de Controle de Estoque (Projeto Integrador 2026)  
**Disciplina:** Algoritmos e Programação II — Engenharia da Computação  
**Objetivo:** Guia completo de preparação para a arguição técnica com a banca / professor.

---

## 14 Perguntas e Respostas Chave para a Defesa

### 1. Como funciona a busca linear implementada no projeto?
> **Resposta:**  
> A busca linear está implementada na função `busca_linear_nome()` em `src/algoritmos.c`. Ela realiza uma varredura sequencial através de um laço `for (int i = 0; i < total; i++)` percorrendo o array de structs `catalogo.itens`. A cada iteração, converte o nome do produto e o termo de pesquisa para letras minúsculas (via `string_para_minusculo()`) e utiliza `strstr()` para verificar se o termo está contido no nome. Se houver correspondência, o índice do elemento é salvo no vetor `indices_encontrados`. Sua complexidade é $O(n)$ no caso médio e pior caso, e sua grande vantagem é permitir buscas parciais sem exigir que o catálogo esteja ordenado previamente por nome.

---

### 2. Por que a busca binária precisa obrigatoriamente de dados ordenados?
> **Resposta:**  
> A busca binária fundamenta-se no paradigma de divisão e conquista. Em cada passo, ela compara a chave de busca com o elemento central do intervalo (`meio`). Caso a chave procurada seja menor, o algoritmo infere com certeza matemática que o elemento só pode residir na metade inferior do vetor; se for maior, na metade superior. Se o vetor não estiver ordenado de forma monotônica crescente, essa inferência perde a validade lógica: poderíamos descartar uma metade que por acaso continha o item procurado. Por isso, nossa função `sistema_pesquisar_por_codigo()` sempre verifica se o array está ordenado (`verificar_ordenado_por_codigo()`) e, se necessário, executa o `ordenar_insertion()` antes de disparar a busca binária.

---

### 3. Qual algoritmo de ordenação foi utilizado e como ele funciona?
> **Resposta:**  
> Implementamos três algoritmos manuais em `src/algoritmos.c`: **Insertion Sort**, **Bubble Sort** e **Selection Sort**, tendo o **Insertion Sort** como algoritmo principal do fluxo de trabalho. O Insertion Sort funciona de forma análoga a organizar cartas de baralho na mão: começa no segundo elemento ($i = 1$) e guarda seu valor em uma variável pivô. Em seguida, compara o pivô com os elementos anteriores através de um laço `while (j >= 0 && comparar_produtos(...) > 0)` e vai deslocando os elementos maiores uma posição à direita. Quando encontra a posição correta, insere o pivô. Ele possui complexidade $O(n)$ no melhor caso (quando o vetor já está ordenado) e $O(n^2)$ no pior caso, operando *in-place* e de forma estável.

---

### 4. Onde os ponteiros aparecem no projeto e por que foram utilizados?
> **Resposta:**  
> Os ponteiros são empregados em três cenários essenciais:
> 1. **Passagem por Referência para Modificação de Estado:** Na função `atualizar_estoque(Produto *p, int quantidade, TipoMovimentacao tipo)` em `src/util.c`, onde recebemos o endereço de memória de um produto no array e alteramos diretamente seu campo `p->quantidade`, sem fazer cópias desnecessárias na pilha de execução.
> 2. **Parâmetros de Saída Seguros:** Nas rotinas de entrada `ler_inteiro_seguro(const char *msg, int *saida, ...)` e `ler_double_seguro(...)`, onde os ponteiros permitem retornar o valor numérico lido enquanto o retorno da função indica o sucesso (1) ou falha (0).
> 3. **Manipulação de Recursos Externos:** No driver do SQLite (`sqlite3 *db` e `sqlite3_stmt *stmt`), onde todos os identificadores e transações operam através de ponteiros opacos gerenciados pela biblioteca.

---

### 5. Como o programa em C se conecta ao banco de dados SQLite?
> **Resposta:**  
> O programa utiliza a biblioteca nativa C do SQLite (`#include <sqlite3.h>`). Na função `banco_inicializar()` em `src/banco.c`, invocamos a função `sqlite3_open(caminho, &db)`, que recebe o caminho do arquivo no disco (`database/estoque.db`) e aloca a estrutura de conexão na memória através do ponteiro `db`. Se o arquivo não existir, o SQLite o cria automaticamente. Em seguida, executamos com `sqlite3_exec()` o script DDL que cria as tabelas `produtos` e `movimentacoes` caso ainda não existam.

---

### 6. Como os dados permanecem disponíveis depois que o programa é encerrado?
> **Resposta:**  
> Os dados não ficam restritos à memória volátil (RAM). Todas as alterações (inserção, atualização, exclusão e movimentações de estoque) são gravadas no arquivo binário `database/estoque.db` no disco rígido através do motor SQLite3. Quando a aplicação é encerrada (`finalizar_sistema()`), o banco é fechado com integridade via `sqlite3_close()`. Ao iniciar o programa novamente, a função `banco_carregar_produtos()` executa um `SELECT` no banco e remonta o catálogo no array de structs em RAM. Dessa forma, a persistência é total e independente do ciclo de vida do processo.

---

### 7. Por que vocês não usaram comandos SQL como `ORDER BY` e `WHERE` para substituir a ordenação e busca em C?
> **Resposta:**  
> Conforme a diretriz imperativa do professor e o objetivo pedagógico da disciplina de Algoritmos e Programação II, o banco de dados tem a função estrita de persistência (armazenamento e recuperação). Substituir a busca linear, a busca binária ou a ordenação manual por cláusulas SQL prontas anularia a demonstração prática das estruturas de dados e dos algoritmos fundamentais da Engenharia da Computação que a disciplina se propõe a avaliar.

---

### 8. Como funciona a integridade transacional no registro de movimentações?
> **Resposta:**  
> Na função `banco_registrar_movimentacao_transacional()` em `src/banco.c`, executamos `BEGIN TRANSACTION;`. Em seguida, realizamos duas operações interdependentes: (1) inserir o histórico na tabela `movimentacoes` e (2) atualizar o campo `quantidade` na tabela `produtos`. Se ambas as instruções executarem com status `SQLITE_DONE`, emitimos o comando `COMMIT;`. Caso qualquer uma falhe, emitimos imediatamente um `ROLLBACK;`, revertendo o banco ao estado original. Isso garante a propriedade de **Atomicidade** do padrão ACID, impedindo que ocorram saídas de estoque sem o respectivo registro de auditoria.

---

### 9. Como o sistema previne ataques de SQL Injection e falhas de formatação?
> **Resposta:**  
> O sistema não faz interpolação de strings nem usa `sprintf` para montar consultas SQL. Utilizamos exclusivamente consultas preparadas e parametrizadas:
> 1. `sqlite3_prepare_v2()` para compilar a consulta com marcadores `?`.
> 2. Funções `sqlite3_bind_int()`, `sqlite3_bind_text()` e `sqlite3_bind_double()` para associar os valores diretamente aos parâmetros tipados.
> O SQLite trata automaticamente aspas, caracteres especiais e valida os tipos, eliminando completamente brechas de injeção de SQL.

---

### 10. Por que utilizar `fgets()` combinado com `strtol()` / `strtod()` em vez do tradicional `scanf()`?
> **Resposta:**  
> O `scanf()` tradicional é propenso a falhas graves em interfaces de console: se o usuário digitar uma letra quando um número é esperado, o `scanf()` falha na conversão, deixa o caractere na fila do buffer `stdin` e as leituras subsequentes entram em loop infinito de erros. Além disso, `scanf("%s")` não previne estouro de buffer (*buffer overflow*). Nossa função `ler_inteiro_seguro()` lê a linha inteira com `fgets()` limitando o tamanho, remove quebras de linha e converte com `strtol()`, auditando o ponteiro `endptr` para garantir que apenas números válidos sejam aceitos.

---

### 11. Como a Matriz Bidimensional (2D) foi aplicada no contexto do projeto?
> **Resposta:**  
> Implementamos em C uma matriz `int matriz_mov[MAX_PRODUTOS][MESES_ANO]`. As linhas correspondem aos índices dos produtos em memória RAM e as colunas aos 12 meses do ano civil (Janeiro a Dezembro). A função `banco_carregar_matriz_movimentacoes()` consulta o histórico de movimentações daquele ano no SQLite e preenche cada célula $[i][m]$ com o saldo líquido de movimentações daquele produto naquele mês. O sistema então processa as somas marginais por linha (balanço anual do item) e por coluna (balanço mensal da empresa inteira), permitindo analisar a densidade de entrada e saída por período.

---

### 12. Qual a diferença entre structs e arrays de structs no projeto?
> **Resposta:**  
> A `struct Produto` define o molde/registro de um produto único, encapsulando dados heterogêneos (inteiros para código e quantidade, cadeias de caracteres para nome e categoria, e double para preço). Já o array de structs `catalogo.itens[MAX_PRODUTOS]` aloca um bloco sequencial contíguo de memória capaz de comportar até 1.000 instâncias dessa struct. Isso permite acesso indexado em tempo $O(1)$ a qualquer registro (`catalogo.itens[i]`), facilitando os algoritmos de ordenação e busca na memória principal.

---

### 13. O que aconteceria se tentássemos excluir um produto que possui movimentações registradas?
> **Resposta:**  
> No arquivo `database/schema.sql`, definimos a chave estrangeira com a diretiva `ON DELETE CASCADE`:
> ```sql
> FOREIGN KEY (codigo_produto) REFERENCES produtos(codigo) ON DELETE CASCADE
> ```
> Além disso, habilitamos `PRAGMA foreign_keys = ON;` na abertura do banco. Ao excluir um produto, o motor do SQLite remove automaticamente em cascata todo o histórico de movimentações atrelado àquele código, preservando a integridade referencial do banco de dados e impedindo a existência de registros órfãos.

---

### 14. Como vocês garantiram que o programa não possui vazamento de memória ou estouro de buffer?
> **Resposta:**  
> Configuramos a flag de compilação `-fsanitize=address,undefined` no Makefile. Essa ferramenta de sanitização (AddressSanitizer) monitora os acessos à memória em tempo de execução. O projeto foi desenhado priorizando o controle estrito de limites: todas as leituras de string utilizam `sizeof(buffer)`, os arrays possuem tamanho pré-determinado com checagem de limites (`catalogo.total < MAX_PRODUTOS`) e toda estrutura do SQLite aberta (`sqlite3_stmt*`) é finalizada com `sqlite3_finalize()`. O resultado dos testes com AddressSanitizer acusou zero falhas de memória.
