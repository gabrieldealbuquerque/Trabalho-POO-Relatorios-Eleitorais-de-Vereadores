#ifndef PARTIDO_HPP
#define PARTIDO_HPP

#include "Candidato.hpp"
#include <string>
#include <set>

class Partido {
private:
    std::string sigla;
    int numero;
    std::set<Candidato*> candidatos;
    int votosLegenda;
public:
    Partido(const std::string &sigla, int numero);
    const std::set<Candidato*>& getCandidatos() const;
    void insereCandidato(Candidato* candidato);
    int getNumero() const;
    const std::string& getSigla() const;
    int getVotosLegenda() const;
    void aumentaVotoLegenda(int votos);
};

#endif // PARTIDO_HPP
