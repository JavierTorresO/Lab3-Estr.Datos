//para encabezados
#ifndef RED_SOCIAL_H
#define RED_SOCIAL_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

class RedSocial {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> grafo;

    // Método privado auxiliar para DFS recursiva
    bool dfs(const std::string& actual, const std::string& objetivo, std::unordered_set<std::string>& visitados) const;

public:
    void add(const std::string& u1, const std::string& u2);
    bool find(const std::string& u) const;
    std::vector<std::vector<std::string>> clique() const;
    std::vector<std::pair<std::string, std::string>> compact() const;
    std::vector<std::string> follow(int n) const;

    void ejecutar_comando(const std::string& linea);
};

#endif