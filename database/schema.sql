-- ============================================================================
-- PROJETO INTEGRADOR: ALGORITMOS E PROGRAMAÇÃO II (2026)
-- BANCO DE DADOS: SQLite3
-- DOMÍNIO: Sistema de Controle de Estoque
-- ============================================================================

PRAGMA foreign_keys = ON;

-- Tabela de Produtos (Entidade Principal)
CREATE TABLE IF NOT EXISTS produtos (
    codigo INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    descricao TEXT,
    categoria TEXT NOT NULL,
    preco REAL NOT NULL CHECK(preco >= 0.0),
    quantidade INTEGER NOT NULL CHECK(quantidade >= 0),
    estoque_minimo INTEGER NOT NULL CHECK(estoque_minimo >= 0)
);

-- Tabela de Movimentações (Entidade Histórica de Entradas e Saídas)
CREATE TABLE IF NOT EXISTS movimentacoes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    codigo_produto INTEGER NOT NULL,
    tipo INTEGER NOT NULL CHECK(tipo IN (1, 2)), -- 1 = ENTRADA, 2 = SAÍDA
    quantidade INTEGER NOT NULL CHECK(quantidade > 0),
    data_hora TEXT NOT NULL,                     -- Formato ISO: YYYY-MM-DD HH:MM:SS
    observacao TEXT,
    FOREIGN KEY (codigo_produto) REFERENCES produtos(codigo) ON DELETE CASCADE
);

-- Índices para otimização de leitura e integridade referencial
CREATE INDEX IF NOT EXISTS idx_produtos_nome ON produtos(nome);
CREATE INDEX IF NOT EXISTS idx_mov_produto ON movimentacoes(codigo_produto);
CREATE INDEX IF NOT EXISTS idx_mov_data ON movimentacoes(data_hora);
