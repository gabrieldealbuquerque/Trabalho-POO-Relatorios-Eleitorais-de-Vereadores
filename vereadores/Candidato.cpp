#include "Candidato.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstring>

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
    if (valor.empty())
        return;
    try {
        if (campo == "SG_UE")
            SG_UE = stoi(valor);
        else if (campo == "CD_CARGO")
            CD_CARGO = stoi(valor);
        else if (campo == "NR_CANDIDATO")
            NR_CANDIDATO = valor;
        else if (campo == "NM_URNA_CANDIDATO")
            NM_URNA_CANDIDATO = valor;
        else if (campo == "NR_PARTIDO")
            NR_PARTIDO = stoi(valor);
        else if (campo == "SG_PARTIDO")
            SG_PARTIDO = valor;
        else if (campo == "NR_FEDERACAO")
            NR_FEDERACAO = stoi(valor);
        else if (campo == "DT_NASCIMENTO")
            DT_NASCIMENTO = parseDate(valor);
        else if (campo == "CD_GENERO")
            CD_GENERO = (valor == "2"); // 2 = Feminino
        else if (campo == "CD_SIT_TOT_TURNO") {
            if (valor == "2" || valor == "3")
                situacaoTurno = CD_SIT_TOT_TURNO::ELEITO;
            else if (valor == "-1")
                situacaoTurno = CD_SIT_TOT_TURNO::INVALIDO;
            else
                situacaoTurno = CD_SIT_TOT_TURNO::NAO_ELEITO;
        }
        else
            throw invalid_argument("Coluna desconhecida: " + campo);
    } catch (const exception &e) {
        throw invalid_argument("Erro ao processar o campo " + campo + " com valor " + valor + ": " + e.what());
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
