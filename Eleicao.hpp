#ifndef ELEICAO_HPP
#define ELEICAO_HPP

#include "Candidato.hpp"
#include "Partido.hpp"
#include <map>
#include <set>
#include <ctime>

class Eleicao {
private:
    int municipio;
    int vagas;
    std::tm dataEleicao;
    std::map<int, Candidato*> candidatos; // chave: número do candidato
    std::set<Partido*> partidos;
    
    // Construtor privado para implementação do singleton
    Eleicao();
public:
    static Eleicao& instance();
    Eleicao(const Eleicao&) = delete;
    Eleicao& operator=(const Eleicao&) = delete;
    
    // Getters e setters
    int getMunicipio() const;
    void setMunicipio(int m);
    int getVagas() const;
    void aumentaVagas(Candidato* candidato);
    std::tm getDataEleicao() const;
    void setDataEleicao(const std::tm &dt);
    const std::map<int, Candidato*>& getCandidatos() const;
    const std::set<Partido*>& getPartidos() const;
    
    void criaCandidato(Candidato* novoCandidato);
    void criaPartido(Partido* novoPartido);
    void processaVoto(int numeroCandidato, int votos);
    Candidato* getCandidatoPorNumero(int numeroCandidato);
    Partido* getPartidoPorNumero(int numero);
    void imprimeCandidatos() const;
};

#endif // ELEICAO_HPP
