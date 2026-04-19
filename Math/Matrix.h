
#ifndef MATRIX_H
#define MATRIX_H

#include "MathUtils.h"
#include "../DataStructures/Vector.h"

#include <iomanip>
#include <stdexcept>
#include <limits>

namespace Math {

    /**
     * @concept Numeric
     * @brief Restringe un tipo de plantilla T a ser un tipo aritmético (int, float, double, etc.).
     */
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * @class Matrix
     * @brief Una clase de plantilla para representación y operaciones de matrices.
     * @tparam T El tipo numérico de los elementos de la matriz (e.g., int, float, double).
     * Debe satisfacer el concept 'Numeric'.
     */
    template<Numeric T>
    class Matrix {
    public:
        /**
         * @brief Construye una matriz con las dimensiones especificadas, inicializada a cero.
         * @param rows Número de filas.
         * @param cols Número de columnas.
         * @throw std::overflow_error Si 'rows * cols' excede el límite de 'size_t'.
         * @note: Complejidad: O(rows * cols)
         */
        Matrix(size_t rows, size_t cols)
                    : rows_(rows), cols_(cols), size_(0), data_() {

            // Verificar límites inferiores de tamaño
            if (rows == 0 || cols == 0) {
                throw std::invalid_argument("Matrix(): Las dimensiones de la matriz deben ser enteros positivos.");
            }

            // Verificar límites de overflow
            if (cols != 0 && rows > std::numeric_limits<size_t>::max() / cols) {
                throw std::overflow_error("Matrix(): Dimensiones de la matriz muy grandes.");
            }

            // Si las dimensiones son válidas, asignar tamaño al vector
            size_ = rows * cols;
            data_.resize(size_);
        }

        /**
         * @brief Destructor por defecto.
         * @note: Complejidad: O(size_) = O(rows_ * cols_)
         */
        ~Matrix() = default;

        /**
         * @brief Obtiene el número de filas.
         * @return El número de filas ('rows_').
         * @note: Complejidad: O(1)
         */
        [[nodiscard]]
        size_t rows() const {
            return rows_;
        }

        /**
         * @brief Obtiene el número de columnas.
         * @return El número de columnas ('cols_').
         * @note: Complejidad: O(1)
         */
        [[nodiscard]]
        size_t cols() const {
            return cols_;
        }

        /**
         * @brief Obtiene el número total de elementos.
         * @return El tamaño total ('size_'), igual a 'rows * cols'.
         * @note: Complejidad: O(1)
         */
        [[nodiscard]]
        size_t size() const {
            return size_;
        }

        /**
         * @brief Redimensiona la matriz.
         *
         * Crea una nueva matriz con las nuevas dimensiones. Los elementos existentes
         * que caben en las nuevas dimensiones se preservan. Los nuevos elementos
         * se inicializan a cero (valor por defecto de T).
         *
         * @param new_rows El nuevo número de filas.
         * @param new_cols El nuevo número de columnas.
         * @throw std::overflow_error Si 'new_rows * new_cols' excede el límite de 'size_t'.
         * @throw std::invalid_argument Si las nuevas dimensiones no son enteros positivos.
         * @note Complejidad: O(new_rows * new_cols)
         */
        void resize(size_t new_rows, size_t new_cols) {
            // Verificar límites
            if (new_cols != 0 && new_rows > std::numeric_limits<size_t>::max() / new_cols) {
                throw std::overflow_error("Matrix::resize(): Dimensiones de la matriz muy grandes.");
            }

            if (new_rows <= 0 || new_cols <= 0) {
                throw std::invalid_argument("Matrix::resize(): Las nuevas dimensiones de la matriz deben ser enteros positivos.");
            }

            DS::Vector<T> new_data(new_rows * new_cols);

            // Calcular hasta donde copiar
            size_t min_rows = Math::min(rows_, new_rows);
            size_t min_cols = Math::min(cols_, new_cols);

            for (size_t i = 0; i < min_rows; ++i) {
                for (size_t j = 0; j < min_cols; ++j) {
                    new_data[i * new_cols + j] = (*this)(i, j);
                }
            }

            // Reemplazar miembros internos
            rows_ = new_rows;
            cols_ = new_cols;
            size_ = new_rows * new_cols;
            data_ = std::move(new_data); // Mover los nuevos datos
        }

        /**
         * @brief Establece todos los elementos de la matriz a cero (T{})
         * @note Esta función no cambia las dimensiones de la matriz.
         * @note Complejidad O(size_) = O(rows_ * cols_)
         */
        void empty() {
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = T{}; // T{} es el valor "cero"
            }
        }

        /**
         * @brief Accede al elemento de la matriz en la posición (i, j).
         *
         * Proporciona acceso con comprobación de límites.
         * Lanza una excepción si los índices están fuera del rango válido.
         *
         * @param i Índice de la fila (0 ≤ i < número de filas).
         * @param j Índice de la columna (0 ≤ j < número de columnas).
         * @return Referencia al elemento en la posición (i, j).
         *
         * @throw std::out_of_range Si i o j están fuera de los límites de la matriz.
         * @note Complejidad: O(1).
         */
        T& at(size_t i, size_t j) {
            if (i >= rows_ || j >= cols_) {
                throw std::out_of_range("Matrix::at(): Índices fuera de rango.");
            }
            return data_[i * cols_ + j];
        }

        /**
         * @brief Accede al elemento de la matriz en la posición (i, j) (versión constante).
         *
         * Proporciona acceso de solo lectura con comprobación de límites.
         * Lanza una excepción si los índices están fuera del rango válido.
         *
         * @param i Índice de la fila (0 ≤ i < número de filas).
         * @param j Índice de la columna (0 ≤ j < número de columnas).
         * @return Referencia constante al elemento en la posición (i, j).
         *
         * @throw std::out_of_range Si i o j están fuera de los límites de la matriz.
         */
        [[nodiscard]]
        const T& at(size_t i, size_t j) const {
            if (i >= rows_ || j >= cols_) {
                throw std::out_of_range("Matrix::at(): Índices fuera de rango.");
            }
            return data_[i * cols_ + j];
        }

        // -----------------------------------------------------------------
        // Operaciones Matemáticas
        // -----------------------------------------------------------------

        /**
         * @brief Operación Elemental 1 (Fila): Intercambia dos filas.
         * R_i <-> R_j
         * @param r1 El índice de la primera fila.
         * @param r2 El índice de la segunda fila.
         * @throw std::out_of_range Si los índices de fila son inválidos.
         * @note Complejidad: O(cols_)
         */
        void swap_rows(size_t r1, size_t r2) {
            if (r1 >= rows_ || r2 >= rows_) {
                throw std::out_of_range("Matrix::swap_rows(): Índice de fila fuera de rango.");
            }
            if (r1 == r2) {
                return; // No hacer nada
            }

            // Iterar por cada columna en estas filas
            for (size_t j = 0; j < cols_; ++j) {
                std::swap((*this)(r1, j), (*this)(r2, j));
            }
        }

        /**
         * @brief Operación Elemental 2 (Fila): Multiplica una fila por un escalar.
         * R_i = k * R_i
         * @param r El índice de la fila a escalar.
         * @param scalar El escalar (no debe ser cero si se usa para invertir).
         * @throw std::out_of_range Si el índice de fila es inválido.
         * @note Complejidad: O(cols_)
         */
        void scale_row(size_t r, T scalar) {
            if (r >= rows_) {
                throw std::out_of_range("Matrix::scale_row(): Índice de fila fuera de rango.");
            }

            // if (scalar == T{}) {
            //    throw std::invalid_argument("El escalar no puede ser cero");
            // }

            for (size_t j = 0; j < cols_; ++j) {
                (*this)(r, j) *= scalar;
            }
        }

        /**
         * @brief Operación Elemental 3 (Fila): Suma un múltiplo de una fila a otra.
         * R_target = R_target + (scalar * R_source)
         * @param r_target La fila que será modificada.
         * @param r_source La fila que se usará como fuente.
         * @param scalar El escalar para multiplicar la fila fuente.
         * @throw std::out_of_range Si los índices de fila son inválidos.
         * @note Complejidad: O(cols_)
         */
        void add_scaled_row(size_t r_target, size_t r_source, T scalar) {
            if (r_target >= rows_ || r_source >= rows_) {
                throw std::out_of_range("Matrix::add_scaled_row(): Índice de fila fuera de rango.");
            }

            for (size_t j = 0; j < cols_; ++j) {
                (*this)(r_target, j) += (*this)(r_source, j) * scalar;
            }
        }

        /**
         * @brief Operación Elemental 1 (Columna): Intercambia dos columnas.
         * C_i <-> C_j
         * @param c1 El índice de la primera columna.
         * @param c2 El índice de la segunda columna.
         * @throw std::out_of_range Si los índices de columna son inválidos.
         * @note Complejidad: O(rows_)
         */
        void swap_cols(size_t c1, size_t c2) {
            if (c1 >= cols_ || c2 >= cols_) {
                throw std::out_of_range("Matrix::swap_cols(): Índice de columna fuera de rango.");
            }
            if (c1 == c2) {
                return; // No hacer nada
            }

            // Iterar por cada fila en estas columnas
            for (size_t i = 0; i < rows_; ++i) {
                std::swap((*this)(i, c1), (*this)(i, c2));
            }
        }

        /**
         * @brief Operación Elemental 2 (Columna): Multiplica una columna por un escalar.
         * C_i = k * C_i
         * @param c El índice de la columna a escalar.
         * @param scalar El escalar.
         * @throw std::out_of_range Si el índice de columna es inválido.
         * @note Complejidad: O(rows_)
         */
        void scale_col(size_t c, T scalar) {
            if (c >= cols_) {
                throw std::out_of_range("Matrix::scale_col(): Índice de columna fuera de rango.");
            }

            for (size_t i = 0; i < rows_; ++i) {
                (*this)(i, c) *= scalar;
            }
        }

        /**
         * @brief Operación Elemental 3 (Columna): Suma un múltiplo de una columna a otra.
         * C_target = C_target + (scalar * C_source)
         * @param c_target La columna que será modificada.
         * @param c_source La columna que se usará como fuente.
         * @param scalar El escalar para multiplicar la columna fuente.
         * @throw std::out_of_range Si los índices de columna son inválidos.
         * @note Complejidad: O(rows_)
         */
        void add_scaled_col(size_t c_target, size_t c_source, T scalar) {
            if (c_target >= cols_ || c_source >= cols_) {
                throw std::out_of_range("Matrix::add_scaled_col(): Índice de columna fuera de rango.");
            }

            for (size_t i = 0; i < rows_; ++i) {
                (*this)(i, c_target) += (*this)(i, c_source) * scalar;
            }
        }

        /**
         * @brief Calcula el determinante usando la traza de U en la factorización LU.
         *
         * Para matrices de 1x1 y 2x2, se usa un cálculo directo.
         *
         * @return T El valor del determinante.
         * @throw std::runtime_error Si la matriz no es cuadrada.
         * @note Complejidad: O(n^3) en el peor caso, O(1) para 1x1 y 2x2
         */
        [[nodiscard]]
        T determinant() const {
            if (rows_ != cols_) {
                throw std::runtime_error("Matrix::determinant(): El determinante solo se puede calcular para matrices cuadradas");
            }

            size_t n = rows_;

            // Caso base: Matriz 1x1
            if (n == 1) {
                return (*this)(0, 0);
            }

            // Caso base: Matriz 2x2
            if (n == 2) {
                // Usamos static_cast a long double para el cálculo intermedio
                // para evitar overflow antes de la resta.
                return static_cast<T>(
                    static_cast<long double>((*this)(0, 0)) * (*this)(1, 1) -
                    static_cast<long double>((*this)(0, 1)) * (*this)(1, 0)
                );
            }

            // Copia en long double para estabilidad numérica (y soportar enteros)
            Matrix<long double> LU(n, n);
            for(size_t i = 0; i < n; ++i) {
                for(size_t j = 0; j < n; ++j) {
                    LU(i, j) = static_cast<long double>((*this)(i, j));
                }
            }

            DS::Vector<size_t> P(n);
            int swaps = 0;
            try {
                swaps = lu_decompose_impl(LU, P);
            } catch (const std::runtime_error&) {
                return T{}; // Si falla por singularidad, el determinante es 0
            }

            // El determinante es el producto de la diagonal,
            // invirtiendo el signo si hubo un número impar de intercambios.
            long double det = (swaps % 2 != 0) ? -1.0L : 1.0L;
            for (size_t i = 0; i < n; ++i) {
                det *= LU(i, i);
            }

            return static_cast<T>(det);
        }

        /**
         * @brief Calcula la inversa de la matriz resolviendo AX = I mediante LU.
         *
         * @return Matrix La matriz inversa.
         * @throw std::runtime_error Si la matriz no es cuadrada o es singular (determinante 0).
         * @note Requiere que T sea un tipo de punto flotante (float, double).
         * @note Complejidad: O(n^3)
         */
        [[nodiscard]]
        Matrix inverse() const {
            if (rows_ != cols_) {
                throw std::runtime_error("Matrix::inverse(): La matriz debe ser cuadrada.");
            }
            
            // Verificación estática: La inversión de enteros no tiene sentido matemático
            // en este contexto (1/2 = 0). Debe usarse float o double.
            static_assert(std::is_floating_point_v<T>, "Matrix::inverse(): El tipo T debe ser punto flotante (float, double, etc).");

            size_t n = rows_;

            // Crear matriz Identidad
            Matrix I(n, n);
            for (size_t i = 0; i < n; ++i) {
                I(i, i) = T{1};
            }

            // La inversa es simplemente resolver el sistema con la Identidad
            return solve(I);
        }

        /**
         * @brief Resuelve el sistema de ecuaciones lineales Ax = B usando Descomposición LU.
         *
         * Descompone la matriz en PA = LU in-place. Es altamente eficiente y estable,
         * especialmente útil para cálculos repetitivos donde se resuelven múltiples
         * vectores B con la misma matriz de coeficientes.
         *
         * @param B La matriz (o vector) de constantes del lado derecho.
         * @return Matrix La matriz X tal que Ax = B.
         * @throw std::invalid_argument Si las dimensiones no coinciden.
         * @throw std::runtime_error Si la matriz A es singular.
         * @note Complejidad: O(n^3) para la factorización LU, más O(p * n^2) para la
         * sustitución (donde 'p' es el número de columnas de B). Tiempo total dominante: O(n^3).
         */
        [[nodiscard]]
        Matrix solve(const Matrix& B) const {
            if (rows_ != cols_) {
                throw std::runtime_error("Matrix::solve(): La matriz de coeficientes A debe ser cuadrada.");
            }
            if (rows_ != B.rows()) {
                throw std::invalid_argument("Matrix::solve(): El número de filas de B debe coincidir con A.");
            }

            static_assert(std::is_floating_point_v<T>, "Matrix::solve(): Requiere tipos de punto flotante.");

            const size_t n = rows_;
            const size_t p = B.cols();

            // Factorización LU in-place con Pivoteo Parcial (PA = LU)
            // L y U se almacenan sobre-escribiendo la misma matriz para ahorrar alocaciones.
            Matrix LU = *this;
            DS::Vector<size_t> P(n);
            lu_decompose_impl(LU, P);

            // Resolución del Sistema: Ly = PB, luego Ux = y
            Matrix X(n, p);

            // Aplicar permutación inicial: X arranca como B transpuesto por P (B' = PB)
            for (size_t i = 0; i < n; ++i) {
                for (size_t col = 0; col < p; ++col) {
                    X(i, col) = B(P[i], col);
                }
            }

            // Sustitución hacia adelante: Resolver Ly = B'
            // Recordar que L tiene unos (1.0) implícitos en su diagonal principal
            for (size_t i = 0; i < n; ++i) {
                for (size_t col = 0; col < p; ++col) {
                    T sum = 0;
                    for (size_t j = 0; j < i; ++j) sum += LU(i, j) * X(j, col);
                    X(i, col) -= sum;
                }
            }

            // Sustitución hacia atrás: Resolver Ux = y
            // Aquí sí dividimos por la diagonal porque la diagonal de U guarda los pivotes
            for (size_t i = n; i-- > 0; ) {
                T pivot = LU(i, i);
                for (size_t col = 0; col < p; ++col) {
                    T sum = 0;
                    for (size_t j = i + 1; j < n; ++j) sum += LU(i, j) * X(j, col);
                    X(i, col) = (X(i, col) - sum) / pivot;
                }
            }

            return X;
        }

        /**
         * @brief Calcula la matriz transpuesta.
         * Devuelve una nueva matriz en la que las filas de la matriz original
         * se convierten en columnas y las columnas en filas.
         * @note La matriz original no se modifica.
         * @return Matriz transpuesta de tamaño (cols_ x rows_).
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        Matrix transpose() const {
            // Crear una matriz con dimensiones invertidas
            Matrix result(cols_, rows_);

            // Intercambiar filas por columnas
            for (size_t i = 0; i < rows_; ++i) {
                for (size_t j = 0; j < cols_; ++j) {
                    result(j, i) = (*this)(i, j);
                }
            }

            // Devolver la matriz transpuesta
            return result;
        }

        /**
         * @brief Calcula la traza de la matriz.
         * La traza de una matriz es la suma de los elementos de su diagonal principal.
         * Si la matriz no es cuadrada, se suman los elementos hasta la mínima dimensión
         * (filas o columnas).
         * @return Suma de los elementos de la diagonal principal.
         * @note La matriz original no se modifica.
         * @note Complejidad: O(min(rows_, cols_))
         */
        [[nodiscard]]
        T trace() const {
            T sum = 0;
            size_t min_dim = Math::min(rows_, cols_);
            for (size_t i = 0; i < min_dim; ++i) {
                sum += (*this)(i, i);
            }
            return sum;
        }

        // -----------------------------------------------------------------
        // Operadores de Acceso
        // -----------------------------------------------------------------

        /**
         * @brief Accede al elemento en la fila 'i' y columna 'j' (modificable).
         * @param i Índice de la fila.
         * @param j Índice de la columna.
         * @return Referencia al elemento 'data_[i * cols_ + j]'.
         *
         * @note No realiza verificación de límites.
         *       Esta decisión prioriza el rendimiento y permite al compilador
         *       aplicar optimizaciones agresivas (por ejemplo, desenrollado
         *       de bucles y vectorización SIMD) en código crítico.
         *
         * @note Complejidad: O(1)
         *
         * @warning El comportamiento es indefinido si los índices están fuera de rango.
         */
        T& operator()(size_t i, size_t j) {
            return data_[i * cols_ + j];
        }

        /**
         * @brief Accede al elemento en la fila 'i' y columna 'j' (solo lectura).
         * @param i Índice de la fila.
         * @param j Índice de la columna.
         * @return Referencia constante al elemento 'data_[i * cols_ + j]'.
         *
         * @note No realiza verificación de límites.
         *       Esta decisión prioriza el rendimiento y permite al compilador
         *       aplicar optimizaciones agresivas (por ejemplo, desenrollado
         *       de bucles y vectorización SIMD) en código crítico.
         *
         * @note Complejidad: O(1)
         *
         * @warning El comportamiento es indefinido si los índices están fuera de rango.
         */
        const T& operator()(size_t i, size_t j) const {
            return data_[i * cols_ + j];
        }

        // -----------------------------------------------------------------
        // Operadores de Asignación Compuesta (In-Place)
        // -----------------------------------------------------------------

        /**
         * @brief Suma otra matriz a la actual (modificación in-place).
         * @param other La matriz a sumar.
         * @return Referencia a la matriz actual modificada (*this).
         * @throw std::invalid_argument Si las dimensiones no coinciden.
         * @note Complejidad: O(size_) = O(rows_ * cols_)
         */
        Matrix& operator+=(const Matrix& other) {
            if (rows_ != other.rows_ || cols_ != other.cols_) {
                throw std::invalid_argument("Matrix::operator+=: Las dimensiones deben coincidir.");
            }
            for (size_t i = 0; i < size_; ++i) {
                data_[i] += other.data_[i];
            }
            return *this;
        }

        /**
         * @brief Resta otra matriz a la actual (modificación in-place).
         * @param other La matriz a restar.
         * @return Referencia a la matriz actual modificada (*this).
         * @throw std::invalid_argument Si las dimensiones no coinciden.
         * @note Complejidad: O(size_) = O(rows_ * cols_)
         */
        Matrix& operator-=(const Matrix& other) {
            if (rows_ != other.rows_ || cols_ != other.cols_) {
                throw std::invalid_argument("Matrix::operator-=: Las dimensiones deben coincidir.");
            }
            for (size_t i = 0; i < size_; ++i) {
                data_[i] -= other.data_[i];
            }
            return *this;
        }

        /**
         * @brief Multiplica la matriz actual por un escalar (modificación in-place).
         * @param scalar El valor escalar.
         * @return Referencia a la matriz actual modificada (*this).
         * @note Complejidad: O(size_) = O(rows_ * cols_)
         */
        Matrix& operator*=(T scalar) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i] *= scalar;
            }
            return *this;
        }

        /**
         * @brief Divide la matriz actual por un escalar (modificación in-place).
         * @param scalar El valor escalar (divisor).
         * @return Referencia a la matriz actual modificada (*this).
         * @throw std::invalid_argument Si el escalar es cero o cercano a cero.
         * @note Complejidad: O(size_) = O(rows_ * cols_)
         */
        Matrix& operator/=(T scalar) {
            if (Math::abs(scalar) <= std::numeric_limits<T>::epsilon()) {
                throw std::invalid_argument("Matrix::operator/=: División por cero.");
            }
            for (size_t i = 0; i < size_; ++i) {
                data_[i] /= scalar;
            }
            return *this;
        }

        /**
         * @brief Multiplica la matriz actual por otra matriz y asigna el resultado.
         * @note Matemáticamente, la multiplicación de matrices requiere un buffer
         * temporal, por lo que esta operación realiza una asignación completa (no es
         * estrictamente O(1) en memoria como los escalares).
         * @param other La matriz multiplicadora.
         * @return Referencia a la matriz actual modificada (*this).
         * @throw std::invalid_argument Si las dimensiones internas no coinciden.
         * @note Complejidad: O(n^2.81) (Delega a Strassen).
         */
        Matrix& operator*=(const Matrix& other) {
            // Evaluamos el producto en una nueva matriz y luego la movemos a *this
            *this = *this * other;
            return *this;
        }

        // -----------------------------------------------------------------
        // Operadores Aritméticos Binarios (Retornan nueva matriz)
        // -----------------------------------------------------------------

        /**
         * @brief Suma de matrices. Delega en operator+=.
         * @param other La matriz a sumar (lado derecho).
         * @return Matrix Nueva matriz resultante.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        Matrix operator+(const Matrix& other) const {
            Matrix result = *this; // Constructor de copia
            result += other;
            return result;
        }

        /**
         * @brief Resta de matrices. Delega en operator-=.
         * @param other La matriz a restar (lado derecho).
         * @return Matrix Nueva matriz resultante.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        Matrix operator-(const Matrix& other) const {
            Matrix result = *this;
            result -= other;
            return result;
        }

        /**
         * @brief Multiplicación por un escalar (Matriz * Escalar).
         * @param scalar El valor escalar.
         * @return Matrix Nueva matriz resultante.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        Matrix operator*(T scalar) const {
            Matrix result = *this;
            result *= scalar;
            return result;
        }

        /**
         * @brief División por un escalar (Matriz / Escalar).
         * @param scalar El valor escalar.
         * @return Matrix Nueva matriz resultante.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        Matrix operator/(T scalar) const {
            Matrix result = *this;
            result /= scalar;
            return result;
        }

        /**
         * @brief Multiplicación de matrices.
         * Llama a la implementación interna de Strassen (o clásica si es pequeña).
         * @param other La matriz por la cual multiplicar (lado derecho).
         * @return Matrix Nueva matriz producto.
         * @throw std::invalid_argument Si cols_ != other.rows_.
         * @note Complejidad: O(n^2.81) para matrices cuadradas n x n.
         */
        [[nodiscard]]
        Matrix operator*(const Matrix& other) const {
            if (cols_ != other.rows_) {
                throw std::invalid_argument("Matrix::operator*: Dimensiones incompatibles para multiplicar.");
            }
            return strassen_mul(other);
        }

        // -----------------------------------------------------------------
        // Operadores Relacionales
        // -----------------------------------------------------------------

        /**
         * @brief Compara la igualdad exacta entre dos matrices.
         * @note Para matrices de punto flotante, considera implementar un método
         * de igualdad aproximada (is_approx) debido a los errores de precisión.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        bool operator==(const Matrix& other) const {
            if (rows_ != other.rows_ || cols_ != other.cols_) return false;
            return data_ == other.data_;
        }

        /**
         * @brief Compara la desigualdad entre dos matrices.
         * @note Complejidad: O(rows_ * cols_)
         */
        [[nodiscard]]
        bool operator!=(const Matrix& other) const {
            return !(*this == other);
        }

    private:
        size_t rows_;           // Número de filas
        size_t cols_;           // Número de columnas
        size_t size_;           // Número total de elementos (rows * cols)
        DS::Vector<T> data_;    // Almacenamiento 1D de datos en orden "row-major"

        /**
         * @brief Factorización LU in-place compartida.
         * @return El número de intercambios de filas realizados (útil para el determinante).
         * @throw std::runtime_error si la matriz es singular.
         * @note Complejidad: O(n^3)
         */
        template <typename U>
        static int lu_decompose_impl(Matrix<U>& LU, DS::Vector<size_t>& P) {
            const size_t n = LU.rows();
            int swaps = 0;

            for (size_t i = 0; i < n; ++i) {
                P[i] = i;
            }

            for (size_t k = 0; k < n; ++k) {
                // --- Pivoteo ---
                size_t pivot_row = k;
                U max_val = Math::abs(LU(k, k));

                for (size_t i = k + 1; i < n; ++i) {
                    U current_val = Math::abs(LU(i, k));
                    if (current_val > max_val) {
                        max_val = current_val;
                        pivot_row = i;
                    }
                }

                // Evitar división por cero (o extremadamente cerca de cero)
                if (max_val <= std::numeric_limits<U>::epsilon()) {
                    throw std::runtime_error("Matrix::lu_decompose_impl(): Matriz singular, el sistema no tiene solución única.");
                }

                // Intercambio lógico de filas (Permutación) y físico en LU
                if (pivot_row != k) {
                    LU.swap_rows(k, pivot_row);
                    std::swap(P[k], P[pivot_row]);
                    swaps++; // Rastrear intercambios para el determinante
                }

                // --- Descomposición ---
                U pivot = LU(k, k);
                for (size_t i = k + 1; i < n; ++i) {
                    // Almacenamos el factor de L directamente debajo de la diagonal principal
                    LU(i, k) /= pivot;
                    U factor = LU(i, k);

                    // Actualizamos la parte U. Iterar sobre 'j' aquí es rapidísimo
                    // porque los datos son contiguos (row-major alignment).
                    for (size_t j = k + 1; j < n; ++j) {
                        LU(i, j) -= factor * LU(k, j);
                    }
                }
            }
            return swaps;
        }
        
        /**
         * @brief Multiplicación de matrices O(n^3) clásica.
         *
         * Se usa como caso base para Strassen cuando las matrices son
         * más pequeñas que el umbral (THRESHOLD).
         *
         * @param other La matriz del lado derecho (dimensiones 'm x p').
         * @return Matrix La matriz resultante del producto (dimensiones 'n x p').
         * @note Complejidad: O(n*m*p)
         */
        [[nodiscard]]
        Matrix classical_mul(const Matrix& other) const {
            const size_t n = rows_;
            const size_t m = cols_; // == other.rows_
            const size_t p = other.cols_;

            Matrix result(n, p);

            const T* A_ptr = data_.data();
            const T* B_ptr = other.data_.data();
            T* R_ptr = result.data_.data();

            for (size_t i = 0; i < n; ++i) {
                // Pre-calcular el offset de la fila i de A y R para no multiplicarlo en cada k
                const size_t row_A_offset = i * m;
                const size_t row_R_offset = i * p;

                for (size_t k = 0; k < m; ++k) {
                    // T aik = (*this)(i, k);
                    T aik = A_ptr[row_A_offset + k];
                    const size_t row_B_offset = k * p;
                    for (size_t j = 0; j < p; ++j) {
                        // result(i, j) += aik * other(k, j);
                        R_ptr[row_R_offset + j] += aik * B_ptr[row_B_offset + j];
                    }
                }
            }
            return result;
        }

        /**
         * @brief Wrapper para la multiplicación de Strassen.
         *
         * Esta función es la interfaz que 'operator*' llama. Se encarga de:
         * 1. Cambiar a la multiplicación clásica O(n^3) si las matrices
         * son más pequeñas que el umbral (STRASSEN_THRESHOLD).
         * 2. Rellenar (padding) las matrices con ceros hasta la
         * siguiente potencia de 2, si son más grandes.
         * 3. Llamar a la función recursiva 'strassen_recursive'
         * en las matrices rellenadas.
         * 4. Extraer el resultado final de la matriz rellenada.
         *
         * @param other La matriz del lado derecho (dimensiones 'm x p').
         * @return La matriz resultante del producto (dimensiones 'n x p').
         * @note Complejidad: O(n^2.81) (junto a strassen_recursive())
         */
        [[nodiscard]]
        Matrix strassen_mul(const Matrix& other) const {
            const size_t n = rows_;
            const size_t m = cols_; // == other.rows_
            const size_t p = other.cols_;

            /*
             * -----------------------------------------------------------------
             * Por qué cambiamos al algoritmo clásico por debajo de un umbral?
             * -----------------------------------------------------------------
             *
             * 1. COSTO DE "OVERHEAD":
             * El algoritmo de Strassen tiene una complejidad asintótica mejor
             * O(n^2.81) que el clásico O(n^3). Sin embargo, esto ignora
             * los "costos fijos" o "constantes" (overhead).
             *
             * El "overhead" de Strassen es MUY ALTO. En cada llamada recursiva:
             * a) Se crean 10+ nuevas matrices temporales (A11, A12, ..., B11, ..., M1, ..., C11, ...).
             * b) Cada una de estas creaciones implica alocación dinámica de memoria
             * (llamar al constructor de DS::Vector), lo cual es lento.
             * c) Se realizan 18 sumas/restas de matrices (que son O(n^2)).
             *
             * 2. CROSSOVER POINT:
             * Para matrices pequeñas, el costo O(n^2) de todo este overhead
             * (alocaciones, sumas, restas) es MUCHO MAYOR que el costo del
             * algoritmo clásico O(n^3).
             *
             * El algoritmo clásico, aunque asintóticamente más lento, es
             * extremadamente simple: solo tiene 3 bucles anidados y casi
             * cero overhead.
             *
             * 3. LA SOLUCIÓN (EL UMBRAL):
             * El STRASSEN_THRESHOLD es el "punto de cruce" (crossover point)
             * donde el costo del overhead de Strassen empieza a ser menor
             * que los ahorros que se obtienen al evitar multiplicaciones.
             *
             * - Si (n < 64): Es más rápido hacer el O(n^3) clásico.
             * - Si (n > 64): El O(n^3) se vuelve tan grande que vale la pena
             * pagar el alto costo del overhead de Strassen para obtener la
             * ventaja del O(n^2.81).
             *
             * Este enfoque híbrido nos da lo mejor de ambos mundos:
             * velocidad O(n^2.81) para matrices grandes y velocidad O(n^3)
             * (con bajo overhead) para las sub-matrices pequeñas en la base
             * de la recursión.
             * -----------------------------------------------------------------
             */

            // --- Cambio a Clásica (Umbral) ---
            constexpr size_t STRASSEN_THRESHOLD = 64;
            if (n <= STRASSEN_THRESHOLD || m <= STRASSEN_THRESHOLD || p <= STRASSEN_THRESHOLD) {
                return classical_mul(other);
            }

            // --- Encontrar Siguiente Potencia de 2 ---
            // 'k' será el tamaño de nuestras matrices cuadradas rellenadas.
            size_t k = 1;
            while (k < n || k < m || k < p) {
                k *= 2;
            }

            // --- Crear Matrices Rellenas (Padding) ---
            // Se crean matrices k x k inicializadas a cero (gracias al constructor)
            Matrix A_pad(k, k);
            Matrix B_pad(k, k);

            // Copiar *this (A) en A_pad
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    A_pad(i, j) = (*this)(i, j);
                }
            }

            // Copiar other (B) en B_pad
            for (size_t i = 0; i < m; ++i) {
                for (size_t j = 0; j < p; ++j) {
                    B_pad(i, j) = other(i, j);
                }
            }

            // --- Llamar a la Recursión ---
            // Llamamos a la función recursiva con nuestras matrices
            // cuadradas y de potencia de 2.
            Matrix C_pad = strassen_recursive(A_pad, B_pad);

            // --- Extraer el Resultado Final ---
            // El resultado C_pad es k x k, pero solo queremos la
            // parte n x p superior izquierda.
            Matrix Result(n, p);
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < p; ++j) {
                    Result(i, j) = C_pad(i, j);
                }
            }

            return Result;
        }

        /**
         * @brief Implementación recursiva del algoritmo de Strassen.
         *
         * @warning Esta función ASUME que las matrices de entrada 'A' y 'B'
         * son CUADRADAS y sus dimensiones son una POTENCIA DE 2.
         * El 'padding' debe ser manejado por la función 'wrapper' (strassen_mul()).
         *
         * @param A La matriz del lado izquierdo (n x n).
         * @param B La matriz del lado derecho (n x n).
         * @return Matrix La matriz producto (n x n).
         * @note Ecuación de recurrencia: T(n) = 7 * T(n/2) + O(n^2)
         */
        [[nodiscard]]
        Matrix strassen_recursive(const Matrix& A, const Matrix& B) const {
            const size_t n = A.rows(); // A y B son n x n

            // --- Caso Base de la Recursión ---
            // (Podríamos poner n == STRASSEN_THRESHOLD y llamar a classical_mul,
            // pero n == 1 es el caso base matemático más puro)
            if (n == 1) {
                Matrix C(1, 1);
                C(0, 0) = A(0, 0) * B(0, 0);
                return C;
            }

            // --- División en Submatrices (n es potencia de 2, así que n/2 es limpio) ---
            size_t half_n = n / 2;

            // Crear submatrices (todas son half_n x half_n)
            Matrix A11(half_n, half_n);
            Matrix A12(half_n, half_n);
            Matrix A21(half_n, half_n);
            Matrix A22(half_n, half_n);

            Matrix B11(half_n, half_n);
            Matrix B12(half_n, half_n);
            Matrix B21(half_n, half_n);
            Matrix B22(half_n, half_n);

            // Llenar submatrices A
            for (size_t i = 0; i < half_n; ++i) {
                for (size_t j = 0; j < half_n; ++j) {
                    A11(i, j) = A(i, j);
                    A12(i, j) = A(i, j + half_n);
                    A21(i, j) = A(i + half_n, j);
                    A22(i, j) = A(i + half_n, j + half_n);
                }
            }

            // Llenar submatrices B
            for (size_t i = 0; i < half_n; ++i) {
                for (size_t j = 0; j < half_n; ++j) {
                    B11(i, j) = B(i, j);
                    B12(i, j) = B(i, j + half_n);
                    B21(i, j) = B(i + half_n, j);
                    B22(i, j) = B(i + half_n, j + half_n);
                }
            }

            // --- Calcular productos de Strassen (7 multiplicaciones recursivas) ---
            // (Las sumas funcionan porque todas las submatrices son half_n x half_n)
            Matrix M1 = strassen_recursive(A11 + A22, B11 + B22);
            Matrix M2 = strassen_recursive(A21 + A22, B11);
            Matrix M3 = strassen_recursive(A11, B12 - B22);
            Matrix M4 = strassen_recursive(A22, B21 - B11);
            Matrix M5 = strassen_recursive(A11 + A12, B22);
            Matrix M6 = strassen_recursive(A21 - A11, B11 + B12);
            Matrix M7 = strassen_recursive(A12 - A22, B21 + B22);

            // --- Calcular submatrices resultantes ---
            Matrix C11 = M1 + M4 - M5 + M7;
            Matrix C12 = M3 + M5;
            Matrix C21 = M2 + M4;
            Matrix C22 = M1 - M2 + M3 + M6;

            // --- Combinar submatrices en el resultado ---
            Matrix<T> result(n, n);
            for (size_t i = 0; i < half_n; ++i) {
                for (size_t j = 0; j < half_n; ++j) {
                    result(i, j) = C11(i, j);
                    result(i, j + half_n) = C12(i, j);
                    result(i + half_n, j) = C21(i, j);
                    result(i + half_n, j + half_n) = C22(i, j);
                }
            }

            return result;
        }

    };

    /**
     * @brief Sobrecarga del operador de inserción en stream ('<<') para imprimir la matriz.
     *
     * @tparam T El tipo numérico de los elementos de la matriz.
     * @param os El stream de salida (ej. 'std::cout').
     * @param matrix La matriz a imprimir.
     * @return El stream de salida ('os').
     * @note Complejidad: O(size_) = O(rows_ * cols_)
     */
    template<Numeric T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
        const size_t rows = matrix.rows();
        const size_t cols = matrix.cols();

        os << std::fixed << std::setprecision(3); // 3 decimales fijos

        for (size_t i = 0; i < rows; ++i) {
            os << "[ ";
            for (size_t j = 0; j < cols; ++j) {
                os << std::setw(10) << matrix(i, j) << " ";
            }
            os << "]\n";
        }

        return os;
    }

}

#endif //MATRIX_H