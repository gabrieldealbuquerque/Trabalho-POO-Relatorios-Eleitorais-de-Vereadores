/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

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
#include <cstdint>

using namespace std;

// Função auxiliar para converter string ISO-8859-1 para UTF-8 forneciada pelo professor
string iso_8859_1_to_utf8(string &str)
{
  // adaptado de: https://stackoverflow.com/a/39884120 :-)
  string strOut;
  for (string::iterator it = str.begin(); it != str.end(); ++it)
  {
    uint8_t ch = *it;
    if (ch < 0x80)
    {
      // já está na faixa ASCII (bit mais significativo 0), só copiar para a saída
      strOut.push_back(ch);
    }
    else
    {
      // está na faixa ASCII-estendido, escrever 2 bytes de acordo com UTF-8
      // o primeiro byte codifica os 2 bits mais significativos do byte original (ISO-8859-1)
      strOut.push_back(0b11000000 | (ch >> 6));
      // o segundo byte codifica os 6 bits menos significativos do byte original (ISO-8859-1)
      strOut.push_back(0b10000000 | (ch & 0b00111111));
    }
  }
  return strOut;
}

// Função auxiliar para dividir uma string com base em um delimitador
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Processa o arquivo de consulta (dados dos candidatos)
void processarArquivoConsulta(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo de consulta: " << filename << endl;
        return;
    }
    string linha;
    if (!getline(file, linha))
        return; // lê o cabeçalho
    linha = iso_8859_1_to_utf8(linha);
    vector<string> cabecalho = split(linha, ';');
    map<int, string> camposColunas;
    vector<string> campos = {"SG_UE", "CD_CARGO", "NR_CANDIDATO", "NM_URNA_CANDIDATO",
                                       "NR_PARTIDO", "SG_PARTIDO", "NR_FEDERACAO", "DT_NASCIMENTO",
                                       "CD_GENERO", "CD_SIT_TOT_TURNO"};
    for (size_t i = 0; i < cabecalho.size(); i++) {
        string col = cabecalho[i];
        col.erase(remove(col.begin(), col.end(), '\"'), col.end());
        for (auto &campo : campos) {
            if (col == campo)
                camposColunas[i] = campo;
        }
    }
    while (getline(file, linha)) {
        if (linha.find_first_not_of(" \t\n\r") == string::npos)
            continue;
        linha = iso_8859_1_to_utf8(linha);
        vector<string> valores = split(linha, ';');
        if (valores.size() < camposColunas.size()) {
            cerr << "Linha mal formatada: " << linha << endl;
            continue;
        }
        Candidato* candidato = new Candidato();
        for (size_t i = 0; i < valores.size(); i++) {
            if (camposColunas.find(i) != camposColunas.end()) {
                string campo = camposColunas[i];
                string valor = valores[i];
                // Remove espaços em branco nas extremidades
                valor.erase(0, valor.find_first_not_of(" \t\r\n"));
                valor.erase(valor.find_last_not_of(" \t\r\n") + 1);
                try {
                    candidato->setAtributo(campo, valor);
                } catch (const exception &e) {
                    cerr << e.what() << endl;
                }
            }
        }
        if (Eleicao::instance().criaCandidato(candidato))
        {
            if (candidato->ehEleito()) {
                Eleicao::instance().aumentaVagas(candidato);
            }        
        }
        else {
            delete candidato;
        }
    }
}

// Processa o arquivo de votação
void processarArquivoVotacao(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo de votação: " << filename << endl;
        return;
    }
    string linha;
    if (!getline(file, linha))
        return; // cabeçalho
    linha = iso_8859_1_to_utf8(linha);
    vector<string> cabecalho = split(linha, ';');
    map<int, string> camposColunas;
    vector<string> campos = {"CD_CARGO", "CD_MUNICIPIO", "NR_VOTAVEL", "QT_VOTOS"};
    for (size_t i = 0; i < cabecalho.size(); i++) {
        string col = cabecalho[i];
        col.erase(remove(col.begin(), col.end(), '\"'), col.end());
        for (auto &campo : campos) {
            if (col == campo)
                camposColunas[i] = campo;
        }
    }
    while (getline(file, linha)) {
        if (linha.find_first_not_of(" \t\n\r") == string::npos)
            continue;
        linha = iso_8859_1_to_utf8(linha);
        vector<string> valores = split(linha, ';');
        int cdCargo = 0, cdMunicipio = 0, nrVotavel = 0, qtVotos = 0;
        for (size_t i = 0; i < valores.size(); i++) {
            if (camposColunas.find(i) != camposColunas.end()) {
                string campo = camposColunas[i];
                string valor = valores[i];
                valor.erase(0, valor.find_first_not_of(" \t\r\n"));
                valor.erase(valor.find_last_not_of(" \t\r\n") + 1);
                try {
                    if (campo == "CD_CARGO")
                        cdCargo = stoi(valor);
                    else if (campo == "CD_MUNICIPIO")
                        cdMunicipio = stoi(valor);
                    else if (campo == "NR_VOTAVEL")
                        nrVotavel = stoi(valor);
                    else if (campo == "QT_VOTOS")
                        qtVotos = stoi(valor);
                } catch (const exception &e) {
                    cerr << "Erro ao processar campo: " << e.what() << endl;
                }
            }
        }
        if (cdCargo == 13 && cdMunicipio == Eleicao::instance().getMunicipio()) {
            Eleicao::instance().processaVoto(nrVotavel, qtVotos);
        }
    }
}

// Função para converter string em data (formato dd/MM/yyyy)
static tm parseDate(const string &dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d/%m/%Y");
    if (ss.fail()) {
        throw runtime_error("Erro: Data em formato inválido.");
    }
    return tm;
}

int main(int argc, char* argv[]){
    if (argc < 5) {
        cerr << "Uso: " << argv[0] << " <municipio> <arquivo_consulta> <arquivo_votacao> <data>" << endl;
        return 1;
    }
    try {
        int municipio = stoi(argv[1]);
        Eleicao::instance().setMunicipio(municipio);
        string arquivoConsulta = argv[2];
        string arquivoVotacao = argv[3];
        tm dataEleicao = parseDate(argv[4]);
        Eleicao::instance().setDataEleicao(dataEleicao);
        try {
            // Configura o locale global para pt_BR.UTF-8
            locale::global(locale("pt_BR.UTF-8"));
            cout.imbue(locale()); // Aplica o locale global ao cout
        } catch (const runtime_error& e) {
            cerr << "Locale pt_BR.UTF-8 não disponível. Usando locale padrão." << endl;
        }
        processarArquivoConsulta(arquivoConsulta);
        processarArquivoVotacao(arquivoVotacao);
        
        Relatorios rel;
        rel.geraTodosRelatorios();
        locale::global(locale("C"));
        cout.imbue(locale()); // para evitar do valgrid apontar memory leak
    } catch (const exception &e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }
    return 0;
}
