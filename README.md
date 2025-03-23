# Relatório da Segunda Implementação de Trabalho de POO

Alunos: Gabriel de Albuquerque e João Henrique Valbusa Lima

## Bugs Conhecidos
O programa foi exaustivamente testado e não foi encontrado bugs.
 
## Fazendo os testes e observando a saída
```bash
./test.sh vereadores > teste.log
```
> **O que será compactado e enviado está mo diretorio vereadores**
```bash
cd vereadores/
zip vereadores-JoaoHenrique-GabrielDeAlbuquerque.zip *.cpp  *.hpp Makefile
```

## Disposição do diretório
    .
    ├── README.md
    ├── jpalmeida-progoo-2024-02-trab2.pdf
    ├── vereadores
    │   ├── App.cpp
    │   ├── Candidato.cpp
    │   ├── Candidato.hpp
    │   ├── Eleicao.cpp
    │   ├── Eleicao.hpp
    │   ├── Makefile
    │   ├── Partido.cpp
    │   ├── Partido.hpp
    │   ├── Relatorios.cpp
    │   └── Relatorios.hpp
    ├── test.sh
    └── testes
        ├── AC1392
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        ├── AC1473
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        ├── ES56472
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        ├── ES57053
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        ├── PE25810
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        ├── PE26298
        │   ├── in
        │   │   ├── candidatos.csv
        │   │   ├── cdMunicipio.txt
        │   │   └── votacao.csv
        │   ├── out
        │   │   └── output.txt
        │   └── test.sh
        └── PE26310
            ├── in
            │   ├── candidatos.csv
            │   ├── cdMunicipio.txt
            │   └── votacao.csv
            ├── out
            │   └── output.txt
            └── test.sh