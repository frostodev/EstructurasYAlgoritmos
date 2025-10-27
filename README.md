# Estructuras y Algoritmos

Este repositorio es una librería personal escrita en C++20 que implementa una amplia variedad de estructuras de datos, algoritmos y funciones matemáticas fundamentales desde cero. El objetivo es proporcionar una referencia clara, moderna y bien documentada con un enfoque en la correcta gestión de memoria (**Rule of Five**) y el análisis de complejidad.

### 🚀 Características

#### 1. Estructuras de Datos (`/DataStructures`):
Implementaciones de contenedores genéricos, la mayoría siguiendo la **Rule of Five** para una gestión de memoria segura y eficiente.
- `Vector`: Un array dinámico con capacidad de redimensionamiento dinámico.
- `LinkedList`: Una lista enlazada simple con punteros head y tail para operaciones O(1) en ambos extremos.
- `DoublyLinkedList`: Una lista doblemente enlazada que permite inserciones/eliminaciones O(1) en cualquier extremo.
- `Stack`: Una pila LIFO (Last-In, First-Out) implementada sobre una lista enlazada.
- `Queue`: Una cola FIFO (First-In, First-Out) implementada sobre una lista enlazada con punteros head y tail para operaciones O(1).
- `PriorityQueue`: Una cola de prioridad basada en un Min-Heap, implementada sobre `DS::Vector`.
- `BinarySearchTree`: Un Árbol Binario de Búsqueda (BST) que soporta inserción, eliminación, búsqueda y varios tipos de recorridos (In-Orden, Pre-Orden, Post-Orden y Nivel-Orden).
- `HashMap`: Una tabla hash genérica que utiliza **encadenamiento separado** (con DS::LinkedList) para manejar colisiones y rehashing automático basado en factor de carga.
- `Graph`: Una clase de grafo genérica que soporta grafos dirigidos/no dirigidos y ponderados/no ponderados, implementada con una lista de adyacencia sobre DS::HashMap.

#### 2. Algoritmos (`/Algorithms`):
Implementaciones de algoritmos comunes que operan sobre las estructuras de datos.
- **Algoritmos de ordenamiento** (`Sorting.h`):
    - `Quicksort`
    - `Mergesort`
    - `Heapsort`
    - `Radixsort`
    - `Selectionsort`
    - `Insertionsort`
    - `Bubblesort`

- **Algoritmos de Grafos** (`Graphs.h`):
  - `DFS`
  - `BFS`
  - `Dijkstra`

#### 3. Matemáticas (`/Math`):
Una librería matemática construida desde cero, usando `concepts` de C++ 20 para plantillas numéricas.
- **Vectores**: `Vector2` y `Vector3` con sobrecarga completa para operadores, magnitud, normalización, etc.
- **Matrices**:
    - Implementación genérica con almacenamiento Row-major.
    - Operaciones elementales de fila y columna.
    - Cálculo de determinante mediante eliminación Gaussiana.
    - Multiplicación de matrices clásica y método de Strassen.
- **Funciones trigonométricas**:
    - Implementaciones de `sin`, `cos` y `tan` usando **series de Taylor** y reducción de rango.
    - Implementaciones de `asin`, `acos`, `atan` y `atan2` usando identidades.
- **Utilidades**
    - Funciones de redondeo.
    - Radicación, exponenciación y logaritmos.
    - Funciones de maniuplación de signo.

### 🏗️ Por implementar

Aún faltan varias estructuras, algoritmos y utilidades que me gustaría implementar. Por ejemplo:
- `HashSet`
- `Deque`
- `Pair`
- Algoritmos de Prim, Kruskal y Floyd-Warshall.

... y muchas cosas más.

### ⚠️ Importante

La idea es mantener la dependencia con la **STL** en lo mínimo. Sin embargo, hay algunas librerías (como `<stdexcept>` o `<iostream>`) de las cuales no he implementado un reemplazo directo.

Además, ciertas estructuras aún dependen de otras que no han sido implementadas en el repositorio. La idea a futuro es implementar todas las necesarias para ser (casi) autosuficiente.

### 📜 Licencia

Este proyecto está licenciado bajo los términos de la GNU General Public License v3.0.
Puedes ver el archivo LICENSE para más detalles.
