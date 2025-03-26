/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

#include "Eleicao.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

using namespace std;

Eleicao::Eleicao() : municipio(0), vagas(0) {
    memset(&dataEleicao, 0, sizeof(dataEleicao));
}

Eleicao& Eleicao::instance() {
    static Eleicao instance;
    return instance;
}

Eleicao::~Eleicao() {
    // Liberar memória dos candidatos
    for (auto& pair : candidatos) {
        delete pair.second; // Deleta cada Candidato*
    }
    candidatos.clear(); // Limpa o map

    // Liberar memória dos partidos
    for (auto partido : partidos) {
        delete partido; // Deleta cada Partido*
    }
    partidos.clear(); // Limpa o set
}

int Eleicao::getMunicipio() const { return municipio; }
void Eleicao::setMunicipio(int m) { municipio = m; }
int Eleicao::getVagas() const { return vagas; }
tm Eleicao::getDataEleicao() const { return dataEleicao; }
void Eleicao::setDataEleicao(const tm &dt) { dataEleicao = dt; }

const map<int, Candidato*>& Eleicao::getCandidatos() const { return candidatos; }
const set<Partido*>& Eleicao::getPartidos() const { return partidos; }

void Eleicao::aumentaVagas(Candidato* candidato) {
    if (candidato->ehEleito() && candidato->getSG_UE() == municipio) {
        vagas++;
    }
}

void Eleicao::criaPartido(Partido* novoPartido) {
    partidos.insert(novoPartido);
}

bool Eleicao::candidatoValido(Candidato* candidato) const {
    return candidato->ehVereador() &&
           !candidato->candidaturaInvalida() &&
           candidato->getSG_UE() == this->municipio &&
           !candidato->getNR_CANDIDATO().empty() &&
           !candidato->getNM_URNA_CANDIDATO().empty();
}

bool Eleicao::criaCandidato(Candidato* novoCandidato) {
    Partido* p = getPartidoPorNumero(novoCandidato->getNR_PARTIDO());
    if (p == nullptr) {
        p = new Partido(novoCandidato->getSG_PARTIDO(), novoCandidato->getNR_PARTIDO());
        criaPartido(p);
    }
    if (candidatoValido(novoCandidato)) {
        int num = stoi(novoCandidato->getNR_CANDIDATO());
        if (candidatos.find(num) == candidatos.end()) {
            candidatos[num] = novoCandidato;
            p->insereCandidato(novoCandidato);
        }
        return true;
    }
    return false;
}

void Eleicao::processaVoto(int numeroCandidato, int votos) {
    if (numeroCandidato < 100) {
        Partido* partido = getPartidoPorNumero(numeroCandidato);
        if (partido != nullptr) {
            partido->aumentaVotoLegenda(votos);
        }
    } else {
        auto it = candidatos.find(numeroCandidato);
        if (it != candidatos.end())
            it->second->increaseVotos(votos);
        else
            cerr << "Candidato não encontrado: " << numeroCandidato << endl;
    }
}

Candidato* Eleicao::getCandidatoPorNumero(int numeroCandidato) {
    auto it = candidatos.find(numeroCandidato);
    return (it != candidatos.end()) ? it->second : nullptr;
}

Partido* Eleicao::getPartidoPorNumero(int num) {
    for (auto p : partidos) {
        if (p->getNumero() == num)
            return p;
    }
    return nullptr;
}

void Eleicao::imprimeCandidatos() const {
    cout << "Candidatos:" << endl;
    for (auto &pair : candidatos) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == municipio)
            cout << c->toString() << endl;
    }
}
