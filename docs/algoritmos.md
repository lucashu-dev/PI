# Análise Teórica e Prática dos Algoritmos Implementados

**Disciplina:** Algoritmos e Programação II — Engenharia da Computação  
**Projeto:** Sistema de Controle de Estoque (2026)  

---

## 1. Busca Linear Manual

### 1.1 Princípio de Funcionamento
A **Busca Linear** (ou Busca Sequencial) percorre sequencialmente o array de structs do primeiro índice (`i = 0`) até o último elemento válido (`i = total - 1`), comparando a chave de busca (no caso, o nome do produto ou substring dele) com o registro atual.

### 1.2 Por que a Busca Linear é ideal para busca por Nome?
1. Não exige que os dados estejam ordenados alfabeticamente na memória.
2. Permite buscas parciais (substrings), como encontrar "Parafuso Sextavado" ao pesquisar apenas "paraf".
3. Funciona de forma confiável mesmo após inserções arbitrárias no final do array.

### 1.3 Análise de Complexidade (Big-O)
- **Melhor Caso:** $O(1)$ — O elemento procurado é a primeira struct do array (`i = 0`).
- **Caso Médio:** $O(n/2) \Rightarrow O(n)$ — O elemento está distribuído aleatoriamente ao longo do array.
- **Pior Caso:** $O(n)$ — O elemento procurado é o último ou não está presente no array.
- **Complexidade de Espaço:** $O(1)$ — Opera *in-place*, utilizando apenas variáveis escalares para índices.

### 1.4 Exemplo de Rastreamento (Teste de Mesa)
Buscando pelo termo `"m8"` em um catálogo de 4 produtos:
- Vetor: `[0: "Arruela Lisa 1/4", 1: "Barra Roscada M8", 2: "Porca Parlock M8", 3: "Parafuso Phillips"]`
- `i = 0`: "Arruela Lisa 1/4" contém "m8"? **Não.**
- `i = 1`: "Barra Roscada M8" contém "m8"? **Sim!** Registra índice `1`.
- `i = 2`: "Porca Parlock M8" contém "m8"? **Sim!** Registra índice `2`.
- `i = 3`: "Parafuso Phillips" contém "m8"? **Não.**
- **Retorno:** 2 registros encontrados nos índices `[1, 2]`.

---

## 2. Busca Binária Manual

### 2.1 Princípio de Funcionamento
A **Busca Binária** aplica o paradigma de **Divisão e Conquista**. A cada iteração, ela calcula a posição intermediária (`meio = inicio + (fim - inicio) / 2`) e compara a chave do elemento do meio com o código procurado:
- Se for igual, o elemento foi encontrado.
- Se o código procurado for menor, descarta-se toda a metade superior (`fim = meio - 1`).
- Se o código procurado for maior, descarta-se toda a metade inferior (`inicio = meio + 1`).

### 2.2 Por que a Busca Binária EXIGE dados ordenados?
A propriedade fundamental da Busca Binária é a capacidade de **eliminar metade do espaço de busca em tempo constante $O(1)$**. 
Se os dados não estiverem ordenados monotonicamente por código, não é possível inferir em qual das metades o elemento procurado se encontra ao compará-lo com o elemento central. Caso fosse executada em um vetor desordenado, a busca binária poderia descartar incorretamente a partição onde o elemento reside, resultando em falsos negativos.

> [!IMPORTANT]
> No nosso sistema, antes de executar a busca binária, a função `verificar_ordenado_por_codigo()` audita se o array em RAM está ordenado. Se não estiver, o algoritmo manual `ordenar_insertion()` por código é automaticamente invocado.

### 2.3 Análise de Complexidade (Big-O)
- **Melhor Caso:** $O(1)$ — O elemento procurado coincide exatamente com o ponto médio inicial.
- **Caso Médio:** $O(\log_2 n)$ — A cada comparação, o espaço de busca é reduzido pela metade.
- **Pior Caso:** $O(\log_2 n)$ — O elemento procurado está em uma das extremidades ou não existe no catálogo.
- **Complexidade de Espaço:** $O(1)$ — Implementação iterativa com variáveis de controle de ponteiros de índice.

### 2.4 Exemplo de Rastreamento (Teste de Mesa)
Procurando `codigo = 305` em um vetor de códigos: `[101, 204, 305, 410, 520, 630, 740]` ($n = 7$):
- **Iteração 1:** `inicio = 0`, `fim = 6` $\rightarrow$ `meio = 0 + (6-0)/2 = 3`. Elemento no meio: `vetor[3].codigo = 410`.
  - Como $305 < 410$, descarta-se a metade superior: `fim = 3 - 1 = 2`.
- **Iteração 2:** `inicio = 0`, `fim = 2` $\rightarrow$ `meio = 0 + (2-0)/2 = 1`. Elemento no meio: `vetor[1].codigo = 204`.
  - Como $305 > 204$, descarta-se a metade inferior: `inicio = 1 + 1 = 2`.
- **Iteração 3:** `inicio = 2`, `fim = 2` $\rightarrow$ `meio = 2 + (2-2)/2 = 2`. Elemento no meio: `vetor[2].codigo = 305`.
  - $305 == 305$ $\rightarrow$ **Elemento localizado no índice 2 com apenas 3 comparações!** (Em contraste com a busca linear que exigiria 3 comparações no caso médio e até 7).

---

## 3. Algoritmos Manuais de Ordenação em C

Implementamos os três algoritmos clássicos abordados na ementa da disciplina, sem utilização de bibliotecas prontas (`qsort`):

### 3.1 Insertion Sort (Inserção Direta) - Algoritmo Padrão
- **Funcionamento:** Constrói a lista ordenada elemento por elemento. A cada iteração a partir do índice 1, retira o elemento atual (pivô) e o compara regressivamente com os elementos anteriores, deslocando os maiores para a direita até encontrar a posição correta de inserção do pivô.
- **Vantagens:**
  - Extremamente eficiente para listas pequenas ou que já estejam quase ordenadas.
  - É um algoritmo **estável** (preserva a ordem relativa de elementos com chaves equivalentes).
  - É um algoritmo **in-place** (não consome memória auxiliar extra).
- **Complexidade:**
  - Melhor Caso: $O(n)$ (quando o vetor já está ordenado).
  - Caso Médio: $O(n^2)$.
  - Pior Caso: $O(n^2)$ (quando o vetor está na ordem estritamente inversa).
  - Espaço: $O(1)$.

### 3.2 Bubble Sort com Flag de Otimização
- **Funcionamento:** Percorre sucessivamente o vetor comparando pares adjacentes e efetuando a troca caso estejam fora de ordem.
- **Otimização Aplicada:** Variável booleana `trocou`. Caso uma passagem completa ocorra sem nenhuma troca, o laço é interrompido imediatamente, alcançando $O(n)$ no melhor caso.
- **Complexidade:** Melhor $O(n)$, Médio $O(n^2)$, Pior $O(n^2)$, Espaço $O(1)$.

### 3.3 Selection Sort (Seleção Direta)
- **Funcionamento:** A cada iteração `i`, percorre o restante do array para encontrar o menor elemento e o troca diretamente com o elemento da posição `i`.
- **Características:** Realiza no máximo $n - 1$ trocas de structs em memória, o que minimiza operações de escrita, embora o número de comparações seja invariavelmente $\frac{n(n-1)}{2}$.
- **Complexidade:** Melhor $O(n^2)$, Médio $O(n^2)$, Pior $O(n^2)$, Espaço $O(1)$.

---

## 4. Matriz Bidimensional 2D (Produtos x 12 Meses)

Atendendo ao critério de avaliação **"Matriz ou processamento equivalente" (0,5 pontos)**:
- **Declaração C11:** `int matriz_mov[MAX_PRODUTOS][MESES_ANO];`
- **Dimensões:**
  - Linhas (`i`): Representam cada produto do catálogo em RAM ($0 \le i < total$).
  - Colunas (`m`): Representam os 12 meses do ano civil ($0 \le m < 12$, onde 0 = Janeiro, 11 = Dezembro).
- **Processamento:**
  - A matriz consolida o balanço líquido $(\sum \text{Entradas} - \sum \text{Saídas})$ de cada produto em cada mês.
  - Permite análise gerencial de densidade de movimentação: meses de alta demanda (picos de saída) e meses de reposição (altas entradas).
  - Calcula a soma vetorial por linha (balanço anual do item) e por coluna (balanço consolidado do mês para a empresa inteira).
