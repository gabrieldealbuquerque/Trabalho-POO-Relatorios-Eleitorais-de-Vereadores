/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

#ifndef ELEICAO_HPP
#define ELEICAO_HPP

#include "Candidato.hpp"
#include "Partido.hpp"
#include <map>
#include <set>
#include <ctime>

using namespace std;

class Eleicao {
private:
    int municipio;
    int vagas;
    tm dataEleicao;
    map<int, Candidato*> candidatos; // chave: número do candidato
    set<Partido*> partidos;
    
    // Construtor privado para implementação do singleton
    Eleicao();
public:
    static Eleicao& instance();
    Eleicao(const Eleicao&) = delete;
    ~Eleicao();
    Eleicao& operator=(const Eleicao&) = delete;
    
    // Getters e setters
    int getMunicipio() const;
    void setMunicipio(int m);
    int getVagas() const;
    void aumentaVagas(Candidato* candidato);
    tm getDataEleicao() const;
    void setDataEleicao(const tm &dt);
    const map<int, Candidato*>& getCandidatos() const;
    const set<Partido*>& getPartidos() const;
    
    bool criaCandidato(Candidato* novoCandidato);
    void criaPartido(Partido* novoPartido);
    void processaVoto(int numeroCandidato, int votos);
    Candidato* getCandidatoPorNumero(int numeroCandidato);
    Partido* getPartidoPorNumero(int numero);
    void imprimeCandidatos() const;
    bool candidatoValido(Candidato* candidato) const;
};

#endif // ELEICAO_HPP
