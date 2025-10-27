
#ifndef SORTING_H
#define SORTING_H

#include "../DataStructures/Vector.h"
#include "../DataStructures/Queue.h"

namespace Algorithms {

    // -----------------------------------------------------------------
    // Quicksort
    // -----------------------------------------------------------------

    /**
     * @brief Función de partición para Quicksort (estilo Lomuto).
     * @param vec El vector a particionar.
     * @param low El índice inicial.
     * @param high El índice final (pivote).
     * @return El nuevo índice donde quedó el pivote.
     * @note Complejidad de Tiempo: O(k), donde k es el número de elementos
     * en el sub-arreglo (high - low + 1).
     */
    template <typename T>
    int partition(DS::Vector<T>& vec, int low, int high) {
        T pivot = vec[high]; // Tomar el último elemento como pivote
        int i = (low - 1);   // Índice del elemento más pequeño

        for (int j = low; j < high; ++j) {
            // Si el elemento actual es menor o igual al pivote
            if (vec[j] <= pivot) {
                i++; // Mover el índice 'i'
                std::swap(vec[i], vec[j]); // Intercambiar
            }
        }

        // Poner el pivote en su lugar correcto
        std::swap(vec[i + 1], vec[high]);
        return (i + 1); // Retornar el índice del pivote
    }

    /**
     * @brief Helper recursivo de Quicksort.
     * @param vec El vector a ordenar.
     * @param low El índice inicial del sub-arreglo.
     * @param high El índice final del sub-arreglo.
     * @note Esta función implementa la lógica de dividir y conquistar.
     * Las complejidades listadas en 'quicksort(vec)' reflejan
     * el comportamiento agregado de estas llamadas recursivas.
     */
    template <typename T>
    void quicksort_recursive(DS::Vector<T>& vec, int low, int high) {
        if (low < high) {
            // pi es el índice de partición, vec[pi] está ahora en el lugar correcto
            int pi = partition(vec, low, high);

            // Ordenar recursivamente los elementos antes y después de la partición
            quicksort_recursive(vec, low, pi - 1);
            quicksort_recursive(vec, pi + 1, high);
        }
    }

    /**
     * @brief Ordena un DS::Vector usando el algoritmo Quicksort.
     * @tparam T El tipo de dato (debe soportar <= y >).
     * @param vec El vector a ordenar (se modifica in-place).
     * @note Complejidad de Tiempo (Promedio/Mejor): O(n log n)
     * - Ocurre cuando el pivote divide el arreglo en mitades balanceadas.
     * - Relación de Recurrencia: T(n) = 2T(n/2) + O(n)
     *
     * @note Complejidad de Tiempo (Peor Caso): O(n^2)
     * - Ocurre con particiones desbalanceadas (ej: vector ya ordenado).
     * - Relación de Recurrencia: T(n) = T(n-1) + O(n)
     *
     * @note Complejidad de Espacio (Pila de llamadas):
     * - Promedio/Mejor: O(log n) (profundidad de recursión)
     * - Peor Caso: O(n) (profundidad de recursión)
     */
    template <typename T>
    void quicksort(DS::Vector<T>& vec) {
        if (vec.empty() || vec.size() == 1) {
            return; // Ya está ordenado
        }
        quicksort_recursive(vec, 0, static_cast<int>(vec.size()) - 1);
    }

    // -----------------------------------------------------------------
    // Mergesort
    // -----------------------------------------------------------------

    /**
     * @brief Función de mezcla (merge) para Mergesort.
     * Mezcla dos sub-vectores ordenados vec[l..m] y vec[m+1..r]
     * en un solo vector ordenado vec[l..r].
     *
     * @param vec El vector original que contiene los datos.
     * @param l Índice izquierdo (left).
     * @param m Índice medio (mid).
     * @param r Índice derecho (right).
     *
     * @note Complejidad de Tiempo: O(k), donde k es el número de elementos
     * a mezclar (k = r - l + 1).
     * @note Complejidad de Espacio: O(k), debido a la creación de los
     * vectores temporales L y R.
     */
    template <typename T>
    void merge(DS::Vector<T>& vec, int l, int m, int r) {
        // Calcular tamaños y crear vectores temporales
        size_t n1 = static_cast<size_t>(m - l + 1);
        size_t n2 = static_cast<size_t>(r - m);

        // Almacenamiento temporal
        DS::Vector<T> L(n1);
        DS::Vector<T> R(n2);

        // Copiar los datos a los vectores temporales L y R
        for (size_t i = 0; i < n1; ++i) {
            L[i] = vec[static_cast<size_t>(l) + i];
        }
        for (size_t j = 0; j < n2; ++j) {
            R[j] = vec[static_cast<size_t>(m + 1) + j];
        }

        // Mezclar (Merge) los vectores temporales de vuelta en vec[l..r]
        size_t i = 0; // Índice inicial del primer sub-vector (L)
        size_t j = 0; // Índice inicial del segundo sub-vector (R)
        size_t k = static_cast<size_t>(l); // Índice inicial del vector mezclado (vec)

        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                vec[k] = L[i];
                i++;
            } else {
                vec[k] = R[j];
                j++;
            }
            k++;
        }

        // Copiar los elementos restantes de L[], si hay alguno
        while (i < n1) {
            vec[k] = L[i];
            i++;
            k++;
        }

        // Copiar los elementos restantes de R[], si hay alguno
        while (j < n2) {
            vec[k] = R[j];
            j++;
            k++;
        }
    }

    /**
     * @brief Helper recursivo de Mergesort.
     * @param vec El vector a ordenar.
     * @param l El índice inicial del sub-arreglo (left).
     * @param r El índice final del sub-arreglo (right).
     * @note Esta función implementa la lógica de dividir y conquistar.
     * Las complejidades listadas en 'mergesort(vec)' reflejan
     * el comportamiento agregado de estas llamadas recursivas.
     */
    template <typename T>
    void mergesort_recursive(DS::Vector<T>& vec, int l, int r) {
        if (l >= r) {
            return; // Caso base: sub-arreglo de 0 o 1 elemento
        }

        // Calcular el medio (evita overflow para l+r muy grandes)
        int m = l + (r - l) / 2;

        // Dividir
        mergesort_recursive(vec, l, m);
        mergesort_recursive(vec, m + 1, r);

        // Conquistar
        merge(vec, l, m, r);
    }

    /**
     * @brief Ordena un DS::Vector usando el algoritmo Mergesort.
     * @tparam T El tipo de dato (debe soportar <=).
     * @param vec El vector a ordenar (se modifica in-place).
     *
     * @note Complejidad de Tiempo (Todos los casos): O(n log n)
     * - Relación de Recurrencia: T(n) = 2T(n/2) + O(n)
     * - A diferencia de Quicksort, Mergesort siempre divide en mitades
     * iguales, por lo que su peor caso sigue siendo O(n log n).
     *
     * @note Complejidad de Espacio: O(n)
     * - El algoritmo no es in-place. Se requiere O(n) espacio
     * adicional para los vectores temporales usados en la función 'merge'.
     * (La pila de llamadas recursivas solo ocupa O(log n)).
     */
    template <typename T>
    void mergesort(DS::Vector<T>& vec) {
        if (vec.empty() || vec.size() == 1) {
            return; // Ya está ordenado
        }
        mergesort_recursive(vec, 0, static_cast<int>(vec.size()) - 1);
    }

    // -----------------------------------------------------------------
    // Selectionsort
    // -----------------------------------------------------------------

    /**
     * @brief Ordena un DS::Vector usando el algoritmo Selectionsort.
     *
     * El algoritmo divide el vector en dos partes:
     * 1. El sub-vector ordenado (que crece desde la izquierda).
     * 2. El sub-vector desordenado (que se achica desde la derecha).
     *
     * En cada iteración, encuentra el elemento más pequeño del
     * sub-vector desordenado y lo intercambia con el primer
     * elemento de ese sub-vector (poniéndolo al final del
     * sub-vector ordenado).
     *
     * @tparam T El tipo de dato (debe soportar < y >).
     * @param vec El vector a ordenar (se modifica in-place).
     *
     * @note Complejidad de Tiempo (Todos los casos): O(n^2)
     * - El bucle externo se ejecuta n-1 veces.
     * - El bucle interno se ejecuta O(n), O(n-1), O(n-2), ... O(1) veces.
     * - Total de comparaciones: (n-1) + (n-2) + ... + 1 = n*(n-1)/2, lo cual es O(n^2).
     * - El rendimiento es el mismo sin importar si el vector
     * está ordenado, al revés o desordenado.
     *
     * @note Complejidad de Espacio: O(1)
     * - Es un algoritmo "in-place", solo requiere unas pocas
     * variables auxiliares (para los índices).
     * - Realiza exactamente n-1 intercambios (swaps).
     */
    template <typename T>
    void selectionsort(DS::Vector<T>& vec) {
        size_t n = vec.size();
        if (n <= 1) {
            return; // Ya está ordenado
        }

        // Bucle externo: recorre el límite entre la parte ordenada y la desordenada
        for (size_t i = 0; i < n - 1; ++i) {
            // Encontrar el índice del elemento mínimo
            // en la parte desordenada (desde i hasta el final)
            size_t min_idx = i;
            for (size_t j = i + 1; j < n; ++j) {
                if (vec[j] < vec[min_idx]) {
                    min_idx = j;
                }
            }

            // Intercambiar el mínimo encontrado con el primer elemento
            // de la parte desordenada (vec[i])
            //
            // (Si min_idx == i, std::swap no hace nada, lo cual es eficiente)
            std::swap(vec[i], vec[min_idx]);
        }
    }

    // -----------------------------------------------------------------
    // Insertionsort
    // -----------------------------------------------------------------

    /**
     * @brief Ordena un DS::Vector usando el algoritmo Insertionsort.
     *
     * El algoritmo construye el vector ordenado final un elemento
     * a la vez. Asume que el primer elemento (índice 0) es un
     * sub-vector ordenado.
     *
     * Luego, itera desde el segundo elemento (i=1) hasta el final.
     * Toma el elemento actual ("key") y lo "inserta" en la
     * posición correcta dentro del sub-vector ordenado (a su izquierda),
     * desplazando los elementos mayores que él hacia la derecha.
     *
     * @tparam T El tipo de dato (debe soportar >).
     * @param vec El vector a ordenar (se modifica in-place).
     *
     * @note Complejidad de Tiempo (Mejor Caso): O(n)
     * - Ocurre cuando el vector ya está ordenado.
     * - El bucle interno (while) nunca se ejecuta (la condición
     * 'vec[j] > key' es falsa inmediatamente).
     *
     * @note Complejidad de Tiempo (Peor Caso / Promedio): O(n^2)
     * - Ocurre cuando el vector está ordenado en reverso.
     * - El bucle interno tiene que desplazar i elementos
     * en cada iteración del bucle externo.
     * - Total de comparaciones: O(n^2).
     *
     * @note Complejidad de Espacio: O(1)
     * - Es un algoritmo "in-place". Solo requiere una variable
     * temporal ("key") y los índices.
     */
    template <typename T>
    void insertionsort(DS::Vector<T>& vec) {
        size_t n = vec.size();
        if (n <= 1) {
            return; // Ya está ordenado
        }

        // Empezar desde el segundo elemento (i=1),
        // asumiendo que el primero (i=0) ya está "ordenado"
        for (size_t i = 1; i < n; ++i) {
            // Guardar el elemento que vamos a insertar
            T key = vec[i];

            // j es el índice del último elemento en el sub-vector
            // ordenado (a la izquierda de 'key')
            int j = static_cast<int>(i) - 1;

            // Mover los elementos de vec[0..i-1] que sean
            // mayores que 'key', una posición a la derecha
            // para hacer espacio.
            //
            // (Usamos 'int' para j, así puede llegar a -1)
            while (j >= 0 && vec[j] > key) {
                vec[j + 1] = vec[j]; // Desplazar el elemento a la derecha
                j = j - 1;
            }

            // Insertar 'key' en su posición correcta
            // (j+1 es la posición correcta, ya sea porque j es -1
            // o porque vec[j] es <= key)
            vec[j + 1] = key;
        }
    }

    // -----------------------------------------------------------------
    // Bubblesort
    // -----------------------------------------------------------------

    /**
     * @brief Ordena un DS::Vector usando el algoritmo Bubblesort (optimizado).
     *
     * El algoritmo recorre el vector múltiples veces. En cada "pasada",
     * compara cada elemento con el siguiente (vec[j] con vec[j+1])
     * y los intercambia si están en el orden incorrecto.
     *
     * Esto causa que los elementos más "pesados" (grandes) "floten"
     * hacia el final del vector, como burbujas.
     *
     * @tparam T El tipo de dato (debe soportar >).
     * @param vec El vector a ordenar (se modifica in-place).
     *
     * @note Complejidad de Tiempo (Mejor Caso): O(n)
     * - Ocurre cuando el vector ya está ordenado.
     * - Se incluye una optimización (flag 'swapped') que detecta
     * si se realizó algún intercambio en una pasada. Si no se
     * hizo ninguno, el algoritmo termina prematuramente.
     *
     * @note Complejidad de Tiempo (Peor Caso / Promedio): O(n^2)
     * - Ocurre cuando el vector está ordenado en reverso (peor caso)
     * o en un orden aleatorio (promedio).
     * - El bucle externo se ejecuta n-1 veces, y el interno O(n) veces.
     *
     * @note Complejidad de Espacio: O(1)
     * - Es un algoritmo "in-place".
     */
    template <typename T>
    void bubblesort(DS::Vector<T>& vec) {
        size_t n = vec.size();
        if (n <= 1) {
            return; // Ya está ordenado
        }

        // Bucle externo: controla el número de pasadas
        // (Después de 'i' pasadas, los últimos 'i' elementos
        // ya están en su lugar correcto)
        for (size_t i = 0; i < n - 1; ++i) {

            // (Optimización)
            // Si en una pasada completa no se hace ningún
            // intercambio, el vector ya está ordenado.
            bool swapped = false;

            // Bucle interno: realiza las comparaciones y burbujeo
            // (n - i - 1) porque no necesitamos comparar los
            // elementos que ya están ordenados al final
            for (size_t j = 0; j < n - i - 1; ++j) {

                // Si el elemento actual es mayor que el siguiente
                if (vec[j] > vec[j + 1]) {
                    // Intercambiarlos
                    std::swap(vec[j], vec[j + 1]);
                    swapped = true;
                }
            }

            // Si no hubo intercambios en esta pasada, terminar.
            if (swapped == false) {
                break;
            }
        }
    }

    // -----------------------------------------------------------------
    // Heapsort
    // -----------------------------------------------------------------

    /**
     * @brief Helper para Heapsort. Convierte un sub-árbol en un Max-Heap.
     *
     * Esta función (también llamada "sift-down" o "bubble-down")
     * asume que los sub-árboles (izquierdo y derecho) del nodo 'i'
     * ya son Max-Heaps. Se encarga de "hundir" el elemento en 'i'
     * a su posición correcta para que el árbol completo con raíz 'i'
     * cumpla la propiedad de Max-Heap.
     *
     * @param vec El vector (tratado como un árbol).
     * @param n El tamaño del heap (no necesariamente del vector).
     * @param i El índice del nodo raíz del sub-árbol a "heapificar".
     *
     * @note Complejidad de Tiempo: O(log n)
     * - En el peor caso, el elemento debe "hundirse" hasta el
     * fondo del árbol, lo cual es la altura del árbol (log n).
     *
     * @note Complejidad de Espacio: O(1)
     * - Esta versión es iterativa, por lo que no usa espacio
     * extra en la pila de llamadas.
     */
    template <typename T>
    void heapify(DS::Vector<T>& vec, size_t n, size_t i) {

        // Bucle iterativo (en lugar de recursivo) para O(1) en espacio
        while (true) {
            size_t largest = i; // Inicializar 'largest' como la raíz 'i'

            // Índices de los hijos
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;

            // Encontrar el elemento más grande entre la raíz (i),
            // el hijo izquierdo (left) y el hijo derecho (right).
            // (Asegurándose de que los hijos estén dentro
            // de los límites del heap 'n')

            // Si el hijo izquierdo es más grande que la raíz
            if (left < n && vec[left] > vec[largest]) {
                largest = left;
            }

            // Si el hijo derecho es más grande que el 'largest' actual
            if (right < n && vec[right] > vec[largest]) {
                largest = right;
            }

            // Si 'largest' sigue siendo 'i', el sub-árbol
            // ya cumple la propiedad de Max-Heap y terminamos.
            if (largest == i) {
                break;
            }

            // Si no, intercambiamos la raíz con el hijo más grande
            std::swap(vec[i], vec[largest]);

            // Y continuamos el proceso de "hundimiento"
            // desde la nueva posición del nodo (que ahora está en 'largest')
            i = largest;
        }
    }


    /**
     * @brief Ordena un DS::Vector usando el algoritmo Heapsort.
     *
     * El algoritmo funciona en dos fases:
     * 1. Fase "Build-Heap": Reorganiza el vector completo para
     * que cumpla la propiedad de un Max-Heap (O(n)).
     * 2. Fase "Sort-Down": Extrae repetidamente el elemento
     * más grande (que siempre está en la raíz, vec[0]),
     * lo mueve al final del vector (su posición ordenada)
     * y re-heapifica el resto (O(n log n)).
     *
     * @tparam T El tipo de dato (debe soportar > y <).
     * @param vec El vector a ordenar (se modifica in-place).
     *
     * @note Complejidad de Tiempo (Todos los casos): O(n log n)
     * - Fase 1 (Build-Heap) es O(n).
     * - Fase 2 (Sort-Down) es n llamadas a 'heapify' (O(log n)).
     * - Total: O(n) + O(n log n) = O(n log n).
     * - A diferencia de Quicksort, no tiene un peor caso de O(n^2).
     *
     * @note Complejidad de Espacio: O(1)
     * - Es un algoritmo "in-place". (Usamos una versión iterativa
     * de 'heapify' para garantizar O(1) en lugar de O(log n)
     * por la pila de recursión).
     */
    template <typename T>
    void heapsort(DS::Vector<T>& vec) {
        size_t n = vec.size();
        if (n <= 1) {
            return; // Ya está ordenado
        }

        // Primero construir el Max-Heap
        // Empezamos desde el último nodo NO-hoja (n / 2 - 1)
        // y "heapificamos" hacia arriba hasta la raíz (0).
        // (Usamos 'int' para que el bucle termine en 0)
        for (int i = static_cast<int>(n / 2) - 1; i >= 0; --i) {
            heapify(vec, n, static_cast<size_t>(i));
        }

        // Ahora, extraer elementos del heap uno por uno ---
        // (En orden, del más grande al más pequeño)
        for (int i = static_cast<int>(n) - 1; i > 0; --i) {

            // Mover la raíz actual (el elemento más grande)
            // al final del vector (a su posición ordenada).
            std::swap(vec[0], vec[i]);

            // Llamar a heapify en el heap *reducido* (tamaño 'i')
            // para restaurar la propiedad de Max-Heap en la raíz (0).
            heapify(vec, static_cast<size_t>(i), 0);
        }
    }

    // -----------------------------------------------------------------
    // Radixsort
    // -----------------------------------------------------------------

    /**
     * @brief Encuentra el valor máximo en un DS::Vector de enteros.
     * @param vec El vector en el que buscar.
     * @return El valor entero más grande.
     * @note Complejidad de Tiempo: O(n)
     * @note Complejidad de Espacio: O(1)
     */
    inline int get_max(DS::Vector<int>& vec) {
        if (vec.empty()) {
            return 0;
        }
        int max_val = vec[0];
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i] > max_val) {
                max_val = vec[i];
            }
        }
        return max_val;
    }

    /**
     * @brief Ordena un DS::Vector de enteros no negativos usando Radixsort (LSD).
     *
     * Este es un algoritmo de ordenamiento no comparativo. Funciona
     * procesando los números dígito por dígito, desde el menos
     * significativo (LSD) hasta el más significativo (MSD).
     *
     * En cada pasada (para cada dígito), distribuye los números
     * en 10 "cubetas" (una para cada dígito, 0-9) y luego los
     * recoge en orden. Las cubetas se implementan como Colas (Queues)
     * para asegurar la estabilidad (números con el mismo dígito
     * mantienen su orden relativo de la pasada anterior).
     *
     * @param vec El vector de enteros no negativos a ordenar.
     *
     * @note Complejidad de Tiempo: O(d * (n + k))
     * - n = número de elementos en el vector.
     * - d = número de dígitos del número máximo
     * - k = la base de los números (10, en este caso).
     * - Dado que k es constante (10), la complejidad
     * se simplifica a O(d * n)
     *
     * @note Complejidad de Espacio: O(n + k)
     * - Se necesita O(k) espacio para las 10 cubetas (vector de colas).
     * - Se necesita O(n) espacio total para almacenar los
     * elementos dentro de esas cubetas durante la fase de distribución.
     */
    inline void radixsort(DS::Vector<int>& vec) {
        size_t n = vec.size();
        if (n <= 1) {
            return; // Ya está ordenado
        }

        // Radixsort (esta implementación) solo funciona con no-negativos
        for (size_t i = 0; i < n; ++i) {
            if (vec[i] < 0) {
                throw std::invalid_argument("radixsort(): El vector no puede contener números negativos.");
            }
        }

        // Encontrar el número máximo para saber cuántos dígitos
        // (cuántas pasadas) necesitamos.
        int maxVal = get_max(vec);

        // Crear los 10 buckets
        DS::Vector<DS::Queue<int>> buckets(10);

        // Bucle principal: una pasada por cada dígito (exponente 1, 10, 100...)
        // (El bucle se detiene cuando 'exp' es mayor que el número máximo)
        for (long long exp = 1; maxVal / exp > 0; exp *= 10) {

            // --- Distribución (Scatter) ---
            // Colocar cada número del vector en el bucket correcto
            // basándose en el dígito actual (vec[i] / exp) % 10
            for (size_t i = 0; i < n; ++i) {
                int digit = (vec[i] / exp) % 10;
                buckets[digit].push(vec[i]); // Encolar el bucket
            }

            // --- Recolección (Gather) ---
            // Recoger los elementos de los bucket (en orden de bucket, 0 a 9)
            // y ponerlos de vuelta en el vector original.
            size_t vec_idx = 0; // Índice para re-llenar 'vec'
            for (size_t b = 0; b < 10; ++b) {

                // Vaciar cada bucket (que es una cola FIFO)
                while (!buckets[b].empty()) {
                    vec[vec_idx] = buckets[b].front();
                    buckets[b].pop();
                    vec_idx++;
                }
            }
            // 'vec' está ahora ordenado por el dígito actual
            // Se repite el proceso para el siguiente dígito
        }
    }

}

#endif //SORTING_H