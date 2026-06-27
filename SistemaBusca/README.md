# Sistema de Busca de Palavras

Este projeto implementa um sistema simples de indexação e consulta de palavras em arquivos de texto, utilizando duas abordagens diferentes de estrutura de dados:

- Árvore AVL
- Tabela Hash com listas encadeadas

A ideia principal é ler um arquivo `.txt`, normalizar as palavras, ignorar stopwords (palavras de exclusão) e inserir as palavras válidas em um repositório indexado. Em seguida, o usuário pode buscar palavras, listar o conteúdo indexado e visualizar estatísticas sobre o repositório.

---

## 1. O que o projeto faz

O sistema funciona como um pequeno mecanismo de indexação textual. Ele recebe um arquivo com texto, processa cada palavra e armazena informações como:

- a palavra em si;
- quantas vezes ela apareceu;
- métricas de desempenho da estrutura utilizada.

Isso permite responder perguntas como:

- a palavra X aparece no texto?
- quantas vezes a palavra X aparece?
- quais palavras foram indexadas?

O projeto foi construído para demonstrar o uso prático de estruturas de dados em C, comparando duas formas diferentes de organização de dados para busca e armazenamento.

---

## 2. Como funciona o fluxo do sistema

O processo ocorre em etapas bem definidas:

1. O usuário escolhe uma das versões do sistema:
   - versão AVL;
   - versão Hash.

2. O programa carrega a lista de palavras de exclusão a partir de um arquivo de texto.

3. O usuário informa um arquivo `.txt` que será processado.

4. O sistema lê o arquivo palavra por palavra.

5. Cada token é normalizado:
   - passa a ficar em minúsculas;
   - acentos são removidos;
   - pontuação e símbolos são descartados.

6. Palavras muito curtas e stopwords são ignoradas.

7. A palavra válida é inserida no repositório escolhido:
   - em uma árvore AVL, se a versão for AVL;
   - em uma tabela hash, se a versão for Hash.

8. O usuário pode então:
   - buscar uma palavra;
   - listar o conteúdo indexado;
   - ver estatísticas relacionadas à estrutura.

---

## 3. Como compilar e executar

### Opção 1: usando o Makefile (Linux/macOS)

Na pasta raiz do projeto, execute:

```bash
make
```

Isso gera dois executáveis:

- `busca_avl.exe`
- `busca_hash.exe`

Para executar:

```bash
./busca_avl.exe
```

ou

```bash
./busca_hash.exe
```

Para limpar os arquivos gerados:

```bash
make clean
```

### Opção 2: usando o arquivo de compilação do Windows

No Windows, pode-se usar o arquivo `compilar.bat`:

```bat
compilar.bat
```

Ele tenta localizar o GCC e compila as duas versões automaticamente.

---

## 4. Estrutura do projeto

```text
SistemaBusca/
├── Makefile
├── compilar.bat
├── avl/
├── common/
├── dados/
├── hash/
└── executáveis gerados
```

### Principais pastas

- `avl/`: implementação da versão baseada em árvore AVL.
- `hash/`: implementação da versão baseada em tabela hash.
- `common/`: código compartilhado entre as duas versões.
- `dados/`: arquivos de entrada e de configuração.

---

## 5. Explicação dos arquivos e como eles se complementam

### Arquivo raiz

#### `Makefile`
Responsável por compilar o projeto automaticamente. Define quais arquivos devem ser compilados, gera os executáveis e também oferece a rotina de limpeza.

Ele atua como um ponto central de montagem do sistema, organizando a compilação das duas versões do programa.

#### `compilar.bat`
Versão para Windows do processo de compilação. Ele procura por um compilador GCC e executa a compilação manualmente, facilitando o uso em ambientes Windows.

---

### Pasta `common`

A pasta `common` contém a lógica compartilhada pelas duas versões do sistema. Ela é o núcleo de integração entre leitura de arquivos, normalização de palavras e filtragem de stopwords.

#### `common/leitor.h`
Define a interface do módulo de leitura. Declara funções para normalizar palavras e processar arquivos de texto.

#### `common/leitor.c`
Implementa a leitura do arquivo, a tokenização das palavras, a normalização e a chamada de inserção no repositório. É o módulo que conecta o conteúdo textual ao mecanismo de indexação.

#### `common/exclusao.h`
Define a estrutura de dados usada para armazenar a lista de palavras de exclusão e a API para carregá-las e consultá-las.

#### `common/exclusao.c`
Implementa a leitura do arquivo de stopwords, a ordenação das palavras e a busca binária para verificar se uma palavra deve ser ignorada. Esse módulo melhora a qualidade do índice, removendo palavras irrelevantes como artigos, preposições e pronomes.

Esses arquivos se complementam porque o leitor depende da lista de exclusão para decidir se uma palavra deve ser indexada.

---

### Pasta `avl`

A pasta `avl` implementa a versão do sistema que usa uma árvore AVL para armazenar as palavras.

#### `avl/avl.h`
Define a estrutura da árvore AVL, com nós contendo:

- palavra;
- quantidade de ocorrências;
- altura do nó;
- ponteiros para subárvores esquerda e direita.

#### `avl/avl.c`
Contém a implementação das operações da árvore AVL:

- inserção;
- balanceamento;
- busca;
- listagem;
- liberação de memória.

A árvore AVL garante que a estrutura permaneça balanceada, o que torna as operações de busca e inserção eficientes.

#### `avl/main_avl.c`
É o programa principal da versão AVL. Ele oferece um menu interativo com opções para:

- carregar arquivos;
- buscar palavras;
- listar palavras indexadas;
- mostrar estatísticas da árvore.

Essa versão é útil para demonstrar uma solução baseada em árvore balanceada.

---

### Pasta `hash`

A pasta `hash` implementa a versão do sistema que usa uma tabela hash com listas encadeadas.

#### `hash/hash.h`
Define a tabela hash, os nós das listas e os campos de estatística, como:

- total de palavras únicas;
- total de ocorrências;
- número de colisões;
- quantidade de buckets usados.

#### `hash/hash.c`
Implementa a função hash, a inserção de palavras e a busca pelas palavras armazenadas. As colisões são tratadas por meio de listas encadeadas, o que torna essa estrutura prática e eficiente para grandes volumes de dados.

#### `hash/main_hash.c`
É o programa principal da versão hash. Ele oferece um menu interativo semelhante ao da versão AVL, porém exibindo estatísticas específicas de uma tabela hash, como fator de carga e colisões.

Essa versão é útil para comparar com a abordagem AVL e observar como diferentes estruturas influenciam o desempenho.

---

### Pasta `dados`

A pasta `dados` contém os arquivos de entrada e configuração do projeto.

#### `dados/exclusao.txt`
Arquivo com a lista de stopwords. Cada palavra é lida, normalizada e usada para filtrar o conteúdo indexado.

#### `dados/texto_exemplo.txt`
Exemplo de texto para teste. Pode ser usado para verificar o funcionamento do sistema com um conteúdo de exemplo mais genérico.

#### `dados/dinossauros.txt`
Arquivo com um texto temático sobre dinossauros. Ele é um bom exemplo para observar como o sistema processa palavras com acentos, pontuação e frases longas.

Esses arquivos servem como base de testes e mostram como o sistema pode trabalhar com diferentes tipos de conteúdo textual.

---

## 6. Como usar o sistema na prática

1. Compile o projeto.
2. Execute uma das versões:
   - `./busca_avl.exe`
   - `./busca_hash.exe`
3. No menu, escolha a opção para carregar um arquivo `.txt`.
4. Informe um caminho como:

```text
dados/texto_exemplo.txt
```

ou

```text
dados/dinossauros.txt
```

5. Depois, use as opções de busca e listagem.

---

## 7. Diferenças entre as duas versões

| Característica | AVL | Hash |
|---|---|---|
| Estrutura | Árvore balanceada | Tabela hash com listas |
| Ordem de listagem | Alfabética | Pela ordem dos buckets |
| Busca | Mais previsível e ordenada | Muito rápida em média |
| Estatísticas | Altura da árvore | Colisões e fator de carga |

A escolha entre as duas versões depende do objetivo do projeto: a AVL enfatiza organização ordenada e estrutura balanceada, enquanto a hash prioriza rapidez média de acesso e simples implementação de indexação.

---

## 8. Resumo arquitetural

O projeto é organizado de forma modular:

- a camada `common` cuida da leitura e filtragem;
- a camada `avl` implementa uma solução baseada em árvore balanceada;
- a camada `hash` implementa uma solução baseada em tabela hash;
- os arquivos em `dados` fornecem conteúdo e listas de palavras de exclusão.

Essa separação torna o sistema mais fácil de compreender, manter e expandir.

---

## 9. Conclusão

Este projeto é uma boa introdução prática ao estudo de estruturas de dados em C. Ele mostra como um sistema simples de busca pode ser montado a partir de componentes bem separados: leitura de arquivos, tratamento de texto, indexação e interface interativa.

Além disso, ele permite comparar duas abordagens clássicas para indexação e recuperação de informação:

- a árvore AVL, que oferece organização ordenada e balanceada;
- a tabela hash, que oferece acesso rápido e eficiente em média.
