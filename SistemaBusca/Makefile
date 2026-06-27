# ============================================================
# Makefile — Sistema de Busca de Palavras
#
# Gera dois executáveis:
#   busca_avl.exe   — versão com Árvore AVL
#   busca_hash.exe  — versão com Tabela Hash + Listas
#
# Uso:
#   make            -> compila os dois executáveis
#   make busca_avl  -> compila somente a versão AVL
#   make busca_hash -> compila somente a versão Hash
#   make clean      -> remove objetos e executáveis
# ============================================================

CC     = gcc
CFLAGS = -Wall -Wextra -O2 -Icommon -Iavl -Ihash

# ------------------------------------------------------------
# Objetos compartilhados
# ------------------------------------------------------------
COMMON_OBJ = common/exclusao.o common/leitor.o

# Objetos da versão AVL
AVL_OBJ = avl/avl.o avl/main_avl.o

# Objetos da versão Hash
HASH_OBJ = hash/hash.o hash/main_hash.o

# ============================================================
.PHONY: all clean

all: busca_avl.exe busca_hash.exe
	@echo.
	@echo Compilacao concluida com sucesso!
	@echo   busca_avl.exe  - versao Arvore AVL
	@echo   busca_hash.exe - versao Tabela Hash

# ============================================================
# Linkagem final
# ============================================================

busca_avl.exe: $(COMMON_OBJ) $(AVL_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm
	@echo [OK] $@ gerado.

busca_hash.exe: $(COMMON_OBJ) $(HASH_OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo [OK] $@ gerado.

# ============================================================
# Compilação dos objetos — common
# ============================================================

common/exclusao.o: common/exclusao.c common/exclusao.h
	$(CC) $(CFLAGS) -c $< -o $@

common/leitor.o: common/leitor.c common/leitor.h common/exclusao.h
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================
# Compilação dos objetos — AVL
# ============================================================

avl/avl.o: avl/avl.c avl/avl.h
	$(CC) $(CFLAGS) -c $< -o $@

avl/main_avl.o: avl/main_avl.c avl/avl.h common/exclusao.h common/leitor.h
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================
# Compilação dos objetos — Hash
# ============================================================

hash/hash.o: hash/hash.c hash/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

hash/main_hash.o: hash/main_hash.c hash/hash.h common/exclusao.h common/leitor.h
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================
# Limpeza
# ============================================================

clean:
	-del /Q common\*.o avl\*.o hash\*.o busca_avl.exe busca_hash.exe 2>nul
	@echo Limpeza concluida.
