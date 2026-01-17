#ifndef TENSOR3D_H
#define TENSOR3D_H

#include "../DataStructures/Vector.h"

#include <stdexcept>
#include <iomanip>
#include <limits>

namespace Math {

    /**
     * @concept Numeric
     * @brief Restringe un tipo de plantilla T a ser un tipo aritmético (int, float, double, etc.).
     */
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * @class Tensor3D
     * @brief Una clase de plantilla para representación de datos 3D (Cubo/Prisma).
     * @tparam T El tipo numérico de los elementos.
     *
     * Almacena los datos en un único vector contiguo para eficiencia de caché.
     * El orden de almacenamiento es (layer, row, column) o (z, y, x).
     */
    template<Numeric T>
    class Tensor3D {
    public:
        /**
         * @brief Construye un Tensor3D con las dimensiones especificadas.
         * @param layers Número de "capas" (dimensión Z).
         * @param rows Número de filas (dimensión Y).
         * @param cols Número de columnas (dimensión X).
         * @throw std::overflow_error Si 'layers * rows * cols' excede el límite.
         * @note: Complejidad: O(layers * rows * cols)
         */
        Tensor3D(size_t layers, size_t rows, size_t cols)
            : layers_(layers), rows_(rows), cols_(cols), size_(0), data_() {

            if (layers <= 0 || rows <= 0 || cols <= 0) {
                throw std::invalid_argument("Tensor3D(): Las dimensiones deben ser positivas.");
            }

            // Verificar overflow
            size_t plane_size = rows * cols;
            if (cols != 0 && rows > std::numeric_limits<size_t>::max() / cols) {
                 throw std::overflow_error("Tensor3D(): Dimensiones 'rows * cols' muy grandes.");
            }
            if (layers != 0 && plane_size > std::numeric_limits<size_t>::max() / layers) {
                throw std::overflow_error("Tensor3D(): Dimensiones 'layers * rows * cols' muy grandes.");
            }

            size_ = layers * plane_size;
            data_.resize(size_); // Inicializa a cero
        }

        // -----------------------------------------------------------------
        // Regla de Cero (Rule of Zero)
        // -----------------------------------------------------------------
        //
        // Esta clase es un "manejador de recursos" que posee un miembro
        // (DS::Vector data_) que ya implementa la Regla de los 5.
        //
        // Al usar 'default', le pedimos al compilador que genere
        // automáticamente las 5 operaciones especiales. El compilador
        // lo hará llamando a las operaciones correspondientes de 'data_'.
        //
        // Esto nos da:
        // - Copia profunda (vía el ctor de copia de DS::Vector)
        // - Movimiento eficiente (vía el ctor de movimiento de DS::Vector)
        // - Destrucción correcta (vía el dtor de DS::Vector)
        // ...todo sin código manual.
        //
        ~Tensor3D() = default;
        Tensor3D(const Tensor3D& other) = default;
        Tensor3D& operator=(const Tensor3D& other) = default;
        Tensor3D(Tensor3D&& other) noexcept = default;
        Tensor3D& operator=(Tensor3D&& other) noexcept = default;

        // -----------------------------------------------------------------
        // Acceso a Dimensiones
        // -----------------------------------------------------------------

        /**
         * @brief Obtiene el número de capas.
         * @return El número de capas ('layers_').
         * @note: Complejidad: O(1)
         */
        size_t layers() const {
            return layers_;
        }

        /**
         * @brief Obtiene el número de filas.
         * @return El número de filas ('rows_').
         * @note: Complejidad: O(1)
         */
        size_t rows() const {
            return rows_;
        }

        /**
         * @brief Obtiene el número de columnas.
         * @return El número de columnas ('cols').
         * @note: Complejidad: O(1)
         */
        size_t cols() const {
            return cols_;
        }

        /**
         * @brief Obtiene el número total de elementos.
         * @return El tamaño total ('size_'), igual a 'rows * cols * layers'.
         * @note: Complejidad: O(1)
         */
        size_t size() const {
            return size_;
        }

        // -----------------------------------------------------------------
        // Acceso a Elementos
        // -----------------------------------------------------------------

        /**
         * @brief Accede al elemento en (z, y, x) (modificable).
         * @param z Índice de la capa (layer).
         * @param y Índice de la fila (row).
         * @param x Índice de la columna (column).
         * @return T& Referencia al elemento.
         * @throw std::out_of_range Si los índices están fuera de límites.
         * @note Complejidad: O(1)
         */
        T& operator()(size_t z, size_t y, size_t x) {
            if (z >= layers_ || y >= rows_ || x >= cols_) {
                throw std::out_of_range("Tensor3D::operator(): Índices fuera de rango.");
            }
            // index = (z * (rows_ * cols_)) + (y * cols_) + x
            return data_[(z * rows_ * cols_) + (y * cols_) + x];
        }

        /**
         * @brief Accede al elemento en (z, y, x) (constante).
         * @param z Índice de la capa (layer).
         * @param y Índice de la fila (row).
         * @param x Índice de la columna (column).
         * @return const T& Referencia constante al elemento.
         * @throw std::out_of_range Si los índices están fuera de límites.
         * @note Complejidad: O(1)
         */
        const T& operator()(size_t z, size_t y, size_t x) const {
            if (z >= layers_ || y >= rows_ || x >= cols_) {
                throw std::out_of_range("Tensor3D::operator(): Índices fuera de rango.");
            }
            // index = (z * (rows_ * cols_)) + (y * cols_) + x
            return data_[(z * rows_ * cols_) + (y * cols_) + x];
        }

    private:
        size_t layers_;         // Dimensión Z
        size_t rows_;           // Dimensión Y
        size_t cols_;           // Dimensión X
        size_t size_;           // Total de elementos (Z * Y * X)
        DS::Vector<T> data_;    // Almacenamiento 1D
    };

    /**
     * @brief Sobrecarga del operador de inserción en stream ('<<') para imprimir el Tensor3D.
     * @tparam T El tipo numérico de los elementos del tensor.
     * @param os El stream de salida (ej. 'std::cout').
     * @param tensor El tensor a imprimir.
     * @return El stream de salida ('os').
     * @note Complejidad: O(size_) = O(rows_ * cols_ * layers_)
     */
    template<Numeric T>
    std::ostream& operator<<(std::ostream& os, const Tensor3D<T>& tensor) {
        os << std::fixed << std::setprecision(3); // 3 decimales fijos

        for (size_t z = 0; z < tensor.layers(); ++z) {
            os << "Capa (Layer) " << z << ":\n";
            for (size_t y = 0; y < tensor.rows(); ++y) {
                os << "[ ";
                for (size_t x = 0; x < tensor.cols(); ++x) {
                    os << std::setw(10) << tensor(z, y, x) << " ";
                }
                os << "]\n";
            }
            os << "\n";
        }
        return os;
    }

}

#endif //TENSOR3D_H