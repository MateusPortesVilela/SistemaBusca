/* ============================================================
 * main_hash.c  —  Programa principal: versão com Tabela Hash
 *
 * Uso: busca_hash.exe
 *      Os arquivos a carregar são informados interativamente.
 *
 * Dependências:
 *   common/exclusao.h + exclusao.c
 *   common/leitor.h   + leitor.c
 *   hash/hash.h       + hash.c
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "../common/exclusao.h"
#include "../common/leitor.h"

/* ============================================================ */
/*  Wrapper: adapta hash_inserir para a assinatura FnInserir    */
/* ============================================================ */
static void fn_inserir_hash(void *repo, const char *palavra) {
    hash_inserir((TabelaHash *)repo, palavra);
}

/* ============================================================ */
/*  Utilitários de interface                                     */
/* ============================================================ */

static void imprimir_separador(void) {
    printf("------------------------------------------------------------\n");
}

static void imprimir_cabecalho(void) {
    printf("\n");
    printf("============================================================\n");
    printf("    SISTEMA DE BUSCA DE PALAVRAS  |  Versao: Hash + Listas  \n");
    printf("============================================================\n");
}

static void imprimir_menu(void) {
    printf("\n");
    imprimir_separador();
    printf("  MENU PRINCIPAL\n");
    imprimir_separador();
    printf("  1. Carregar arquivo(s) .txt\n");
    printf("  2. Buscar palavra\n");
    printf("  3. Listar todas as palavras\n");
    printf("  4. Exibir estatisticas\n");
    printf("  5. Sair\n");
    imprimir_separador();
    printf("  Opcao: ");
}

/* ============================================================ */
/*  Opção 1 — Carregar arquivo(s)                               */
/* ============================================================ */
static void menu_carregar(TabelaHash *tabela, const ListaExclusao *excl) {
    char caminho[512];

    printf("\n  Digite o caminho do arquivo .txt (ou aperte Enter para voltar):\n");
    printf("  > ");
    if (!fgets(caminho, sizeof(caminho), stdin)) return;

    int len = (int)strlen(caminho);
    while (len > 0 && (caminho[len-1] == '\n' || caminho[len-1] == '\r'))
        caminho[--len] = '\0';

    if (caminho[0] == '\0') return;

    printf("  Carregando: %s\n", caminho);
    int inseridas = leitor_processar(caminho, excl, fn_inserir_hash, tabela);
    if (inseridas >= 0) {
        printf("  Carregamento concluido! Total inserido: %d\n", inseridas);
        printf("  Palavras unicas no repositorio: %d\n", tabela->total_unicos);
    }
}

/* ============================================================ */
/*  Opção 2 — Buscar palavra                                    */
/* ============================================================ */
static void menu_buscar(const TabelaHash *tabela) {
    char palavra[128];

    printf("\n  Digite a palavra a buscar: ");
    if (!fgets(palavra, sizeof(palavra), stdin)) return;
    int len = (int)strlen(palavra);
    while (len > 0 && (palavra[len-1] == '\n' || palavra[len-1] == '\r'))
        palavra[--len] = '\0';

    /* Normaliza a consulta da mesma forma que as palavras indexadas */
    leitor_normalizar(palavra);

    if (strlen(palavra) == 0) {
        printf("  Palavra invalida.\n");
        return;
    }

    int ocorr = hash_buscar(tabela, palavra);
    imprimir_separador();
    if (ocorr > 0)
        printf("  Resultado: \"%s\" encontrada %d vez(es).\n", palavra, ocorr);
    else
        printf("  Resultado: \"%s\" NAO encontrada no repositorio.\n", palavra);
    printf("  (Indice hash: %u)\n", hash_funcao(palavra));
    imprimir_separador();
}

/* ============================================================ */
/*  Opção 3 — Listar palavras                                   */
/* ============================================================ */
static void menu_listar(const TabelaHash *tabela) {
    printf("\n");
    imprimir_separador();
    printf("  PALAVRAS INDEXADAS - total: %d\n", tabela->total_unicos);
    printf("  (Ordem de insercao nos buckets, nao alfabetica)\n");
    imprimir_separador();
    hash_listar(tabela);
    imprimir_separador();
}

/* ============================================================ */
/*  Opção 4 — Estatísticas                                      */
/* ============================================================ */
static void menu_estatisticas(const TabelaHash *tabela) {
    double fator_carga = (double)tabela->total_unicos / (double)TAM_TABELA;
    int    buckets_livres = TAM_TABELA - tabela->buckets_usados;

    printf("\n");
    imprimir_separador();
    printf("  ESTATISTICAS - Tabela Hash com Listas\n");
    imprimir_separador();
    printf("  Palavras unicas         : %d\n",   tabela->total_unicos);
    printf("  Total de ocorrencias    : %d\n",   tabela->total_ocorrencias);
    printf("  Tamanho da tabela       : %d buckets\n", TAM_TABELA);
    printf("  Buckets usados          : %d\n",   tabela->buckets_usados);
    printf("  Buckets livres          : %d\n",   buckets_livres);
    printf("  Colisoes                : %d\n",   tabela->colisoes);
    printf("  Fator de carga (n/m)    : %.4f", fator_carga);
    if (fator_carga < 0.5)
        printf("  [BOM — abaixo de 0.5]\n");
    else if (fator_carga < 0.75)
        printf("  [ACEITAVEL — abaixo de 0.75]\n");
    else
        printf("  [ALTO — considere aumentar TAM_TABELA]\n");
    printf("  Funcao hash             : djb2 (hash * 33 + c)\n");
    imprimir_separador();
}

/* ============================================================ */
/*  main                                                         */
/* ============================================================ */
int main(void) {
    ListaExclusao exclusao;
    TabelaHash    tabela;

    imprimir_cabecalho();

    /* Carrega lista de exclusão */
    printf("\n");
    exclusao_carregar(&exclusao, "dados/exclusao.txt");

    /* Inicializa a tabela hash */
    hash_inicializar(&tabela);

    /* Loop principal do menu */
    int rodando = 1;
    while (rodando) {
        imprimir_menu();

        char entrada[8];
        if (!fgets(entrada, sizeof(entrada), stdin)) break;
        int opcao = atoi(entrada);

        switch (opcao) {
            case 1: menu_carregar(&tabela, &exclusao);    break;
            case 2: menu_buscar(&tabela);                  break;
            case 3: menu_listar(&tabela);                  break;
            case 4: menu_estatisticas(&tabela);            break;
            case 5: rodando = 0;                           break;
            default:
                printf("  Opcao invalida. Tente novamente.\n");
        }
    }

    /* Libera memória */
    hash_liberar(&tabela);
    printf("\n  Encerrando. Memoria liberada. Ate logo!\n\n");
    return 0;
}
