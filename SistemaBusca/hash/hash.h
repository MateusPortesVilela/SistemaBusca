/* ============================================================
 * hash.h  —  Tabela Hash com Listas Encadeadas
 *
 * Função hash: djb2 (hash * 33 + c)
 * Colisões:    encadeamento separado (lista ligada por bucket)
 * Tamanho:     10007 (número primo para distribuição uniforme)
 * ============================================================ */

#ifndef HASH_H
#define HASH_H

#define TAM_TABELA   10007   /* Número primo — minimiza colisões */
#define MAX_PAL_HASH   128   /* Deve coincidir com MAX_PALAVRA em leitor.h */

/* ------------------------------------------------------------ */
/*  Nó da lista encadeada em cada bucket                        */
/* ------------------------------------------------------------ */
typedef struct NoHash {
    char          palavra[MAX_PAL_HASH];
    int           ocorrencias;
    struct NoHash *proximo;
} NoHash;

/* ------------------------------------------------------------ */
/*  Tabela Hash                                                  */
/* ------------------------------------------------------------ */
typedef struct {
    NoHash *tabela[TAM_TABELA];
    int     total_unicos;       /* palavras distintas */
    int     total_ocorrencias;  /* soma de todas as ocorrências */
    int     colisoes;           /* colisões ocorridas na inserção */
    int     buckets_usados;     /* buckets com ao menos um elemento */
} TabelaHash;

/* Calcula o índice hash usando algoritmo djb2. */
unsigned int hash_funcao(const char *palavra);

/* Inicializa a tabela (deve ser chamada antes de qualquer uso). */
void hash_inicializar(TabelaHash *t);

/* Insere 'palavra'; se já existir no bucket, incrementa o contador. */
void hash_inserir(TabelaHash *t, const char *palavra);

/* Busca 'palavra'; retorna número de ocorrências (0 = não encontrada). */
int  hash_buscar(const TabelaHash *t, const char *palavra);

/* Lista todas as palavras (percorre todos os buckets). */
void hash_listar(const TabelaHash *t);

/* Libera toda a memória alocada pela tabela. */
void hash_liberar(TabelaHash *t);

#endif /* HASH_H */
