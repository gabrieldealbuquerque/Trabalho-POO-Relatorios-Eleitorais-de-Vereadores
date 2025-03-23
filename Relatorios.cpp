#include "Relatorios.hpp"
#include "Eleicao.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <ctime>

// Função auxiliar para calcular a porcentagem
static double calculatePercentage(int count, int total) {
    return (total == 0) ? 0.0 : static_cast<double>(count) / total;
}

// Função auxiliar para calcular a idade (em anos) a partir de duas datas
static int calculateAge(const std::tm &birth, const std::tm &election) {
    int age = election.tm_year - birth.tm_year;
    if (election.tm_mon < birth.tm_mon || (election.tm_mon == birth.tm_mon && election.tm_mday < birth.tm_mday))
        age--;
    return age;
}

void Relatorios::GeraRelatorio1() {
    std::cout << "\nNúmero de vagas: " << Eleicao::instance().getVagas() << std::endl;
}

void Relatorios::GeraRelatorio2() {
    std::cout << "\nVereadores eleitos:" << std::endl;
    std::vector<Candidato*> eleitos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->ehEleito() && c->getSG_UE() == Eleicao::instance().getMunicipio())
            eleitos.push_back(c);
    }
    std::sort(eleitos.begin(), eleitos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    for (size_t i = 0; i < eleitos.size(); i++) {
        std::cout << (i + 1) << " - " << eleitos[i]->toString() << std::endl;
    }
}

void Relatorios::GeraRelatorio3() {
    std::cout << "\nCandidatos mais votados (em ordem decrescente de votação e respeitando número de vagas):" << std::endl;
    std::vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    std::sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (int i = 0; i < vagas && i < static_cast<int>(candidatos.size()); i++) {
        std::cout << (i + 1) << " - " << candidatos[i]->toString() << std::endl;
    }
}

void Relatorios::GeraRelatorio4() {
    std::cout << "\nTeriam sido eleitos se a votação fosse majoritária, e não foram eleitos:" << std::endl;
    std::vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    std::sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (int i = 0; i < vagas && i < static_cast<int>(candidatos.size()); i++) {
        if (!candidatos[i]->ehEleito())
            std::cout << (i + 1) << " - " << candidatos[i]->toString() << std::endl;
    }
}

void Relatorios::GeraRelatorio5() {
    std::cout << "\nEleitos, que se beneficiaram do sistema proporcional:" << std::endl;
    std::vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    std::sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (size_t i = vagas; i < candidatos.size(); i++) {
        Candidato* c = candidatos[i];
        if (c->ehEleito())
            std::cout << (i + 1) << " - " << c->toString() << std::endl;
    }
}

void Relatorios::GeraRelatorio6() {
    std::cout << "\nVotação dos partidos e número de candidatos eleitos:" << std::endl;
    std::vector<Partido*> partidos(Eleicao::instance().getPartidos().begin(), Eleicao::instance().getPartidos().end());
    std::sort(partidos.begin(), partidos.end(), PartidoVotosComparator());
    
    for (size_t i = 0; i < partidos.size(); i++) {
        Partido* p = partidos[i];
        int eleitos = 0;
        int votosNominais = 0;
        for (auto c : p->getCandidatos()) {
            if (c->ehEleito())
                eleitos++;
            votosNominais += c->getVotos();
        }
        int totalVotos = votosNominais + p->getVotosLegenda();
        std::cout << (i + 1) << " - " << p->getSigla() << " - " << p->getNumero() 
                  << ",  " << totalVotos << " votos (" << votosNominais 
                  << " nominais e " << p->getVotosLegenda() 
                  << " de legenda), " << eleitos << " candidato(s) eleito(s)" << std::endl;
    }
}

void Relatorios::GeraRelatorio7() {
    std::cout << "\nPrimeiro e último colocados de cada partido:" << std::endl;
    std::vector<Partido*> partidos;
    for (auto p : Eleicao::instance().getPartidos()) {
        bool hasPositive = false;
        for (auto c : p->getCandidatos()) {
            if (c->getSG_UE() == Eleicao::instance().getMunicipio() && c->getVotos() > 0) {
                hasPositive = true;
                break;
            }
        }
        if (hasPositive)
            partidos.push_back(p);
    }
    std::sort(partidos.begin(), partidos.end(), PartidoCandidatoComparator());
    
    int i = 0;
    for (auto p : partidos) {
        i++;
        std::vector<Candidato*> candidatos;
        for (auto c : p->getCandidatos()) {
            if (c->getSG_UE() == Eleicao::instance().getMunicipio())
                candidatos.push_back(c);
        }
        std::sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
        if (!candidatos.empty()) {
            Candidato* primeiro = candidatos.front();
            Candidato* ultimo = candidatos.back();
            std::cout << i << " - " << p->getSigla() << " - " << primeiro->getNR_PARTIDO() 
                      << ", " << primeiro->getNM_URNA_CANDIDATO() << " (" << primeiro->getNR_CANDIDATO() 
                      << ", " << primeiro->getVotos() << " votos) / " << ultimo->getNM_URNA_CANDIDATO() 
                      << " (" << ultimo->getNR_CANDIDATO() << ", " << ultimo->getVotos() << " votos)" << std::endl;
        }
    }
}

void Relatorios::GeraRelatorio8() {
    int ate30 = 0, ate40 = 0, ate50 = 0, ate60 = 0, acima60 = 0;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (!c->ehEleito() || c->getSG_UE() != Eleicao::instance().getMunicipio())
            continue;
        int idade = calculateAge(c->getDT_NASCIMENTO(), Eleicao::instance().getDataEleicao());
        if (idade < 30)
            ate30++;
        else if (idade < 40)
            ate40++;
        else if (idade < 50)
            ate50++;
        else if (idade < 60)
            ate60++;
        else
            acima60++;
    }
    int vagas = Eleicao::instance().getVagas();
    std::cout << "\nEleitos, por faixa etária (na data da eleição):" << std::endl;
    std::cout << "      Idade < 30: " << ate30 << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(ate30, vagas) * 100 << "%)" << std::endl;
    std::cout << "30 <= Idade < 40: " << ate40 << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(ate40, vagas) * 100 << "%)" << std::endl;
    std::cout << "40 <= Idade < 50: " << ate50 << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(ate50, vagas) * 100 << "%)" << std::endl;
    std::cout << "50 <= Idade < 60: " << ate60 << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(ate60, vagas) * 100 << "%)" << std::endl;
    std::cout << "60 <= Idade     : " << acima60 << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(acima60, vagas) * 100 << "%)" << std::endl;
}

void Relatorios::GeraRelatorio9() {
    int feminino = 0, masculino = 0;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->ehEleito() && c->getSG_UE() == Eleicao::instance().getMunicipio()) {
            if (c->getCD_GENERO())
                feminino++;
            else
                masculino++;
        }
    }
    int vagas = Eleicao::instance().getVagas();
    std::cout << "\nEleitos, por gênero:" << std::endl;
    std::cout << "Feminino: " << feminino << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(feminino, vagas) * 100 << "%)" << std::endl;
    std::cout << "Masculino: " << masculino << " (" << std::fixed << std::setprecision(2) 
              << calculatePercentage(masculino, vagas) * 100 << "%)" << std::endl;
}

void Relatorios::GeraRelatorio10() {
    int totalVotosNominais = 0;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio() && c->getNR_CANDIDATO().size() == 5)
            totalVotosNominais += c->getVotos();
    }
    int totalVotosLegenda = 0;
    for (auto p : Eleicao::instance().getPartidos()) {
        totalVotosLegenda += p->getVotosLegenda();
    }
    int totalVotos = totalVotosNominais + totalVotosLegenda;
    std::cout << "\nTotal de votos válidos:\t" << totalVotos << std::endl;
    std::cout << "Total de votos nominais:\t" << totalVotosNominais << " (" << std::fixed << std::setprecision(2)
              << calculatePercentage(totalVotosNominais, totalVotos) * 100 << "%)" << std::endl;
    std::cout << "Total de votos de legenda:\t" << totalVotosLegenda << " (" << std::fixed << std::setprecision(2)
              << calculatePercentage(totalVotosLegenda, totalVotos) * 100 << "%)" << std::endl;
}

void Relatorios::geraTodosRelatorios() {
    GeraRelatorio1();
    GeraRelatorio2();
    GeraRelatorio3();
    GeraRelatorio4();
    GeraRelatorio5();
    GeraRelatorio6();
    GeraRelatorio7();
    GeraRelatorio8();
    GeraRelatorio9();
    GeraRelatorio10();
}

// Implementação dos functors de comparação
bool Relatorios::CandidatoComparator::operator()(Candidato* c1, Candidato* c2) const {
    if (c2->getVotos() != c1->getVotos())
        return c2->getVotos() < c1->getVotos(); // ordem decrescente de votos
    int idadeC1 = calculateAge(c1->getDT_NASCIMENTO(), dataEleicao);
    int idadeC2 = calculateAge(c2->getDT_NASCIMENTO(), dataEleicao);
    return idadeC2 < idadeC1; // em caso de empate, o mais velho vem primeiro
}

bool Relatorios::PartidoVotosComparator::operator()(Partido* p1, Partido* p2) const {
    int votosP1 = p1->getVotosLegenda();
    for (auto c : p1->getCandidatos())
        votosP1 += c->getVotos();
    int votosP2 = p2->getVotosLegenda();
    for (auto c : p2->getCandidatos())
        votosP2 += c->getVotos();
    if (votosP1 != votosP2)
        return votosP2 < votosP1; // ordem decrescente
    return p1->getNumero() < p2->getNumero();
}

bool Relatorios::PartidoCandidatoComparator::operator()(Partido* p1, Partido* p2) const {
    int maxVotosP1 = 0;
    for (auto c : p1->getCandidatos())
        if (c->getVotos() > maxVotosP1)
            maxVotosP1 = c->getVotos();
    int maxVotosP2 = 0;
    for (auto c : p2->getCandidatos())
        if (c->getVotos() > maxVotosP2)
            maxVotosP2 = c->getVotos();
    return maxVotosP2 < maxVotosP1; // ordem decrescente
}
