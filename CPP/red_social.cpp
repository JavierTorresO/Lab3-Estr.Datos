//tiene las implementaciones(los metodos)
#include "../H/red_social.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Metodo Add: agrega la relación de amistad entre 2 usuarios u1 y u2
void RedSocial::add(const std::string& u1, const std::string& u2) {
    // Agregar u2 a la lista de amigos de u1
    grafo[u1].insert(u2);

    // Agregar u1 a la lista de amigos de u2
    grafo[u2].insert(u1);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Implementación del recorrido DFS privado (recursivo)
bool RedSocial::dfs(const std::string& actual, const std::string& objetivo, std::unordered_set<std::string>& visitados) const {
    if (actual == objetivo) return true;
    visitados.insert(actual);
    for (const auto& vecino : grafo.at(actual)) {
        if (visitados.find(vecino) == visitados.end()) {
            if (dfs(vecino, objetivo, visitados)) return true;
        }
    }
    return false;
}

// Metodo Find: (usando DFS), busca si el usuario es parte o no de la red social
bool RedSocial::find(const std::string& u) const {
    if (grafo.empty()) return false;

    if (grafo.find(u) == grafo.end()) return false;

    std::unordered_set<std::string> visitados;

    // Buscar en todas las componentes conectadas
    for (const auto& par : grafo) {
        const std::string& nodo = par.first;
        if (visitados.find(nodo) == visitados.end()) {
            if (dfs(nodo, u, visitados)) return true;
        }
    }

    return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Metodo Follow: Encuentra los n usuarios más importantes de la red (importacia=numero de amigos -> grado del nodo)
std::vector<std::string> RedSocial::follow(int n) const {
    // Vector para pares (usuario, grado)
    std::vector<std::pair<std::string, int>> grados;

    for (const auto& par : grafo) {
        grados.emplace_back(par.first, par.second.size());
    }

    // Ordenar por grado descendente
    std::sort(grados.begin(), grados.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Mayor grado primero
    });

    // Sacar los primeros n usuarios (o menos si no hay tantos)
    std::vector<std::string> resultado;
    int limite = std::min(n, static_cast<int>(grados.size()));

    for (int i = 0; i < limite; ++i) {
        resultado.push_back(grados[i].first);
    }

    return resultado;
}