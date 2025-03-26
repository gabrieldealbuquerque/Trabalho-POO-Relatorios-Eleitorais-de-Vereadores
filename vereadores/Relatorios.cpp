/**
 * @author: João Henrique
 * @author: Gabriel de Albuquerque
 */

#include "Relatorios.hpp"
#include "Eleicao.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <ctime>

using namespace std;

// Função auxiliar para calcular a porcentagem
static double calculatePercentage(int count, int total) {
    return (total == 0) ? 0.0 : static_cast<double>(count) / total;
}

// Função auxiliar para calcular a idade (em anos) a partir de duas datas
static int calculateAge(const tm &birth, const tm &election) {
    int age = election.tm_year - birth.tm_year;
    if (election.tm_mon < birth.tm_mon || (election.tm_mon == birth.tm_mon && election.tm_mday < birth.tm_mday))
        age--;
    return age;
}

void Relatorios::GeraRelatorio1() {
    cout << "\nNúmero de vagas: " << Eleicao::instance().getVagas() << endl;
}

void Relatorios::GeraRelatorio2() {
    cout << "\nVereadores eleitos:" << endl;
    vector<Candidato*> eleitos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->ehEleito() && c->getSG_UE() == Eleicao::instance().getMunicipio())
            eleitos.push_back(c);
    }
    sort(eleitos.begin(), eleitos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    for (size_t i = 0; i < eleitos.size(); i++) {
        cout << (i + 1) << " - " << eleitos[i]->toString() << endl;
    }
}

void Relatorios::GeraRelatorio3() {
    cout << "\nCandidatos mais votados (em ordem decrescente de votação e respeitando número de vagas):" << endl;
    vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (int i = 0; i < vagas && i < static_cast<int>(candidatos.size()); i++) {
        cout << (i + 1) << " - " << candidatos[i]->toString() << endl;
    }
}

void Relatorios::GeraRelatorio4() {
    cout << "\nTeriam sido eleitos se a votação fosse majoritária, e não foram eleitos:" << endl;
    cout << "(com sua posição no ranking de mais votados)" << endl;
    vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (int i = 0; i < vagas && i < static_cast<int>(candidatos.size()); i++) {
        if (!candidatos[i]->ehEleito())
            cout << (i + 1) << " - " << candidatos[i]->toString() << endl;
    }
}

void Relatorios::GeraRelatorio5() {
    cout << "\nEleitos, que se beneficiaram do sistema proporcional:" << endl;
    cout << "(com sua posição no ranking de mais votados)" << endl;
    vector<Candidato*> candidatos;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->getSG_UE() == Eleicao::instance().getMunicipio())
            candidatos.push_back(c);
    }
    sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
    int vagas = Eleicao::instance().getVagas();
    for (size_t i = vagas; i < candidatos.size(); i++) {
        Candidato* c = candidatos[i];
        if (c->ehEleito())
            cout << (i + 1) << " - " << c->toString() << endl;
    }
}

void Relatorios::GeraRelatorio6() {
    cout << "\nVotação dos partidos e número de candidatos eleitos:" << endl;
    vector<Partido*> partidos(Eleicao::instance().getPartidos().begin(), Eleicao::instance().getPartidos().end());
    sort(partidos.begin(), partidos.end(), PartidoVotosComparator());
    
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
        cout << (i + 1) << " - " << p->getSigla() << " - " << p->getNumero() 
             << ", " << totalVotos;
        
        if (totalVotos <= 1) {
            cout << " voto (" << votosNominais;
        } else {
            cout << " votos (" << votosNominais;
        }
        
        if (votosNominais <= 1) {
            cout << " nominal e ";
        } else {
            cout << " nominais e ";
        }
        
        cout << p->getVotosLegenda() << " de legenda), " << eleitos;
        
        if (eleitos <= 1) {
            cout << " candidato eleito" << endl;
        } else {
            cout << " candidatos eleitos" << endl;
        }
    }
}

void Relatorios::GeraRelatorio7() {
    cout << "\nPrimeiro e último colocados de cada partido:" << endl;
    vector<Partido*> partidos;
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
    sort(partidos.begin(), partidos.end(), PartidoCandidatoComparator());
    
    int i = 0;
    for (auto p : partidos) {
        i++;
        vector<Candidato*> candidatos;
        for (auto c : p->getCandidatos()) {
            if (c->getSG_UE() == Eleicao::instance().getMunicipio())
                candidatos.push_back(c);
        }
        sort(candidatos.begin(), candidatos.end(), CandidatoComparator(Eleicao::instance().getDataEleicao()));
        if (!candidatos.empty()) {
            Candidato* primeiro = candidatos.front();
            Candidato* ultimo = candidatos.back();
            cout << i << " - " << p->getSigla() << " - " << primeiro->getNR_PARTIDO() 
                      << ", " << primeiro->getNM_URNA_CANDIDATO() << " (" << primeiro->getNR_CANDIDATO() 
                      << ", " << primeiro->getVotos(); 
                    if (primeiro->getVotos() <= 1) {
                        cout << " voto) / ";
                    } else {
                        cout << " votos) / ";
                    }
                    cout << ultimo->getNM_URNA_CANDIDATO() 
                      << " (" << ultimo->getNR_CANDIDATO() << ", " << ultimo->getVotos();
                      if (ultimo->getVotos() <= 1) {
                        cout << " voto)" << endl;
                    } else {
                        cout << " votos)" << endl;
                    }
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
    cout << "\nEleitos, por faixa etária (na data da eleição):" << endl;
    cout << "      Idade < 30: " << ate30 << " (" << fixed << setprecision(2) 
              << calculatePercentage(ate30, vagas) * 100 << "%)" << endl;
    cout << "30 <= Idade < 40: " << ate40 << " (" << fixed << setprecision(2) 
              << calculatePercentage(ate40, vagas) * 100 << "%)" << endl;
    cout << "40 <= Idade < 50: " << ate50 << " (" << fixed << setprecision(2) 
              << calculatePercentage(ate50, vagas) * 100 << "%)" << endl;
    cout << "50 <= Idade < 60: " << ate60 << " (" << fixed << setprecision(2) 
              << calculatePercentage(ate60, vagas) * 100 << "%)" << endl;
    cout << "60 <= Idade     : " << acima60 << " (" << fixed << setprecision(2) 
              << calculatePercentage(acima60, vagas) * 100 << "%)" << endl;
}

void Relatorios::GeraRelatorio9() {
    int feminino = 0, masculino = 0;
    for (auto &pair : Eleicao::instance().getCandidatos()) {
        Candidato* c = pair.second;
        if (c->ehEleito() && c->getSG_UE() == Eleicao::instance().getMunicipio()) {
            if (c->getCD_GENERO())
                masculino++;
            else
                feminino++;
        }
    }
    int vagas = Eleicao::instance().getVagas();
    cout << "\nEleitos, por gênero:" << endl;
    cout << "Feminino: " << feminino << " (" << fixed << setprecision(2) 
              << calculatePercentage(feminino, vagas) * 100 << "%)" << endl;
    cout << "Masculino: " << masculino << " (" << fixed << setprecision(2) 
              << calculatePercentage(masculino, vagas) * 100 << "%)" << endl;
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
    cout << "\nTotal de votos válidos:\t" << totalVotos << endl;
    cout << "Total de votos nominais:\t" << totalVotosNominais << " (" << fixed << setprecision(2)
              << calculatePercentage(totalVotosNominais, totalVotos) * 100 << "%)" << endl;
    cout << "Total de votos de legenda:\t" << totalVotosLegenda << " (" << fixed << setprecision(2)
              << calculatePercentage(totalVotosLegenda, totalVotos) * 100 << "%)" << endl;
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
