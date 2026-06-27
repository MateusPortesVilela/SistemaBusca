@echo off
REM ============================================================
REM compilar.bat — Compila o Sistema de Busca de Palavras
REM
REM Detecta automaticamente o GCC em locais comuns.
REM Execute este arquivo na pasta raiz do projeto.
REM ============================================================

setlocal

REM --- Tenta detectar GCC no PATH ---
where gcc >nul 2>&1
if %ERRORLEVEL% == 0 (
    set GCC=gcc
    set MAKE_CMD=mingw32-make
    goto :found
)

REM --- Tenta locais comuns do MinGW / MSYS2 ---
set CANDIDATES=^
    "C:\MinGW\bin\gcc.exe" ^
    "C:\mingw64\bin\gcc.exe" ^
    "C:\msys64\mingw64\bin\gcc.exe" ^
    "C:\msys64\usr\bin\gcc.exe" ^
    "C:\TDM-GCC-64\bin\gcc.exe" ^
    "C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe"

for %%G in (%CANDIDATES%) do (
    if exist %%G (
        set GCC=%%G
        goto :found
    )
)

REM --- GCC nao encontrado ---
echo.
echo [ERRO] GCC nao encontrado no sistema.
echo.
echo Para instalar o GCC no Windows, escolha uma opcao:
echo.
echo  1. Scoop (recomendado):
echo     Abra o PowerShell e execute:
echo     Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
echo     irm get.scoop.sh ^| iex
echo     scoop install gcc
echo.
echo  2. MinGW-w64 direto:
echo     https://github.com/niXman/mingw-builds-binaries/releases
echo     Baixe a versao x86_64-*-release-*-seh-*.7z
echo     Extraia em C:\mingw64
echo     Adicione C:\mingw64\bin ao PATH do sistema
echo.
echo  3. Code::Blocks com MinGW:
echo     https://www.codeblocks.org/downloads/
echo     Escolha o instalador "codeblocks-XX.XXmingw-setup.exe"
echo.
pause
exit /b 1

:found
echo [OK] GCC encontrado: %GCC%
echo.

REM --- Define diretorio do GCC para o Makefile ---
for %%F in (%GCC%) do set GCC_DIR=%%~dpF

REM --- Compila ---
echo Compilando versao AVL...
%GCC% -Wall -Wextra -O2 -Icommon -Iavl ^
    common\exclusao.c ^
    common\leitor.c ^
    avl\avl.c ^
    avl\main_avl.c ^
    -o busca_avl.exe -lm

if %ERRORLEVEL% neq 0 (
    echo [ERRO] Falha ao compilar busca_avl.exe
    pause
    exit /b 1
)
echo [OK] busca_avl.exe gerado.

echo.
echo Compilando versao Hash...
%GCC% -Wall -Wextra -O2 -Icommon -Ihash ^
    common\exclusao.c ^
    common\leitor.c ^
    hash\hash.c ^
    hash\main_hash.c ^
    -o busca_hash.exe

if %ERRORLEVEL% neq 0 (
    echo [ERRO] Falha ao compilar busca_hash.exe
    pause
    exit /b 1
)
echo [OK] busca_hash.exe gerado.

echo.
echo ============================================================
echo  Compilacao concluida com sucesso!
echo  - busca_avl.exe   (versao Arvore AVL)
echo  - busca_hash.exe  (versao Tabela Hash)
echo ============================================================
echo.
echo Para executar:
echo   busca_avl.exe
echo   busca_hash.exe
echo.
pause
endlocal
