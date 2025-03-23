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

static bool candidatoValido(Candidato* candidato, const Eleicao &eleicao) {
    return candidato->ehVereador() &&
           !candidato->candidaturaInvalida() &&
           candidato->getSG_UE() == eleicao.getMunicipio() &&
           !candidato->getNR_CANDIDATO().empty() &&
           !candidato->getNM_URNA_CANDIDATO().empty();
}

void Eleicao::criaCandidato(Candidato* novoCandidato) {
    Partido* p = getPartidoPorNumero(novoCandidato->getNR_PARTIDO());
    if (p == nullptr) {
        p = new Partido(novoCandidato->getSG_PARTIDO(), novoCandidato->getNR_PARTIDO());
        criaPartido(p);
    }
    if (candidatoValido(novoCandidato, *this)) {
        int num = stoi(novoCandidato->getNR_CANDIDATO());
        candidatos[num] = novoCandidato;
        p->insereCandidato(novoCandidato);
    }
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
