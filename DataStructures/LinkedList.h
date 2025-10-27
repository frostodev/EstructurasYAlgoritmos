#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <stdexcept>

namespace DS {

    /**
     * @class LinkedList
     * @brief Implementación de una lista simplemente enlazada genérica.
     * @tparam T El tipo de dato que almacenará la lista.
     *
     * Esta clase gestiona una colección de elementos donde cada nodo
     * mantiene un puntero únicamente al nodo siguiente, permitiendo
     * inserciones y eliminaciones eficientes O(1) al inicio de la lista.
     * Sin embargo, las operaciones al final pueden requerir un recorrido
     * completo O(n) debido a la ausencia de puntero al nodo anterior.
     */
    template <typename T>
    class LinkedList {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea una lista vacía.
         * @note Complejidad: O(1)
         */
        LinkedList() : head_(nullptr), tail_(nullptr), count_(0) {}

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada para los nodos.
         * @note Complejidad: O(n)
         */
        ~LinkedList() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * Crea una nueva lista como una copia profunda de 'other'.
         * @param other La lista a copiar.
         * @note Complejidad: O(n)
         */
        LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), count_(0) {
            // Recorre la otra lista y añade cada elemento a esta
            Node* current = other.head_;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }

        /**
         * @brief Operador de asignación por copia.
         * Sobrescribe esta lista con una copia profunda de 'other'.
         * @param other La lista a copiar.
         * @return Referencia a esta lista (*this) después de la asignación.
         * @note Complejidad: O(n + m) donde n es el tamaño de this y m el de other.
         */
        LinkedList& operator=(const LinkedList& other) {
            if (this == &other) {
                // Protección contra auto-asignación (list = list)
                return *this;
            }

            // Borrar el contenido actual de esta lista
            clear();

            // Copiar los elementos de 'other'
            Node* current = other.head_;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }

            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * "Roba" los recursos (punteros y tamaño) de 'other'.
         * Deja 'other' en un estado vacío y válido.
         * @param other La lista de la cual mover los recursos (será dejada vacía)
         * @note Complejidad: O(1)
         */
        LinkedList(LinkedList&& other) noexcept
            : head_(other.head_), tail_(other.tail_), count_(other.count_) {
            // 'other' ya no es dueño de los nodos
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * Libera los recursos de esta lista y "roba" los de 'other'.
         * @param other La lista de la cual mover los recursos (será dejada vacía).
         * @return Referencia a esta lista (*this) después de la asignación.
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        LinkedList& operator=(LinkedList&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            // Liberar los recursos propios
            clear();

            // Transferir la propiedad de 'other' a 'this'
            head_ = other.head_;
            tail_ = other.tail_;
            count_ = other.count_;

            // Dejar 'other' en un estado vacío y válido
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.count_ = 0;

            return *this;
        }

        // -----------------------------------------------------------------
        // Métodos de Capacidad
        // -----------------------------------------------------------------

        /**
         * @brief Retorna el número de elementos en la lista.
         * @return int El número de elementos en la lista.
         * @note Complejidad: O(1)
         */
        int get_size() const {
            return count_;
        }

        /**
         * @brief Verifica si la lista está vacía.
         * @return true si la lista está vacía, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool is_empty() const {
            return count_ == 0;
        }

        // -----------------------------------------------------------------
        // Métodos de Inserción
        // -----------------------------------------------------------------

        /**
         * @brief Añade un elemento al inicio de la lista.
         * @param value El valor a añadir al inicio.
         * @note Complejidad: O(1)
         */
        void push_front(const T& value) {
            Node* new_node = new Node(value);
            if (is_empty()) {
                // Si la lista está vacía, el nuevo nodo es cabeza y cola
                head_ = new_node;
                tail_ = new_node;
            } else {
                // El nuevo nodo apunta a la antigua cabeza
                new_node->next = head_;
                // El nuevo nodo se convierte en la nueva cabeza
                head_ = new_node;
            }
            count_++;
        }

        /**
         * @brief Añade un elemento al final de la lista.
         * Gracias al puntero 'tail_', esto es O(1).
         * @param value El valor a añadir al final.
         * @note Complejidad: O(1)
         */
        void push_back(const T& value) {
            Node* new_node = new Node(value);
            if (is_empty()) {
                // Si la lista está vacía, el nuevo nodo es cabeza y cola
                head_ = new_node;
                tail_ = new_node;
            } else {
                // La antigua cola apunta al nuevo nodo
                tail_->next = new_node;
                // El nuevo nodo se convierte en la nueva cola
                tail_ = new_node;
            }
            count_++;
        }

        /**
         * @brief Inserta un elemento en un índice específico.
         * @param index El índice donde insertar (0 <= index <= get_size()).
         * @param value El valor a insertar.
         * @throws std::out_of_range si el índice está fuera de límites.
         * @note Complejidad: O(n) en el peor caso (insertar cerca del final).
         */
        void insert_at(int index, const T& value) {
            if (index < 0 || index > count_) {
                throw std::out_of_range("LinkedList::insert_at(): Índice fuera de rango.");
            }

            if (index == 0) {
                push_front(value);
            } else if (index == count_) {
                push_back(value);
            } else {
                // Encontrar el nodo *anterior* al índice
                Node* prev = head_;
                for (int i = 0; i < index - 1; ++i) {
                    prev = prev->next;
                }
                Node* new_node = new Node(value);
                new_node->next = prev->next; // El nuevo nodo apunta al nodo en 'index'
                prev->next = new_node;       // El nodo 'prev' apunta al nuevo nodo
                count_++;
            }
        }

        // -----------------------------------------------------------------
        // Métodos de Eliminación
        // -----------------------------------------------------------------

        /**
         * @brief Elimina el primer elemento de la lista.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(1)
         */
        void pop_front() {
            if (is_empty()) {
                throw std::out_of_range("LinkedList::pop_front(): La lista está vacía.");
            }

            Node* node_to_delete = head_;
            head_ = head_->next; // Mueve la cabeza al siguiente
            delete node_to_delete;
            count_--;

            if (is_empty()) {
                // Si la lista quedó vacía, tail_ también debe ser null
                tail_ = nullptr;
            }
        }

        /**
         * @brief Elimina el último elemento de la lista.
         * En una lista *simplemente* enlazada, esto es O(n)
         * porque necesitamos encontrar el *penúltimo* nodo.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(n)
         */
        void pop_back() {
            if (is_empty()) {
                throw std::out_of_range("LinkedList::pop_back(): La lista está vacía.");
            }

            if (count_ == 1) { // o head_ == tail_
                // Caso especial: solo hay un elemento
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;
            } else {
                // Encontrar el penúltimo nodo
                Node* current = head_;
                while (current->next != tail_) {
                    current = current->next;
                }

                delete tail_;        // Borra la antigua cola
                tail_ = current;     // El penúltimo nodo es la nueva cola
                tail_->next = nullptr; // Asegura que la nueva cola apunte a null
            }
            count_--;
        }

        /**
         * @brief Elimina un elemento en un índice específico.
         * @param index El índice del elemento a eliminar (0 <= index < get_size()).
         * @throws std::out_of_range si el índice está fuera de rango
         * @note Complejidad: O(n) en el peor caso.
         */
        void remove_at(int index) {
            if (index < 0 || index >= count_) {
                throw std::out_of_range("LinkedList::remove_at(): Índice fuera de rango.");
            }

            if (index == 0) {
                pop_front();
            } else if (index == count_ - 1) {
                pop_back(); // Sigue siendo O(n), así que está bien
            } else {
                // Encontrar el nodo anterior al que se va a borrar
                Node* prev = head_;
                for (int i = 0; i < index - 1; ++i) {
                    prev = prev->next;
                }

                Node* node_to_delete = prev->next;
                prev->next = node_to_delete->next; // "Salta" el nodo a eliminar

                delete node_to_delete;
                count_--;
            }
        }

        /**
         * @brief Elimina todos los nodos de la lista.
         * @note Complejidad: O(n)
         */
        void clear() {
            Node* current = head_;
            while (current != nullptr) {
                Node* next = current->next; // Guarda el siguiente nodo
                delete current;             // Borra el nodo actual
                current = next;             // Avanza al siguiente
            }
            head_ = nullptr;
            tail_ = nullptr;
            count_ = 0;
        }

        // -----------------------------------------------------------------
        // Métodos de Acceso
        // -----------------------------------------------------------------

        /**
         * @brief Obtiene una referencia al dato en un índice específico.
         * Permite modificar el dato (ej: list.get(1) = 100;)
         * @param index El índice del elemento a obtener (0 <= index < get_size()).
         * @return T& Referencia al dato en el índice especificado.
         * @throws std::out_of_range si el índice está fuera de límites
         * @note Complejidad: O(n)
         */
        T& get(int index) {
            if (index < 0 || index >= count_) {
                throw std::out_of_range("LinkedList::get(): Índice fuera de rango.");
            }

            Node* current = head_;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            return current->data;
        }

        /**
         * @brief Obtiene una referencia constante al dato en un índice.
         * Versión const-correct para listas constantes.
         * @param index El índice del elemento a obtener (0 <= index < get_size()).
         * @return const T& Referencia constante al dato en el índice especificado.
         * @throws std::out_of_range si el índice está fuera de límites.
         * @note Complejidad: O(n)
         */
        const T& get(int index) const {
            if (index < 0 || index >= count_) {
                throw std::out_of_range("LinkedList::get(): Índice fuera de rango.");
            }

            Node* current = head_;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            return current->data;
        }

        // -----------------------------------------------------------------
        // Utilidades
        // -----------------------------------------------------------------

        /**
         * @brief Imprime el contenido de la lista en la consola.
         * Útil para depuración.
         * @note Complejidad: O(n)
         */
        void print_list() const {
            Node* current = head_;
            while (current != nullptr) {
                std::cout << current->data << " -> ";
                current = current->next;
            }
            std::cout << "nullptr" << std::endl;
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en la lista.
         * Contiene el dato y un puntero al siguiente nodo.
         */
        struct Node {
            T data;
            Node* next;

            /**
             * @brief Constructor del Nodo
             * @param val El valor (dato) a almacenar en el nodo.
             */
            explicit Node(const T& val) : data(val), next(nullptr) {}
        };

        Node* head_;  // Puntero al primer nodo de la lista
        Node* tail_;  // Puntero al último nodo (para inserciones O(1) al final)
        int count_;   // Número de elementos en la lista

    };

}

#endif //LINKED_LIST_H