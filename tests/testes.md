# Plano e Registro de Testes de Software

**Projeto:** Sistema de Controle de Estoque (Projeto Integrador 2026)  
**Disciplina:** Algoritmos e Programação II — Engenharia da Computação  
**Ambiente de Testes:** Linux GCC C11 / MinGW64 com SQLite3  

---

## 1. Testes de Compilação Rigorosa e Sanitização de Memória

### Teste 1.1: Compilação Estrita C11
- **Comando:**
  ```bash
  gcc -std=c11 -Wall -Wextra -Wpedantic -g main.c src/*.c -lsqlite3 -o sistema
  ```
- **Critério de Sucesso:** Zero avisos (warnings) e zero erros.
- **Resultado:** Aprovado. Código em conformidade total com o padrão C11 e flags `-pedantic`.

### Teste 1.2: Auditoria de Memória (AddressSanitizer + UndefinedBehaviorSanitizer)
- **Comando:**
  ```bash
  gcc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.c src/*.c -lsqlite3 -o sistema
  ./sistema
  ```
- **Itens Auditados:**
  - Ausência de estouro de buffer de array (`buffer overflow` em strings e no array `catalogo.itens`).
  - Verificação de ponteiros nulos (`null pointer dereference`).
  - Prevenção de vazamento de memória (`memory leak`).
- **Resultado:** Aprovado. Toda a alocação de structs é mantida em memória estática/automática com limites pré-definidos e buffers seguros com `fgets()`.

---

## 2. Teste Obrigatório de Persistência e Integridade do SQLite

### Roteiro Executado:
1. **Passo 1 (Inicialização Limpa):**
   - O arquivo `database/estoque.db` não existe inicialmente.
   - O sistema é iniciado; o SQLite cria automaticamente as tabelas `produtos` e `movimentacoes` e os índices conforme `database/schema.sql`.
2. **Passo 2 (Cadastro e Movimentação Inicial):**
   - Cadastrado Produto: `Código: 101`, `Nome: Parafuso Sextavado`, `Categoria: Fixadores`, `Preço: 1.50`, `Estoque Inicial: 100`, `Estoque Mínimo: 20`.
   - Cadastrado Produto: `Código: 102`, `Nome: Porca Parlock M8`, `Categoria: Fixadores`, `Preço: 0.80`, `Estoque Inicial: 15`, `Estoque Mínimo: 30` (disparando alerta de baixo estoque).
3. **Passo 3 (Movimentação Transacional):**
   - Registrada SAÍDA de 40 unidades do Produto 101 com observação "Envio Produção OP-44".
   - Saldo atualizado em RAM e no SQLite para 60 unidades.
4. **Passo 4 (Fechamento do Sistema):**
   - Executada a Opção `0` (Sair do Sistema). Conexão SQLite encerrada via `sqlite3_close()`.
5. **Passo 5 (Reinicialização e Auditoria):**
   - Reiniciado o binário `./sistema`.
   - Verificado o log: `[SUCESSO] 2 produtos carregados do SQLite para a memória RAM`.
   - Consulta de listagem (Opção 4) e histórico de movimentações (Opção 12).
- **Resultado:** Aprovado. 100% dos dados cadastrados, saldos e histórico de movimentações foram restaurados com integridade e fidelidade absoluta.

---

## 3. Matriz de Casos de Teste Funcionais e de Limite (Edge Cases)

| ID | Cenário de Teste | Entrada Fornecida | Comportamento Esperado | Status |
|:---|:---|:---|:---|:---:|
| **CT-01** | Código Duplicado | Cadastro com código `101` já existente | Sistema detecta código em RAM/SQLite, emite mensagem de erro e solicita novo código | **PASSOU** |
| **CT-02** | Entrada com Letras em Campo Numérico | Entrada de `"abc"` no código ou preço | `ler_inteiro_seguro` rejeita, limpa buffer e solicita digitação correta | **PASSOU** |
| **CT-03** | Saída Maior que o Saldo Disponível | Retirada de 500 unidades quando há apenas 60 | Sistema bloqueia operação ("Saldo insuficiente"), não gera registro | **PASSOU** |
| **CT-04** | Preço Negativo ou Zero | Entrada de `-15.50` no preço | `ler_double_seguro` valida `min: 0.01` e impede valor inválido | **PASSOU** |
| **CT-05** | Busca Binária com Código Inexistente | Pesquisar código `999` | Algoritmo divide e conquista, atinge `inicio > fim` e retorna -1 | **PASSOU** |
| **CT-06** | Busca Linear por Termo Parcial | Pesquisar `"par"` | Algoritmo manual localiza `"Parafuso Sextavado"` e `"Porca Parlock"` | **PASSOU** |
| **CT-07** | Ordenação Manual (Insertion/Bubble/Selection) | Ordenar por Preço e por Quantidade | Array em RAM tem suas structs reorganizadas sem recurso a bibliotecas prontas | **PASSOU** |
| **CT-08** | Matriz 2D de Movimentações | Solicitar matriz do ano 2026 | Matriz [Produtos x 12 Meses] é preenchida e exibe balanço líquido mensal | **PASSOU** |
| **CT-09** | Transação SQL com Falha Induzida | Tentativa de atualizar produto inexistente | Bloco `ROLLBACK` é acionado preservando integridade das tabelas | **PASSOU** |
| **CT-10** | Remoção com Cascata | Excluir produto com movimentações registradas | `PRAGMA foreign_keys = ON` e `ON DELETE CASCADE` garantem consistência | **PASSOU** |
