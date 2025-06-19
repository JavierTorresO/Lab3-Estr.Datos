# Proyecto Estructura de Datos: Simulador de Red Social

## Descripción del problema

Este proyecto consiste en implementar un programa que simula las operaciones básicas de una red social. Cada usuario es un vértice en un grafo no dirigido, y las amistades son las aristas. El programa debe soportar los siguientes comandos:

- **Add u1 u2**: Agregar una relación de amistad bidireccional entre los usuarios `u1` y `u2`.
- **Find u**: Verificar si el usuario `u` existe en la red (imprime `Yes` o `No`).
- **Clique**: Encontrar y listar todos los cliques máximos de tamaño entre 3 y 5 (cada clique impreso en una línea, vértices separados por espacios).
- **Compact**: Compactar cada clique máximo en un "vértice virtual" y reconstruir el grafo resultante, imprimiendo cada arista en formato `(origen, destino)`.
- **Follow n**: Mostrar los `n` usuarios con mayor número de amigos (grado más alto), en orden descendente.

## Integrantes

- Martín González
- Javier Torres
- Joaquín Quintana

## Compilación y ejecución

1. Abrir una terminal en la carpeta raíz del proyecto.
2. Compilar con:
   ```bash
   g++ -std=c++17 main.cpp CPP/red_social.cpp -o red_social
   ```
3. Ejecutar con un archivo de comandos, por ejemplo:
   ```bash
   ./red_social < entrada.txt
   ```
