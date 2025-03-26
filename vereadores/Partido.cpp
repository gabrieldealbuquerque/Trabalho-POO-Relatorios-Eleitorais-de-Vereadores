/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

#include "Partido.hpp"

Partido::Partido(const string &sigla, int numero) : sigla(sigla), numero(numero), votosLegenda(0) {}

Partido::~Partido() {
    candidatos.clear(); // Limpa o set
}

const set<Candidato*>& Partido::getCandidatos() const { return candidatos; }

void Partido::insereCandidato(Candidato* candidato) {
    candidatos.insert(candidato);
}

int Partido::getNumero() const { return numero; }
const string& Partido::getSigla() const { return sigla; }
int Partido::getVotosLegenda() const { return votosLegenda; }
void Partido::aumentaVotoLegenda(int votos) {
    votosLegenda += votos;
}
