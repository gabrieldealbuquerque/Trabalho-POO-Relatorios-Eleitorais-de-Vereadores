#include "Candidato.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstring>
#include <algorithm>

using namespace std;

// Construtor: inicializa os atributos padrão
// A ordem aqui segue exatamente a ordem de declaração no header
Candidato::Candidato()
    : SG_UE(0),
      CD_CARGO(0),
      NR_CANDIDATO(""),
      NM_URNA_CANDIDATO(""),
      NR_PARTIDO(0),
      SG_PARTIDO(""),
      NR_FEDERACAO(-1),
      DT_NASCIMENTO{},
      CD_GENERO(false),
      situacaoTurno(CD_SIT_TOT_TURNO::NAO_ELEITO),
      votos(0)
{
    // Se preferir garantir que a data esteja zerada:
    memset(&DT_NASCIMENTO, 0, sizeof(DT_NASCIMENTO));
}

int Candidato::getSG_UE() const { return SG_UE; }
int Candidato::getCD_CARGO() const { return CD_CARGO; }
const string& Candidato::getNR_CANDIDATO() const { return NR_CANDIDATO; }
const string& Candidato::getNM_URNA_CANDIDATO() const { return NM_URNA_CANDIDATO; }
int Candidato::getNR_PARTIDO() const { return NR_PARTIDO; }
const string& Candidato::getSG_PARTIDO() const { return SG_PARTIDO; }
int Candidato::getNR_FEDERACAO() const { return NR_FEDERACAO; }
tm Candidato::getDT_NASCIMENTO() const { return DT_NASCIMENTO; }
bool Candidato::getCD_GENERO() const { return CD_GENERO; }
CD_SIT_TOT_TURNO Candidato::getSituacaoTurno() const { return situacaoTurno; }
int Candidato::getVotos() const { return votos; }

void Candidato::increaseVotos(int v) {
    votos += v;
}

// Função auxiliar para converter string para tm (formato dd/MM/yyyy)
static tm parseDate(const string &dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d/%m/%Y");
    if (ss.fail()) {
        throw runtime_error("Falha ao converter data: " + dateStr);
    }
    return tm;
}

void Candidato::setAtributo(const string &campo, const string &valor) {
    if (valor.empty() || all_of(valor.begin(), valor.end(), [](char c) { return isspace(c); })) {
        return; // Ignora valores nulos ou vazios
    }

    // Remove aspas
    string valorLimpo = valor;
    valorLimpo.erase(remove(valorLimpo.begin(), valorLimpo.end(), '\"'), valorLimpo.end());

    try {
        if (campo == "SG_UE") {
            SG_UE = stoi(valorLimpo);
        }
        else if (campo == "CD_CARGO") {
            CD_CARGO = stoi(valorLimpo);
        }
        else if (campo == "NR_CANDIDATO") {
            NR_CANDIDATO = valorLimpo;
        }
        else if (campo == "NM_URNA_CANDIDATO") {
            NM_URNA_CANDIDATO = valorLimpo;
        }
        else if (campo == "NR_PARTIDO") {
            NR_PARTIDO = stoi(valorLimpo);
        }
        else if (campo == "SG_PARTIDO") {
            SG_PARTIDO = valorLimpo;
        }
        else if (campo == "NR_FEDERACAO") {
            NR_FEDERACAO = stoi(valorLimpo);
        }
        else if (campo == "DT_NASCIMENTO") {
            // Formato esperado: "DD/MM/AAAA"
            if (valorLimpo.length() == 10 && valorLimpo[2] == '/' && valorLimpo[5] == '/') {
                int dia = stoi(valorLimpo.substr(0, 2));
                int mes = stoi(valorLimpo.substr(3, 2));
                int ano = stoi(valorLimpo.substr(6, 4));
                
                // Preencher a estrutura tm
                memset(&DT_NASCIMENTO, 0, sizeof(tm));
                DT_NASCIMENTO.tm_mday = dia;
                DT_NASCIMENTO.tm_mon = mes - 1;  // tm_mon é 0-11
                DT_NASCIMENTO.tm_year = ano - 1900;  // tm_year é anos desde 1900
            } else {
                throw invalid_argument("Formato de data inválido: " + valorLimpo);
            }
        }
        else if (campo == "CD_GENERO") {
            CD_GENERO = (valorLimpo == "2"); // 2 = Feminino (true), outros = Masculino (false)
        }
        else if (campo == "CD_SIT_TOT_TURNO") {
            if (valorLimpo == "2" || valorLimpo == "3")
                situacaoTurno = CD_SIT_TOT_TURNO::ELEITO;
            else if (valorLimpo == "-1")
                situacaoTurno = CD_SIT_TOT_TURNO::INVALIDO;
            else
                situacaoTurno = CD_SIT_TOT_TURNO::NAO_ELEITO;
        }
        else {
            throw invalid_argument("Coluna desconhecida: " + campo);
        }
    } catch (const exception &e) {
        throw invalid_argument("Erro ao processar o campo " + campo + " com valor \"" + valorLimpo + "\": " + e.what());
    }
}

bool Candidato::ehVereador() const {
    return CD_CARGO == CODIGO_VEREADOR;
}

bool Candidato::ehEleito() const {
    return situacaoTurno == CD_SIT_TOT_TURNO::ELEITO;
}

bool Candidato::candidaturaInvalida() const {
    return situacaoTurno == CD_SIT_TOT_TURNO::INVALIDO;
}

string Candidato::toString() const {
    ostringstream oss;
    // Se NR_FEDERACAO != -1, prefixa com '*'
    if (NR_FEDERACAO != -1)
        oss << "*";
    oss << NM_URNA_CANDIDATO << " (" << SG_PARTIDO << ", " << votos;
    oss << (votos == 1 ? " voto)" : " votos)");
    return oss.str();
}
