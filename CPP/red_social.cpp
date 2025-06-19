// tiene las implementaciones(los metodos)
#include "../H/red_social.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>

void RedSocial::ejecutar_comando(const std::string &linea)
{
    // Copiamos y quitamos CR/LF y espacios finales
    std::string l = linea;
    while (!l.empty() && std::isspace(static_cast<unsigned char>(l.back())))
        l.pop_back();

    // Separamos el comando del resto
    std::string cmd, rest;
    auto pos = l.find(' ');
    if (pos == std::string::npos)
    {
        cmd = l;
    }
    else
    {
        cmd = l.substr(0, pos);
        rest = l.substr(pos + 1);
    }

    // Y ahora comparamos
    if (cmd == "Add")
    {
        std::istringstream iss(rest);
        std::string u1, u2;
        iss >> u1 >> u2;
        add(u1, u2);
    }
    else if (cmd == "Find")
    {
        // rest es directamente el nombre de usuario
        bool existe = find(rest);
        std::cout << (existe ? "Yes\n" : "No\n");
    }
    else if (cmd == "Clique")
    {
        for (auto &c : clique())
        {
            for (size_t i = 0; i < c.size(); ++i)
                std::cout << c[i] << (i + 1 < c.size() ? ' ' : '\n');
        }
    }
    else if (cmd == "Compact")
    {
        for (auto &p : compact())
            std::cout << "(" << p.first << ", " << p.second << ")\n";
    }
    else if (cmd == "Follow")
    {
        int n = std::stoi(rest);
        for (auto &u : follow(n))
            std::cout << u << "\n";
    }
    // si quieres, puedes manejar un else para errores
}

// Metodo Add: agrega la relación de amistad entre 2 usuarios u1 y u2
void RedSocial::add(const std::string &u1, const std::string &u2)
{
    // Agregar u2 a la lista de amigos de u1
    grafo[u1].insert(u2);

    // Agregar u1 a la lista de amigos de u2
    grafo[u2].insert(u1);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// private helper: busca 'objetivo' en el subgrafo de 'actual'
bool RedSocial::dfs(const std::string &actual,
                    const std::string &objetivo,
                    std::unordered_set<std::string> &visitados) const
{
    if (actual == objetivo)
        return true;
    visitados.insert(actual);
    // recorrer vecinos
    for (const auto &vecino : grafo.at(actual))
    {
        if (!visitados.count(vecino))
        {
            if (dfs(vecino, objetivo, visitados))
                return true;
        }
    }
    return false;
}

// public: devuelve true si el usuario 'u' está en la red
bool RedSocial::find(const std::string &u) const
{
    // si no existe clave, ni siquiera arrancamos
    auto it = grafo.find(u);
    if (it == grafo.end())
        return false;

    // si existe, hacemos DFS desde 'u' a 'u' (es un ciclo trivial)
    std::unordered_set<std::string> visitados;
    return dfs(u, u, visitados);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Metodo Follow: Encuentra los n usuarios más importantes de la red (importacia=numero de amigos -> grado del nodo)
std::vector<std::string> RedSocial::follow(int n) const
{
    // Vector para pares (usuario, grado)
    std::vector<std::pair<std::string, int>> grados;

    for (const auto &par : grafo)
    {
        grados.emplace_back(par.first, par.second.size());
    }

    // Ordenar por grado descendente
    std::sort(grados.begin(), grados.end(), [](const auto &a, const auto &b)
              {
                  return a.second > b.second; // Mayor grado primero
              });

    // Sacar los primeros n usuarios (o menos si no hay tantos)
    std::vector<std::string> resultado;
    int limite = std::min(n, static_cast<int>(grados.size()));

    for (int i = 0; i < limite; ++i)
    {
        resultado.push_back(grados[i].first);
    }

    return resultado;
}

// Bron–Kerbosch recursivo para encontrar todos los cliques máximos
static void bronk(
    const std::unordered_map<std::string, std::unordered_set<std::string>> &grafo,
    std::vector<std::string> &R,
    std::unordered_set<std::string> &P,
    std::unordered_set<std::string> &X,
    std::vector<std::vector<std::string>> &cliques)
{
    if (P.empty() && X.empty())
    {
        // es un clique maximal
        if (R.size() >= 3 && R.size() <= 5)
        {
            std::vector<std::string> clique = R;
            std::sort(clique.begin(), clique.end());
            cliques.push_back(clique);
        }
        return;
    }

    // iteramos sobre una copia de P para poder modificar P sobre la marcha
    std::vector<std::string> Pvec(P.begin(), P.end());
    for (const auto &v : Pvec)
    {
        if (!P.count(v))
            continue; // ya fue retirado
        // R' = R ∪ {v}
        R.push_back(v);

        // P' = P ∩ N(v)
        std::unordered_set<std::string> P2;
        for (auto &u : P)
            if (grafo.at(v).count(u))
                P2.insert(u);

        // X' = X ∩ N(v)
        std::unordered_set<std::string> X2;
        for (auto &u : X)
            if (grafo.at(v).count(u))
                X2.insert(u);

        bronk(grafo, R, P2, X2, cliques);

        // Deshacer cambios: R deja de contener v; mover v de P a X
        R.pop_back();
        P.erase(v);
        X.insert(v);
    }
}

std::vector<std::vector<std::string>> RedSocial::clique() const
{
    std::vector<std::vector<std::string>> cliques;
    std::vector<std::string> R;
    std::unordered_set<std::string> P, X;

    // Inicializar P = todos los vértices del grafo
    for (auto &kv : grafo)
    {
        P.insert(kv.first);
    }

    bronk(grafo, R, P, X, cliques);

    // Ordenamos la lista de cliques: primero por tamaño (asc), luego lexicográficamente
    std::sort(cliques.begin(), cliques.end(),
              [](auto &a, auto &b)
              {
                  if (a.size() != b.size())
                      return a.size() < b.size();
                  return a < b; // vector<string> define comparación lex
              });

    return cliques;
}

std::vector<std::pair<std::string, std::string>> RedSocial::compact() const
{
    // 1) Obtengo los cliques que ya calcula clique()
    auto clqs = clique();

    // 2) Asigno un nombre “Componente 1”, “Componente 2”, … a cada clique
    std::unordered_map<std::string, std::string> compMap;
    for (size_t i = 0; i < clqs.size(); ++i)
    {
        std::string comp = "Componente " + std::to_string(i + 1);
        for (auto &v : clqs[i])
        {
            compMap[v] = comp;
        }
    }
    // 3) Los vértices que no están en ningún clique se quedan con su propio nombre
    for (auto &kv : grafo)
    {
        if (!compMap.count(kv.first))
            compMap[kv.first] = kv.first;
    }

    // 4) Recorro cada arista (u,v) del grafo original y la agrego al grafo compactado
    //    pero sólo si mapea a dos vértices distintos
    std::set<std::pair<std::string, std::string>> edgeSet;
    for (auto &kv : grafo)
    {
        auto u = kv.first;
        for (auto &v : kv.second)
        {
            if (u < v)
            { // evitar duplicados
                auto mu = compMap[u];
                auto mv = compMap[v];
                if (mu != mv)
                {
                    // normalizo el orden para que siempre salga (a, b) con a < b
                    if (mu < mv)
                        edgeSet.emplace(mu, mv);
                    else
                        edgeSet.emplace(mv, mu);
                }
            }
        }
    }

    // 5) Paso el set a un vector para devolverlo
    return std::vector<std::pair<std::string, std::string>>(edgeSet.begin(), edgeSet.end());
}