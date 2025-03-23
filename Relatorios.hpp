#ifndef RELATORIOS_HPP
#define RELATORIOS_HPP

#include "Eleicao.hpp"
#include "Candidato.hpp"
#include "Partido.hpp"
#include <vector>
#include <ctime>

class Relatorios {
public:
    void GeraRelatorio1();
    void GeraRelatorio2();
    void GeraRelatorio3();
    void GeraRelatorio4();
    void GeraRelatorio5();
    void GeraRelatorio6();
    void GeraRelatorio7();
    void GeraRelatorio8();
    void GeraRelatorio9();
    void GeraRelatorio10();
    void geraTodosRelatorios();
    
    // Functors de comparação
    struct CandidatoComparator {
        std::tm dataEleicao;
        CandidatoComparator(const std::tm &dt) : dataEleicao(dt) {}
        bool operator()(Candidato* c1, Candidato* c2) const;
    };
    
    struct PartidoVotosComparator {
        bool operator()(Partido* p1, Partido* p2) const;
    };
    
    struct PartidoCandidatoComparator {
        bool operator()(Partido* p1, Partido* p2) const;
    };
};

#endif // RELATORIOS_HPP
