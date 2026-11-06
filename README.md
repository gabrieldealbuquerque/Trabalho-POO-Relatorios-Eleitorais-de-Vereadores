# Relatórios eleitorais de vereadores — C++17

Aplicação de linha de comando que lê arquivos CSV de candidatos e votação, seleciona um município e produz relatórios sobre a eleição de vereadores. Projeto acadêmico da disciplina de Programação Orientada a Objetos, desenvolvido por **Gabriel de Albuquerque e João Henrique Valbusa Lima**.

O foco é modelagem orientada a objetos e processamento de dados eleitorais com a biblioteca padrão C++. O [enunciado original](jpalmeida-progoo-2024-02-trab2.pdf) está preservado no repositório.

## O que o programa faz

A execução produz dez relatórios: número de vagas; vereadores eleitos; candidatos mais votados; comparação do resultado proporcional com um ranking majoritário; votação e eleitos por partido; primeiro e último colocados de cada partido; distribuição dos eleitos por idade e gênero; e totais de votos nominais e de legenda.

Os eleitos são identificados pela situação de totalização presente no CSV de candidatos. O programa analisa esses resultados e não implementa uma nova apuração ou distribuição oficial de cadeiras.

## Estrutura e modelagem

| Arquivo | Responsabilidade |
| --- | --- |
| [main.cpp](vereadores/main.cpp) | Argumentos da CLI, leitura dos CSVs e início da geração dos relatórios. |
| [Candidato.hpp](vereadores/Candidato.hpp) e [Candidato.cpp](vereadores/Candidato.cpp) | Dados cadastrais, situação eleitoral e votos do candidato. |
| [Partido.hpp](vereadores/Partido.hpp) e [Partido.cpp](vereadores/Partido.cpp) | Agrupamento de candidatos e votos de legenda. |
| [Eleicao.hpp](vereadores/Eleicao.hpp) e [Eleicao.cpp](vereadores/Eleicao.cpp) | Contexto da eleição em singleton, filtro por município e associação entre votos, candidatos e partidos. |
| [Relatorios.hpp](vereadores/Relatorios.hpp) e [Relatorios.cpp](vereadores/Relatorios.cpp) | Agregações, ordenação e formatação dos dez relatórios. |
| [Makefile](vereadores/Makefile) | Compilação com G++, C++17 e avisos `-Wall`. |
| [test.sh](test.sh) | Script de avaliação acadêmica que depende de casos externos. |

Fluxo: `CSV de candidatos + CSV de votos → Eleicao → Candidato/Partido → Relatorios → terminal`. A implementação usa `map`, `set`, `vector` e comparadores para organizar os dados e ordenar os resultados.

## Compilar e executar

Requisitos: Linux ou WSL, G++ com suporte a C++17 e GNU Make. Não são necessárias bibliotecas externas.

Na raiz do repositório:

```bash
make -B -C vereadores
```

O parâmetro `-B` força a recompilação dos fontes, pois o histórico inclui arquivos objeto `.o`. O executável gerado é `vereadores/vereadores`.

Com arquivos compatíveis disponíveis localmente:

```bash
./vereadores/vereadores 57053 /caminho/candidatos.csv /caminho/votacao.csv 06/10/2024
```

| Argumento | Formato |
| --- | --- |
| Município | Código numérico usado nos arquivos de entrada. |
| Candidatos | Caminho do CSV de consulta de candidatos. |
| Votação | Caminho do CSV de votação. |
| Data da eleição | `DD/MM/AAAA`, usada no cálculo de idade. |

Os caminhos do exemplo devem ser substituídos pelos arquivos locais. Para salvar a saída, acrescente `> relatorios.txt`. O programa tenta usar o locale `pt_BR.UTF-8` e avisa quando precisa recorrer ao locale padrão.

### Contrato dos arquivos

Os arquivos devem ter cabeçalho e campos separados por `;`. O parser reconhece os nomes das colunas, removendo aspas nos campos tratados:

- Candidatos: `SG_UE`, `CD_CARGO`, `NR_CANDIDATO`, `NM_URNA_CANDIDATO`, `NR_PARTIDO`, `SG_PARTIDO`, `NR_FEDERACAO`, `DT_NASCIMENTO`, `CD_GENERO` e `CD_SIT_TOT_TURNO`.
- Votação: `CD_CARGO`, `CD_MUNICIPIO`, `NR_VOTAVEL` e `QT_VOTOS`.

São considerados os registros do município informado e do cargo `13` (vereador). O parser separa a linha diretamente por ponto e vírgula; não é um leitor CSV genérico para delimitadores dentro de campos entre aspas. Há uma função fornecida pelo professor para conversão ISO-8859-1/UTF-8, aplicada aos cabeçalhos; a conversão das linhas de dados está comentada no código original.

## Avaliação acadêmica e limitações

O script original pode ser chamado com `bash test.sh vereadores`, mas precisa do diretório `testes/`, não incluído neste snapshot, e do Valgrind. Ele grava a saída em `test_output.log` e trabalha em `/tmp/progoo-tests-<usuario>`. A ausência dos casos impede reproduzir a avaliação completa somente com os arquivos versionados.

O alvo `make clean` também remove arquivos `*.csv` e `*.txt` do diretório `vereadores`; mantenha entradas e resultados fora dele ou use uma cópia de trabalho.

Na revisão desta documentação, os fontes compilaram com o Makefile em uma cópia limpa. Uma execução com CSVs sintéticos mínimos emitiu os dez relatórios, mas terminou com falha de segmentação; o `main` contém uma chamada manual ao destrutor do singleton `Eleicao`. Também existe divergência entre o mapeamento de `CD_GENERO` em `Candidato.cpp` e os contadores do relatório por gênero. Esses comportamentos do código original foram preservados e exigem correção/conferência antes de uso além do estudo acadêmico.

A compilação não garante a exatidão dos relatórios. Os dados de entrada e os resultados devem ser conferidos com o enunciado; este projeto não é uma ferramenta oficial de apuração eleitoral.

## Autoria e referências

- **Gabriel de Albuquerque e João Henrique Valbusa Lima** — implementação acadêmica em dupla.
- **Prof. João Paulo A. Almeida** — enunciado e Makefile da disciplina, conforme os materiais originais.
- A função de conversão de caracteres em `main.cpp` mantém a atribuição original ao professor e à [referência de adaptação](https://stackoverflow.com/a/39884120).

O enunciado, os comentários de autoria e os materiais originais permanecem preservados.
