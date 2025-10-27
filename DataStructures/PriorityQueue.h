
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Vector.h"
#include <stdexcept>

namespace DS {

    /**
     * @class PriorityQueue
     * @brief Implementación de una Cola de Prioridad (Min-Heap)
     * @tparam T El tipo de dato a almacenar.
     *
     * Utiliza DS::Vector como contenedor subyacente.
     * Mantiene el elemento más pequeño en el tope.
     */
    template <typename T>
    class PriorityQueue {
    public:
        // -----------------------------------------------------------------
        // Métodos Públicos Principales
        // -----------------------------------------------------------------

        /**
         * @brief Verifica si la cola de prioridad está vacía.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return heap_data_.empty();
        }

        /**
         * @brief Retorna el número de elementos.
         * @note Complejidad: O(1)
         */
        size_t size() const {
            return heap_data_.size();
        }

        /**
         * @brief Retorna una referencia al elemento tope (el mínimo).
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        T& top() {
            if (empty()) {
                throw std::out_of_range("PriorityQueue::top(): Llamado en PriorityQueue vacía.");
            }
            return heap_data_.front(); // El mínimo siempre está en la raíz [0]
        }

        /**
         * @brief Retorna una referencia constante al elemento tope (el mínimo).
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        const T& top() const {
            if (empty()) {
                throw std::out_of_range("PriorityQueue::top() (const): Llamado en PriorityQueue vacía.");
            }
            return heap_data_.front(); // El mínimo siempre está en la raíz [0]
        }

        /**
         * @brief Inserta un elemento y mantiene la propiedad del heap.
         * @param value El valor a insertar.
         * @note Complejidad: O(log n)
         */
        void push(const T& value) {
            // Añadir el elemento al final del vector
            heap_data_.push_back(value);
            // "Subir" el elemento hasta su posición correcta
            bubble_up(heap_data_.size() - 1);
        }

        /**
         * @brief Elimina el elemento tope (el mínimo) y mantiene la propiedad.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(log n)
         */
        void pop() {
            if (empty()) {
                throw std::out_of_range("PriorityQueue::pop(): Llamado en PriorityQueue vacía.");
            }

            // Mover el último elemento a la raíz
            std::swap(heap_data_.front(), heap_data_.back());

            // Eliminar el último elemento (que ahora es el min)
            heap_data_.pop_back();

            // Si la cola no quedó vacía, "bajar" la nueva raíz
            if (!empty()) {
                bubble_down(0);
            }
        }

    private:
        DS::Vector<T> heap_data_; // El vector que almacena los datos del heap

        // -----------------------------------------------------------------
        // Funciones Helper Privadas (la lógica del heap)
        // -----------------------------------------------------------------

        /**
         * @brief Mueve un elemento hacia arriba para restaurar la propiedad del heap.
         * (Usado después de push)
         * @param index El índice del elemento a "subir".
         * @note Complejidad: O(log n)
         */
        void bubble_up(size_t index) {
            // Mientras no seamos la raíz (index > 0) y seamos más pequeños que nuestro padre
            // NOTA: Usamos > para un Min-Heap (el padre debe ser <= al hijo)
            while (index > 0 && heap_data_[index] < heap_data_[parent(index)]) {
                // Intercambiar con el padre
                std::swap(heap_data_[index], heap_data_[parent(index)]);
                // Movernos al índice del padre para seguir subiendo
                index = parent(index);
            }
        }

        /**
         * @brief Mueve un elemento hacia abajo para restaurar la propiedad del heap.
         * (Usado después de pop)
         * @param index El índice del elemento a "bajar".
         * @note Complejidad: O(log n)
         */
        void bubble_down(size_t index) {
            size_t min_index = index;

            while (true) {
                size_t left = left_child(index);
                size_t right = right_child(index);
                size_t N = heap_data_.size();

                // Encontrar el índice más pequeño (padre, hijo izq, hijo der)
                // NOTA: Usamos < para un Min-Heap

                // Si el hijo izquierdo existe y es más pequeño que el actual
                if (left < N && heap_data_[left] < heap_data_[min_index]) {
                    min_index = left;
                }

                // Si el hijo derecho existe y es más pequeño que el (hasta ahora) mínimo
                if (right < N && heap_data_[right] < heap_data_[min_index]) {
                    min_index = right;
                }

                // Si 'index' ya es el más pequeño, la propiedad se cumple
                if (min_index == index) {
                    break;
                }

                // Si no, intercambiar con el hijo más pequeño y repetir
                std::swap(heap_data_[index], heap_data_[min_index]);
                index = min_index;
            }
        }

        // -----------------------------------------------------------------
        // Helpers para índices
        // -----------------------------------------------------------------

        /**
         * @brief Calcula el índice del padre de un nodo.
         * @param i Índice del nodo actual.
         * @return Índice del padre.
         * @note Complejidad: O(1)
         */
        size_t parent(size_t i) {
            return (i - 1) / 2;
        }

        /**
         * @brief Calcula el índice del hijo izquierdo de un nodo.
         * @param i Índice del nodo actual.
         * @return Índice del hijo izquierdo.
         * @note Complejidad: O(1)
         */
        size_t left_child(size_t i) {
            return 2 * i + 1;
        }

        /**
         * @brief Calcula el índice del hijo derecho de un nodo.
         * @param i Índice del nodo actual.
         * @return Índice del hijo derecho.
         * @note Complejidad: O(1)
         */
        size_t right_child(size_t i) {
            return 2 * i + 2;
        }
    };

}

#endif //PRIORITY_QUEUE_H
