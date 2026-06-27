/* ============================================================
 * avl.h  —  Árvore AVL para indexação de palavras
 *
 * Cada nó armazena a palavra e o número de ocorrências.
 * O balanceamento AVL garante O(log n) para inserção e busca.
 * ============================================================ */

#ifndef AVL_H
#define AVL_H

#define MAX_PAL_AVL  128   /* Deve coincidir com MAX_PALAVRA em leitor.h */

/* ------------------------------------------------------------ */
/*  Nó da árvore AVL                                            */
/* ------------------------------------------------------------ */
typedef struct NoAVL {
    char          palavra[MAX_PAL_AVL];
    int           ocorrencias;
    int           altura;
    struct NoAVL *esq;
    struct NoAVL *dir;
} NoAVL;

/* ------------------------------------------------------------ */
/*  Estrutura principal da árvore                               */
/* ------------------------------------------------------------ */
typedef struct {
    NoAVL *raiz;
    int    total_unicos;       /* palavras distintas */
    int    total_ocorrencias;  /* soma de todas as ocorrências */
} ArvoreAVL;

/* Inicializa a árvore (deve ser chamada antes de qualquer uso). */
void avl_inicializar(ArvoreAVL *t);

/* Insere 'palavra'; se já existir, incrementa o contador. */
void avl_inserir(ArvoreAVL *t, const char *palavra);

/* Busca 'palavra'; retorna número de ocorrências (0 = não encontrada). */
int  avl_buscar(const ArvoreAVL *t, const char *palavra);

/* Lista todas as palavras em ordem alfabética (percurso in-order). */
void avl_listar(const ArvoreAVL *t);

/* Retorna a altura da sub-árvore enraizada em 'no' (0 se NULL). */
int  avl_altura_no(const NoAVL *no);

/* Libera toda a memória alocada pela árvore. */
void avl_liberar(ArvoreAVL *t);

#endif /* AVL_H */
