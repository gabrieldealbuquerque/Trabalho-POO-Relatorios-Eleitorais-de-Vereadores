/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

#ifndef PARTIDO_HPP
#define PARTIDO_HPP

#include "Candidato.hpp"
#include <string>
#include <set>

using namespace std;

class Partido {
private:
    string sigla;
    int numero;
    set<Candidato*> candidatos;
    int votosLegenda;
public:
    Partido(const string &sigla, int numero);
    ~Partido();
    const set<Candidato*>& getCandidatos() const;
    void insereCandidato(Candidato* candidato);
    int getNumero() const;
    const string& getSigla() const;
    int getVotosLegenda() const;
    void aumentaVotoLegenda(int votos);
};

#endif // PARTIDO_HPP
