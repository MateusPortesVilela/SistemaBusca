# 🔎 Sistema de Busca de Palavras

> 📚 Projeto de falculdade da matéria de Estutura de Dados, nessamatéria aprendi desde o inicia da Linguagem C, OPERADORES DE ATRIBUIÇÃO SIMPLIFICADA, Sintaxe C e ponteiro de função. Depois comecei os estudos de Listas
> (Fila e Pilha) depois oque foi mais complicados e mais importantes, Vetores e Ponteiros. Após isso Arvores, AVL - CPP.
> Esse projeto foi desenvolvido em **C** para demonstrar técnicas de **indexação textual**, comparando o desempenho entre uma **Árvore AVL** e uma **Tabela Hash com listas encadeadas**.

![C](https://img.shields.io/badge/Linguagem-C-blue)
![Status](https://img.shields.io/badge/Status-Concluído-success)
![Estruturas](https://img.shields.io/badge/Estruturas-AVL%20%7C%20Hash-orange)

---

# 📑 Índice

* 🎯 Sobre o Projeto
* ✨ Funcionalidades
* ⚙️ Fluxo de Funcionamento
* 🚀 Como Compilar e Executar
* 📁 Estrutura do Projeto
* 📚 Explicação dos Arquivos
* 💻 Como Utilizar
* ⚖️ Comparação entre AVL e Hash
* 🏗 Arquitetura
* 🔮 Melhorias Futuras
* 👨‍💻 Conclusão

---

# 🎯 Sobre o Projeto

Este projeto implementa um **sistema de busca e indexação de palavras** utilizando duas estruturas de dados clássicas:

🌳 **Árvore AVL**

⚡ **Tabela Hash com listas encadeadas**

O sistema realiza a leitura de arquivos `.txt`, processa todas as palavras encontradas, elimina palavras irrelevantes (*stopwords*) e cria um índice capaz de responder rapidamente consultas realizadas pelo usuário.

O objetivo principal é comparar diferentes estratégias de armazenamento e busca, permitindo visualizar suas vantagens na prática.

---

# ✨ Funcionalidades

✅ Leitura de arquivos `.txt`

✅ Normalização automática das palavras

* letras minúsculas
* remoção de acentos
* remoção de pontuação

✅ Remoção de *stopwords*

✅ Contagem de ocorrências

✅ Busca rápida de palavras

✅ Listagem completa do índice

✅ Estatísticas da estrutura utilizada

---

# ⚙️ Fluxo de Funcionamento

```text
📄 Arquivo TXT
       │
       ▼
📖 Leitura do arquivo
       │
       ▼
🧹 Normalização
       │
       ▼
🚫 Remoção de Stopwords
       │
       ▼
📥 Inserção na Estrutura
   ├── 🌳 AVL
   └── ⚡ Hash
       │
       ▼
🔎 Consultas e Estatísticas
```

## Etapas

1️⃣ Escolha a versão do sistema.

* 🌳 AVL
* ⚡ Hash

2️⃣ O programa carrega a lista de palavras de exclusão.

3️⃣ O usuário informa um arquivo `.txt`.

4️⃣ O sistema lê palavra por palavra.

5️⃣ Cada palavra é normalizada.

6️⃣ Stopwords são descartadas.

7️⃣ A palavra é inserida na estrutura escolhida.

8️⃣ O usuário pode:

* 🔎 Buscar palavras
* 📋 Listar palavras
* 📊 Visualizar estatísticas

---

# 🚀 Como Compilar e Executar

## 🐧 Linux / macOS

Compile utilizando o **Makefile**:

```bash
make
```

Serão gerados os executáveis:

```text
busca_avl.exe
busca_hash.exe
```

Execute:

```bash
./busca_avl.exe
```

ou

```bash
./busca_hash.exe
```

Para remover arquivos compilados:

```bash
make clean
```

---

## 🪟 Windows

Execute:

```bat
compilar.bat
```

O script localizará automaticamente o compilador GCC e realizará toda a compilação.

---

# 📁 Estrutura do Projeto

```text
📂 SistemaBusca
│
├── 📄 Makefile
├── 📄 compilar.bat
│
├── 📂 avl
│   ├── avl.c
│   ├── avl.h
│   └── main_avl.c
│
├── 📂 hash
│   ├── hash.c
│   ├── hash.h
│   └── main_hash.c
│
├── 📂 common
│   ├── leitor.c
│   ├── leitor.h
│   ├── exclusao.c
│   └── exclusao.h
│
├── 📂 dados
│   ├── exclusao.txt
│   ├── texto_exemplo.txt
│   └── dinossauros.txt
│
└── 📄 README.md
```

---

# 📚 Explicação dos Arquivos

## 📄 Makefile

Automatiza toda a compilação do projeto.

**Responsabilidades:**

* Compilar a versão AVL
* Compilar a versão Hash
* Limpar arquivos temporários

---

## 📄 compilar.bat

Versão da compilação para Windows.

Ideal para quem utiliza **MinGW** ou **GCC** no Windows.

---

# 📂 Pasta `common`

Esta pasta contém tudo o que é compartilhado entre as duas implementações.

### 📖 leitor.c / leitor.h

Responsável por:

* Ler arquivos
* Separar palavras
* Normalizar texto
* Encaminhar palavras para a estrutura escolhida

---

### 🚫 exclusao.c / exclusao.h

Responsável por:

* Carregar stopwords
* Ordenar palavras
* Realizar busca binária
* Filtrar palavras irrelevantes

---

# 🌳 Pasta `avl`

Implementa toda a lógica utilizando uma **Árvore AVL**.

Inclui:

* Inserção
* Busca
* Balanceamento
* Listagem
* Liberação de memória

### Informações armazenadas

* Palavra
* Número de ocorrências
* Altura
* Filhos esquerdo e direito

---

# ⚡ Pasta `hash`

Implementa uma **Tabela Hash** com listas encadeadas.

Possui:

* Função Hash
* Inserção
* Busca
* Tratamento de colisões

Também calcula estatísticas como:

* Número de colisões
* Buckets utilizados
* Fator de carga
* Total de palavras

---

# 📂 Pasta `dados`

Contém todos os arquivos utilizados pelo programa.

### 📄 exclusao.txt

Lista de stopwords.

Exemplo:

```text
de
da
do
para
com
```

---

### 📄 texto_exemplo.txt

Arquivo simples para testes rápidos.

---

### 🦖 dinossauros.txt

Texto maior utilizado para testar:

* remoção de acentos
* pontuação
* desempenho
* contagem de palavras

---

# 💻 Como Utilizar

### 1️⃣ Compile o projeto

```bash
make
```

---

### 2️⃣ Execute uma das versões

```bash
./busca_avl.exe
```

ou

```bash
./busca_hash.exe
```

---

### 3️⃣ Escolha um arquivo

Exemplo:

```text
dados/texto_exemplo.txt
```

ou

```text
dados/dinossauros.txt
```

---

### 4️⃣ Utilize o menu

Você poderá:

🔎 Buscar palavras

📋 Listar palavras

📊 Visualizar estatísticas

❌ Encerrar o programa

---

# ⚖️ Comparação entre AVL e Hash

| Característica |       🌳 AVL      |           ⚡ Hash          |
| :------------- | :---------------: | :-----------------------: |
| Estrutura      | Árvore Balanceada |        Tabela Hash        |
| Busca          |    Muito rápida   |    Muito rápida (média)   |
| Inserção       |     Balanceada    |        Muito rápida       |
| Ordenação      |    ✅ Alfabética   |       ❌ Não ordenada      |
| Colisões       |     Não possui    |           Possui          |
| Estatísticas   |  Altura da árvore | Colisões e fator de carga |

---

# 🏗 Arquitetura

```text
                 📄 Arquivo TXT
                        │
                        ▼
                📖 Leitor (common)
                        │
         ┌──────────────┴──────────────┐
         ▼                             ▼
 🚫 Stopwords                  🧹 Normalização
         │                             │
         └──────────────┬──────────────┘
                        ▼
             📥 Estrutura de Dados
                 ├── 🌳 AVL
                 └── ⚡ Hash
                        │
                        ▼
          🔎 Busca • 📋 Listagem • 📊 Estatísticas
```

---

# 🔮 Melhorias Futuras

* 🔍 Busca por prefixos
* 📄 Indexação de múltiplos arquivos
* 📊 Relatórios estatísticos
* 📈 Comparação automática de desempenho
* 🧵 Processamento paralelo
* 💾 Persistência em banco de dados
* 🌐 Interface gráfica

---

# 👨‍💻 Conclusão

Este projeto demonstra, de forma prática, como diferentes **estruturas de dados** influenciam o desempenho de um sistema de busca textual.

Enquanto a **🌳 Árvore AVL** oferece organização ordenada e balanceada, a **⚡ Tabela Hash** prioriza velocidade média de acesso, permitindo comparar duas abordagens clássicas de indexação e recuperação de informações.

Além de servir como um estudo sobre **Estruturas de Dados em C**, o projeto foi desenvolvido de forma modular, facilitando futuras expansões e a manutenção do código.

---

## ⭐ Se este projeto foi útil

Se este repositório ajudou nos seus estudos, considere deixar uma **⭐ no GitHub**. Isso incentiva o desenvolvimento de novos projetos e facilita que outras pessoas encontrem este material.

**Bons estudos e boa programação! 🚀**
