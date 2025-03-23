#include "Partido.hpp"

Partido::Partido(const std::string &sigla, int numero) : sigla(sigla), numero(numero), votosLegenda(0) {}

const std::set<Candidato*>& Partido::getCandidatos() const { return candidatos; }

void Partido::insereCandidato(Candidato* candidato) {
    candidatos.insert(candidato);
}

int Partido::getNumero() const { return numero; }
const std::string& Partido::getSigla() const { return sigla; }
int Partido::getVotosLegenda() const { return votosLegenda; }
void Partido::aumentaVotoLegenda(int votos) {
    votosLegenda += votos;
}
