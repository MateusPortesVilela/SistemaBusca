/* ============================================================
 * leitor.c  —  Leitura e tokenização de arquivos .txt
 *
 * Suporte a codificações:
 *   - ASCII puro
 *   - Latin-1 / Windows-1252 (bytes 0x80-0xFF como caracteres únicos)
 *   - UTF-8 (sequências de 2 bytes 0xC3 xx para o Latin-1 Supplement)
 *
 * Processo por token:
 *   1. fscanf lê string delimitada por espaço
 *   2. leitor_normalizar remove acentos e pontuação, converte para minúsculas
 *   3. Verifica comprimento mínimo (>= 2 letras)
 *   4. Verifica lista de exclusão (busca binária)
 *   5. Chama fn_inserir no repositório
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "leitor.h"

/* ------------------------------------------------------------ */
/*  Mapeamento Latin-1 (byte único >= 0x80) para ASCII         */
/* ------------------------------------------------------------ */
static char latin1_para_ascii(unsigned char c) {
    /* Maiúsculas com acento */
    if (c >= 0xC0 && c <= 0xC5) return 'a'; /* À Á Â Ã Ä Å */
    if (c == 0xC7)               return 'c'; /* Ç             */
    if (c >= 0xC8 && c <= 0xCB) return 'e'; /* È É Ê Ë      */
    if (c >= 0xCC && c <= 0xCF) return 'i'; /* Ì Í Î Ï      */
    if (c == 0xD1)               return 'n'; /* Ñ             */
    if (c >= 0xD2 && c <= 0xD6) return 'o'; /* Ò Ó Ô Õ Ö   */
    if (c == 0xD8)               return 'o'; /* Ø             */
    if (c >= 0xD9 && c <= 0xDC) return 'u'; /* Ù Ú Û Ü      */
    if (c == 0xDD)               return 'y'; /* Ý             */
    /* Minúsculas com acento */
    if (c >= 0xE0 && c <= 0xE5) return 'a'; /* à á â ã ä å  */
    if (c == 0xE7)               return 'c'; /* ç             */
    if (c >= 0xE8 && c <= 0xEB) return 'e'; /* è é ê ë      */
    if (c >= 0xEC && c <= 0xEF) return 'i'; /* ì í î ï      */
    if (c == 0xF1)               return 'n'; /* ñ             */
    if (c >= 0xF2 && c <= 0xF6) return 'o'; /* ò ó ô õ ö   */
    if (c == 0xF8)               return 'o'; /* ø             */
    if (c >= 0xF9 && c <= 0xFC) return 'u'; /* ù ú û ü      */
    if (c == 0xFD || c == 0xFF) return 'y'; /* ý ÿ           */
    return 0; /* byte não mapeado — ignorar */
}

/* ------------------------------------------------------------ */
/*  Mapeamento UTF-8: segundo byte de sequência 0xC3 xx        */
/* ------------------------------------------------------------ */
static char utf8_c3_para_ascii(unsigned char c2) {
    /* Maiúsculas (0x80-0x9F) */
    if (c2 >= 0x80 && c2 <= 0x85) return 'a'; /* À-Å  */
    if (c2 == 0x87)                return 'c'; /* Ç    */
    if (c2 >= 0x88 && c2 <= 0x8B) return 'e'; /* È-Ë  */
    if (c2 >= 0x8C && c2 <= 0x8F) return 'i'; /* Ì-Ï  */
    if (c2 == 0x91)                return 'n'; /* Ñ    */
    if (c2 >= 0x92 && c2 <= 0x96) return 'o'; /* Ò-Ö  */
    if (c2 == 0x98)                return 'o'; /* Ø    */
    if (c2 >= 0x99 && c2 <= 0x9C) return 'u'; /* Ù-Ü  */
    /* Minúsculas (0xA0-0xBF) */
    if (c2 >= 0xA0 && c2 <= 0xA5) return 'a'; /* à-å  */
    if (c2 == 0xA7)                return 'c'; /* ç    */
    if (c2 >= 0xA8 && c2 <= 0xAB) return 'e'; /* è-ë  */
    if (c2 >= 0xAC && c2 <= 0xAF) return 'i'; /* ì-ï  */
    if (c2 == 0xB1)                return 'n'; /* ñ    */
    if (c2 >= 0xB2 && c2 <= 0xB6) return 'o'; /* ò-ö  */
    if (c2 == 0xB8)                return 'o'; /* ø    */
    if (c2 >= 0xB9 && c2 <= 0xBC) return 'u'; /* ù-ü  */
    return 0;
}

/* ------------------------------------------------------------ */
void leitor_normalizar(char *palavra) {
    unsigned char *src = (unsigned char *)palavra;
    int len = (int)strlen(palavra);
    int i = 0, j = 0;

    while (i < len) {
        unsigned char c = src[i];

        if (c < 0x80) {
            /* Byte ASCII puro */
            if (isalpha(c))
                palavra[j++] = (char)tolower(c);
            i++;
        } else if (c == 0xC3 && (i + 1) < len && src[i+1] >= 0x80) {
            /* UTF-8: sequência de 2 bytes 0xC3 + continuação */
            char mapped = utf8_c3_para_ascii(src[i + 1]);
            if (mapped) palavra[j++] = mapped;
            i += 2;
        } else if (c >= 0x80) {
            /* Latin-1 / Windows-1252 ou byte de continuação UTF-8 */
            char mapped = latin1_para_ascii(c);
            if (mapped) palavra[j++] = mapped;
            i++;
        } else {
            i++;
        }
    }
    palavra[j] = '\0';
}

/* ------------------------------------------------------------ */
int leitor_processar(const char *caminho,
                     const ListaExclusao *exclusao,
                     FnInserir fn_inserir,
                     void *repositorio) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("[ERRO] Nao foi possivel abrir: %s\n", caminho);
        return -1;
    }

    char  token[512];      /* buffer grande para tokens brutos */
    char  palavra[MAX_PALAVRA];
    int   count  = 0;
    int   total  = 0;

    while (fscanf(f, "%511s", token) == 1) {
        total++;

        /* Copia com limite de segurança */
        strncpy(palavra, token, MAX_PALAVRA - 1);
        palavra[MAX_PALAVRA - 1] = '\0';

        /* Normaliza: remove acentos, pontuação, converte para minúsculas */
        leitor_normalizar(palavra);

        /* Descarta palavras muito curtas */
        if ((int)strlen(palavra) < 2) continue;

        /* Verifica lista de exclusão (busca binária) */
        if (exclusao_contem(exclusao, palavra)) continue;

        /* Insere no repositório (AVL ou Hash) */
        fn_inserir(repositorio, palavra);
        count++;
    }

    fclose(f);
    printf("  Tokens lidos: %d | Palavras validas inseridas: %d\n", total, count);
    return count;
}
