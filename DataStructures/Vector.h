
#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <utility>

namespace DS {

    /**
     * @class Vector
     * @brief Implementación de un array dinámico (Vector) genérico.
     * @tparam T El tipo de dato que almacenará el vector.
     *
     * Gestiona un bloque de memoria contiguo y se redimensiona
     * automáticamente (con crecimiento exponencial) cuando es necesario.
     */
    template <typename T>
    class Vector {
    public:
        // --- Typedefs de Iterador ---
        // Un iterador para un vector contiguo es simplemente un puntero.
        using iterator = T*;
        using const_iterator = const T*;

        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea un vector vacío sin capacidad.
         * @note Complejidad: O(1)
         */
        Vector() : data_(nullptr), size_(0), capacity_(0) {}

        /**
         * @brief Constructor que crea un vector con 'size' elementos
         * inicializados por defecto.
         * @param size El número inicial de elementos.
         * @note Complejidad: O(n)
         */
        explicit Vector(size_t size)
            : data_(new T[size]{}), size_(size), capacity_(size) {
            // new T[size] llama al constructor por defecto de T para cada elemento
        }

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada.
         * @note Complejidad: O(n) (llama al destructor de cada T)
         */
        ~Vector() {
            delete[] data_;
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * @param other El vector a copiar.
         * @note Complejidad: O(n)
         */
        Vector(const Vector& other)
            : data_(new T[other.capacity_]), // Asignar nueva memoria
              size_(other.size_),
              capacity_(other.capacity_) {

            // Copiar los elementos
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * @param other El vector a asignar.
         * @return Vector& Una referencia a *this.
         * @note Complejidad: O(n)
         */
        Vector& operator=(const Vector& other) {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Crear una copia temporal (usa el constructor de copia)
            Vector temp(other);
            // Intercambiar los recursos internos con la copia temporal
            swap(temp);
            // 'temp' se destruye aquí, liberando la memoria antigua de *this
            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * "Roba" los recursos de 'other'.
         * @param other El vector (r-value) del que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        Vector(Vector&& other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {

            // Dejar 'other' en un estado nulo y válido
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * @param other El vector (r-value) a asignar.
         * @return Vector& Una referencia a *this.
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        Vector& operator=(Vector&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            // Liberar los recursos propios
            delete[] data_;

            // Transferir la propiedad de 'other' a 'this'
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            // Deja 'other' en un estado nulo y válido
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;

            return *this;
        }

        // -----------------------------------------------------------------
        // Modificadores
        // -----------------------------------------------------------------

        /**
         * @brief Añade un elemento al final del vector (copia).
         * @param value El valor a añadir.
         * @note Complejidad: O(1) amortizado (O(n) si ocurre reasignación).
         */
        void push_back(const T& value) {
            if (size_ == capacity_) {
                // Si estamos llenos, duplicar la capacidad (o empezar en 8)
                reserve((capacity_ == 0) ? 8 : capacity_ * 2);
            }
            data_[size_] = value;
            size_++;
        }

        /**
         * @brief Añade un elemento al final del vector (movimiento).
         * @param value El valor (r-value) a mover.
         * @note Complejidad: O(1) amortizado (O(n) si ocurre reasignación).
         */
        void push_back(T&& value) {
            if (size_ == capacity_) {
                reserve((capacity_ == 0) ? 8 : capacity_ * 2);
            }
            data_[size_] = std::move(value); // Mover el valor
            size_++;
        }

        /**
         * @brief Elimina el último elemento del vector.
         * Llama explícitamente al destructor del elemento T antes
         * de decrementar el tamaño lógico del vector.
         * @note Complejidad: O(1)
         */
        void pop_back() {
            if (size_ > 0) {
                // Llamar al destructor del último elemento
                // para liberar sus recursos (si T es un objeto complejo).
                data_[size_ - 1].~T();

                // Decrementar el tamaño lógico
                size_--;
            }
        }

        /**
         * @brief Solicita que la capacidad del vector sea al menos 'new_capacity'.
         * Si 'new_capacity' es mayor que la capacidad actual,
         * se reasigna la memoria.
         * @param new_capacity La capacidad mínima deseada.
         * @note Complejidad: O(n) si hay reasignación.
         */
        void reserve(size_t new_capacity) {
            if (new_capacity <= capacity_) {
                return;
            }

            // Asignar nuevo bloque de memoria
            T* new_data = new T[new_capacity]{};

            // Mover los elementos antiguos al nuevo bloque
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }

            // Liberar el bloque antiguo
            delete[] data_;

            // Actualizar punteros y capacidad
            data_ = new_data;
            capacity_ = new_capacity;
        }

        /**
         * @brief Cambia el tamaño ('size_') del vector.
         * Si el nuevo tamaño es mayor, los elementos extra se inicializan
         * por defecto. Si es menor, se trunca.
         * @param new_size El nuevo tamaño deseado.
         * @note Complejidad: O(n) si hay reasignación.
         */
        void resize(size_t new_size) {
            if (new_size > capacity_) {
                // Si el nuevo tamaño excede la capacidad, reasignar
                reserve(new_size);
                // Los nuevos elementos (de size_ a new_size) ya fueron
                // inicializados por defecto por 'new T[new_size]'
            }
            // Si new_size > size_ pero <= capacity_, los elementos
            // (de size_ a new_size) ya están inicializados por defecto
            // desde la asignación original

            size_ = new_size;
        }

        /**
         * @brief Elimina todos los elementos del vector (pone size_ a 0).
         * No libera la memoria (la capacidad sigue siendo la misma).
         * @note Complejidad: O(1)
         */
        void clear() {
            size_ = 0;
        }

        // -----------------------------------------------------------------
        // Acceso a Elementos
        // -----------------------------------------------------------------

        /**
         * @brief Acceso a elemento por índice (sin verificación de límites).
         * @param index El índice.
         * @return T& Referencia al elemento.
         * @note Complejidad: O(1)
         */
        T& operator[](size_t index) {
            return data_[index];
        }

        /**
         * @brief Acceso constante a elemento (sin verificación).
         * @param index El índice.
         * @return const T& Referencia constante al elemento.
         * @note Complejidad: O(1)
         */
        const T& operator[](size_t index) const {
            return data_[index];
        }

        /**
         * @brief Acceso a elemento por índice (con verificación de límites).
         * @param index El índice.
         * @return T& Referencia al elemento.
         * @throws std::out_of_range si index >= size().
         * @note Complejidad: O(1)
         */
        T& at(size_t index) {
            if (index >= size_) {
                throw std::out_of_range("Vector::at(): índice fuera de rango.");
            }
            return data_[index];
        }

        /**
         * @brief Acceso constante a elemento (con verificación).
         * @param index El índice.
         * @return const T& Referencia constante al elemento.
         * @throws std::out_of_range si index >= size().
         * @note Complejidad: O(1)
         */
        const T& at(size_t index) const {
            if (index >= size_) {
                throw std::out_of_range("Vector::at() (const): índice fuera de rango.");
            }
            return data_[index];
        }

        /**
         * @brief Obtiene una referencia al primer elemento.
         * @note Complejidad: O(1)
         */
        T& front() {
            return data_[0];
        }

        /**
         * @brief Obtiene una referencia al último elemento.
         * @note Complejidad: O(1)
         */
        T& back() {
            return data_[size_ - 1];
        }

        /**
         * @brief Compara la igualdad entre dos vectores.
         * @note Complejidad: O(n)
         */
        bool operator==(const Vector& other) const {
            if (size_ != other.size_) {
                return false;
            }
            for (size_t i = 0; i < size_; ++i) {
                if (data_[i] != other.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Compara la desigualdad entre dos vectores.
         * @note Complejidad: O(n)
         */
        bool operator!=(const Vector& other) const {
            return !(*this == other);
        }

        // -----------------------------------------------------------------
        // Capacidad
        // -----------------------------------------------------------------

        /**
         * @brief Retorna el número de elementos en uso.
         * @return size_t El número de elementos.
         * @note Complejidad: O(1)
         */
        size_t size() const {
            return size_;
        }

        /**
         * @brief Retorna la capacidad total de memoria asignada.
         * @return size_t La capacidad.
         * @note Complejidad: O(1)
         */
        size_t capacity() const {
            return capacity_;
        }

        /**
         * @brief Verifica si el vector está vacío.
         * @return true si size_ == 0, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return size_ == 0;
        }

        // -----------------------------------------------------------------
        // Métodos de Iterador
        // -----------------------------------------------------------------

        /**
         * @brief Retorna un iterador al principio.
         * @note Complejidad: O(1)
         */
        iterator begin() {
            return data_; // Puntero al primer elemento
        }

        /**
         * @brief Retorna un iterador al final (uno después del último).
         * @note Complejidad: O(1)
         */
        iterator end() {
            return data_ + size_; // Puntero uno después del último
        }

        /**
         * @brief Retorna un iterador constante al principio.
         * @note Complejidad: O(1)
         */
        const_iterator begin() const {
            return data_;
        }

        /**
         * @brief Retorna un iterador constante al final.
         * @note Complejidad: O(1)
         */
        const_iterator end() const {
            return data_ + size_;
        }

        /**
         * @brief Retorna un iterador constante al principio (cbegin).
         * @note Complejidad: O(1)
         */
        const_iterator cbegin() const {
            return data_;
        }

        /**
         * @brief Retorna un iterador constante al final (cend).
         * @note Complejidad: O(1)
         */
        const_iterator cend() const {
            return data_ + size_;
        }

    private:
        T* data_;           // Puntero al bloque de memoria en el heap
        size_t size_;       // Número de elementos actualmente en uso
        size_t capacity_;   // Número total de elementos que caben en data_

        /**
         * @brief Función auxiliar para intercambiar el estado de dos vectores.
         * @param other El otro vector con el que intercambiar.
         * @note Complejidad: O(1)
         */
        void swap(Vector& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
        }
    };

}

#endif //VECTOR_H