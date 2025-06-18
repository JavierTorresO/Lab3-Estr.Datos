#include <iostream>
#include <string>
#include "H/red_social.h"

int main()
{
    RedSocial rs;
    std::string linea;
    while (std::getline(std::cin, linea))
    {
        if (linea.empty())
            continue;
        rs.ejecutar_comando(linea);
    }
    return 0;
}
