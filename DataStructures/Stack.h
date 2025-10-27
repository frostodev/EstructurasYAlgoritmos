#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <utility>

namespace DS {

    /**
     * @class Stack
     * @brief Implementación de una Pila (LIFO - Last-In, First-Out) genérica.
     * @tparam T El tipo de dato que almacenará la pila.
     *
     * Esta estructura de datos restringe la inserción (push) y eliminación (pop)
     * a un solo extremo, llamado "tope" (top).
     * Se implementa internamente como una lista enlazada simple.
     */
    template <typename T>
    class Stack {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea una pila vacía.
         * @note Complejidad: O(1)
         */
        Stack() : top_node_(nullptr), element_count_(0) {}

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada para los nodos.
         * @note Complejidad: O(n)
         */
        ~Stack() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * Crea una nueva pila como una copia profunda de 'other'.
         * Mantiene el orden original de los elementos.
         * @param other La pila a copiar.
         * @note Complejidad: O(n)
         */
        Stack(const Stack& other) : top_node_(nullptr), element_count_(0) {
            if (other.empty()) {
                return;
            }

            // Copiar el nodo tope
            top_node_ = new Node(other.top_node_->data);
            element_count_ = other.element_count_;

            // Iterar y enlazar el resto de los nodos
            Node* current_this = top_node_;
            Node* current_other = other.top_node_->next;

            while (current_other != nullptr) {
                current_this->next = new Node(current_other->data);
                current_this = current_this->next;
                current_other = current_other->next;
            }
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * @param other La pila a asignar.
         * @return Stack& Una referencia a esta pila (*this).
         * @note Complejidad: O(n)
         */
        Stack& operator=(const Stack& other) {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Crea una copia temporal y luego intercambia
            Stack temp(other);
            swap(temp);

            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * "Roba" los recursos de 'other' y la deja en estado vacío.
         * @param other La pila (r-value) de la que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        Stack(Stack&& other) noexcept
            : top_node_(other.top_node_), element_count_(other.element_count_) {
            // 'other' ya no es dueña de los nodos
            other.top_node_ = nullptr;
            other.element_count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * @param other La pila (r-value) a asignar.
         * @return Stack& Una referencia a esta pila (*this).
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        Stack& operator=(Stack&& other) noexcept {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Libera los recursos propios
            clear();

            // Transfiere la propiedad de 'other' a 'this'
            top_node_ = other.top_node_;
            element_count_ = other.element_count_;

            // Deja 'other' en un estado vacío y válido
            other.top_node_ = nullptr;
            other.element_count_ = 0;

            return *this;
        }

        // -----------------------------------------------------------------
        // Operaciones Principales de la Pila
        // -----------------------------------------------------------------

        /**
         * @brief Inserta un elemento en el tope de la pila.
         * @param value El valor a insertar.
         * @note Complejidad: O(1)
         */
        void push(const T& value) {
            Node* new_node = new Node(value);
            new_node->next = top_node_;
            top_node_ = new_node;
            element_count_++;
        }

        /**
         * @brief Elimina el elemento del tope de la pila.
         * @throws std::out_of_range si la pila está vacía.
         * @note Complejidad: O(1)
         */
        void pop() {
            if (empty()) {
                throw std::out_of_range("Stack::pop(): Llamado en una pila vacía.");
            }

            Node* node_to_delete = top_node_;
            top_node_ = top_node_->next;
            delete node_to_delete;
            element_count_--;
        }

        /**
         * @brief Obtiene una referencia al elemento en el tope de la pila.
         * @return T& Referencia al dato en el tope.
         * @throws std::out_of_range si la pila está vacía.
         * @note Complejidad: O(1)
         */
        T& top() {
            if (empty()) {
                throw std::out_of_range("Stack::top(): Llamado en una pila vacía.");
            }
            return top_node_->data;
        }

        /**
         * @brief Obtiene una referencia constante al elemento en el tope.
         * (Versión const-correct).
         * @return const T& Referencia constante al dato en el tope.
         * @throws std::out_of_range si la pila está vacía.
         * @note Complejidad: O(1)
         */
        const T& top() const {
            if (empty()) {
                throw std::out_of_range("Stack::top() (const): Llamado en una pila vacía.");
            }
            return top_node_->data;
        }

        // -----------------------------------------------------------------
        // Métodos de Capacidad y Utilidad
        // -----------------------------------------------------------------

        /**
         * @brief Verifica si la pila está vacía.
         * @return true si la pila está vacía, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return element_count_ == 0;
            // Alternativamente: return top_node_ == nullptr;
        }

        /**
         * @brief Retorna el número de elementos en la pila.
         * @return int El número de elementos.
         * @note Complejidad: O(1)
         */
        int size() const {
            return element_count_;
        }

        /**
         * @brief Elimina todos los elementos de la pila.
         * La pila queda en estado vacío.
         * @note Complejidad: O(n)
         */
        void clear() {
            while (!empty()) {
                pop(); // pop() ya maneja la memoria y el contador
            }
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en la pila.
         */
        struct Node {
            T data;
            Node* next; // Puntero al siguiente elemento (el que está "debajo")

            explicit Node(const T& val) : data(val), next(nullptr) {}
        };

        Node* top_node_;      // Puntero al nodo en el tope de la pila.
        int element_count_;   // Número de elementos en la pila (para O(1) get_size).

        /**
         * @brief Función auxiliar para intercambiar el estado de dos pilas.
         * @param other La otra pila con la que intercambiar el contenido.
         * @note Complejidad: O(1)
         */
        void swap(Stack& other) noexcept {
            // Intercambia los punteros y contadores
            std::swap(top_node_, other.top_node_);
            std::swap(element_count_, other.element_count_);
        }

    };

}

#endif // STACK_H
