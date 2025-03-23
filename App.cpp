#include "Eleicao.hpp"
#include "Candidato.hpp"
#include "Relatorios.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <iomanip>

// Função auxiliar para dividir uma string com base em um delimitador
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Processa o arquivo de consulta (dados dos candidatos)
void processarArquivoConsulta(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo de consulta: " << filename << std::endl;
        return;
    }
    std::string linha;
    if (!std::getline(file, linha))
        return; // lê o cabeçalho
    std::vector<std::string> cabecalho = split(linha, ';');
    std::map<int, std::string> camposColunas;
    std::vector<std::string> campos = {"SG_UE", "CD_CARGO", "NR_CANDIDATO", "NM_URNA_CANDIDATO",
                                       "NR_PARTIDO", "SG_PARTIDO", "NR_FEDERACAO", "DT_NASCIMENTO",
                                       "CD_GENERO", "CD_SIT_TOT_TURNO"};
    for (size_t i = 0; i < cabecalho.size(); i++) {
        std::string col = cabecalho[i];
        col.erase(std::remove(col.begin(), col.end(), '\"'), col.end());
        for (auto &campo : campos) {
            if (col == campo)
                camposColunas[i] = campo;
        }
    }
    while (std::getline(file, linha)) {
        if (linha.find_first_not_of(" \t\n\r") == std::string::npos)
            continue;
        std::vector<std::string> valores = split(linha, ';');
        if (valores.size() < camposColunas.size()) {
            std::cerr << "Linha mal formatada: " << linha << std::endl;
            continue;
        }
        Candidato* candidato = new Candidato();
        for (size_t i = 0; i < valores.size(); i++) {
            if (camposColunas.find(i) != camposColunas.end()) {
                std::string campo = camposColunas[i];
                std::string valor = valores[i];
                // Remove espaços em branco nas extremidades
                valor.erase(0, valor.find_first_not_of(" \t\r\n"));
                valor.erase(valor.find_last_not_of(" \t\r\n") + 1);
                try {
                    candidato->setAtributo(campo, valor);
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
        Eleicao::instance().criaCandidato(candidato);
        if (candidato->ehEleito()) {
            Eleicao::instance().aumentaVagas(candidato);
        }
    }
}

// Processa o arquivo de votação
void processarArquivoVotacao(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo de votação: " << filename << std::endl;
        return;
    }
    std::string linha;
    if (!std::getline(file, linha))
        return; // cabeçalho
    std::vector<std::string> cabecalho = split(linha, ';');
    std::map<int, std::string> camposColunas;
    std::vector<std::string> campos = {"CD_CARGO", "CD_MUNICIPIO", "NR_VOTAVEL", "QT_VOTOS"};
    for (size_t i = 0; i < cabecalho.size(); i++) {
        std::string col = cabecalho[i];
        col.erase(std::remove(col.begin(), col.end(), '\"'), col.end());
        for (auto &campo : campos) {
            if (col == campo)
                camposColunas[i] = campo;
        }
    }
    while (std::getline(file, linha)) {
        if (linha.find_first_not_of(" \t\n\r") == std::string::npos)
            continue;
        std::vector<std::string> valores = split(linha, ';');
        int cdCargo = 0, cdMunicipio = 0, nrVotavel = 0, qtVotos = 0;
        for (size_t i = 0; i < valores.size(); i++) {
            if (camposColunas.find(i) != camposColunas.end()) {
                std::string campo = camposColunas[i];
                std::string valor = valores[i];
                valor.erase(0, valor.find_first_not_of(" \t\r\n"));
                valor.erase(valor.find_last_not_of(" \t\r\n") + 1);
                try {
                    if (campo == "CD_CARGO")
                        cdCargo = std::stoi(valor);
                    else if (campo == "CD_MUNICIPIO")
                        cdMunicipio = std::stoi(valor);
                    else if (campo == "NR_VOTAVEL")
                        nrVotavel = std::stoi(valor);
                    else if (campo == "QT_VOTOS")
                        qtVotos = std::stoi(valor);
                } catch (const std::exception &e) {
                    std::cerr << "Erro ao processar campo: " << e.what() << std::endl;
                }
            }
        }
        if (cdCargo == 13 && cdMunicipio == Eleicao::instance().getMunicipio()) {
            Eleicao::instance().processaVoto(nrVotavel, qtVotos);
        }
    }
}

// Função para converter string em data (formato dd/MM/yyyy)
static std::tm parseDate(const std::string &dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%d/%m/%Y");
    if (ss.fail()) {
        throw std::runtime_error("Erro: Data em formato inválido.");
    }
    return tm;
}

int main(int argc, char* argv[]){
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <municipio> <arquivo_consulta> <arquivo_votacao> <data>" << std::endl;
        return 1;
    }
    try {
        int municipio = std::stoi(argv[1]);
        Eleicao::instance().setMunicipio(municipio);
        std::string arquivoConsulta = argv[2];
        std::string arquivoVotacao = argv[3];
        std::tm dataEleicao = parseDate(argv[4]);
        Eleicao::instance().setDataEleicao(dataEleicao);
        
        processarArquivoConsulta(arquivoConsulta);
        processarArquivoVotacao(arquivoVotacao);
        
        Relatorios rel;
        rel.geraTodosRelatorios();
    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
