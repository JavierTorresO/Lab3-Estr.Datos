#ifndef RED_SOCIAL_H
#define RED_SOCIAL_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

class RedSocial
{
private:
    // Grafo: para cada usuario, el conjunto de vecinos
    std::unordered_map<std::string, std::unordered_set<std::string>> grafo;

    // Auxiliar para DFS (usado en find u, y puede servir para clique, compact, etc.)
    bool dfs(const std::string &actual,
             const std::string &objetivo,
             std::unordered_set<std::string> &visitados) const;

public:
    // Agrega la arista bidireccional (u1, u2)
    void add(const std::string &u1, const std::string &u2);

    // Devuelve true si el vértice u está en el grafo
    bool find(const std::string &u) const;

    // Encuentra todos los cliques máximos de tamaño 3, 4 o 5
    std::vector<std::vector<std::string>> clique() const;

    // Compacta los cliques en vértices virtuales y devuelve la lista de aristas del grafo resultante
    std::vector<std::pair<std::string, std::string>> compact() const;

    // Devuelve los n usuarios con mayor grado (número de vecinos), en orden descendente
    std::vector<std::string> follow(int n) const;

    // Procesa una línea de comando (“Add …”, “Find …”, “Clique”, “Compact”, “Follow …”)
    void ejecutar_comando(const std::string &linea);
};

#endif // RED_SOCIAL_H
