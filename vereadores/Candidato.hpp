#ifndef CANDIDATO_HPP
#define CANDIDATO_HPP

#include <string>
#include <ctime>

using namespace std;


// Enumeração representando a situação do candidato
enum class CD_SIT_TOT_TURNO {
    ELEITO,
    INVALIDO,
    NAO_ELEITO
};

class Candidato {
private:
    static const int CODIGO_VEREADOR = 13;
    int SG_UE;
    int CD_CARGO;
    string NR_CANDIDATO;
    string NM_URNA_CANDIDATO;
    int NR_PARTIDO;
    string SG_PARTIDO;
    int NR_FEDERACAO;
    tm DT_NASCIMENTO;
    bool CD_GENERO; // true = Feminino, false = Masculino
    CD_SIT_TOT_TURNO situacaoTurno;
    int votos;
public:
    Candidato();

    // Getters
    int getSG_UE() const;
    int getCD_CARGO() const;
    const string& getNR_CANDIDATO() const;
    const string& getNM_URNA_CANDIDATO() const;
    int getNR_PARTIDO() const;
    const string& getSG_PARTIDO() const;
    int getNR_FEDERACAO() const;
    tm getDT_NASCIMENTO() const;
    bool getCD_GENERO() const;
    CD_SIT_TOT_TURNO getSituacaoTurno() const;
    int getVotos() const;
    
    // Métodos
    void increaseVotos(int v);
    void setAtributo(const string &campo, const string &valor);
    bool ehVereador() const;
    bool ehEleito() const;
    bool candidaturaInvalida() const;
    string toString() const;
};

#endif // CANDIDATO_HPP
