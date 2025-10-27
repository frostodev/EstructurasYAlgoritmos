
#ifndef VECTOR2_H
#define VECTOR2_H

#include "MathUtils.h"
#include "Trigonometry.h"

#include <iomanip>

namespace Math {

    /**
     * @concept Numeric
     * @brief Restringe un tipo de plantilla T a ser un tipo aritmético (int, float, double, etc.).
     */
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * @class Vector2
     * @brief Representa un vector 2D (x, y).
     * @tparam T El tipo numérico (int, float, double) para las componentes.
     * Debe satisfacer el concept 'Numeric'.
     */
    template<Numeric T>
    class Vector2 {
    public:
        // --- Componentes ---
        T x;
        T y;

        // -----------------------------------------------------------------
        // Constructores
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto (Vector Cero).
         * @note Complejidad: O(1)
         */
        Vector2() : x(T{}), y(T{}) {}

        /**
         * @brief Constructor con componentes.
         * @param x_val Valor inicial para la componente x.
         * @param y_val Valor inicial para la componente y.
         * @note Complejidad: O(1)
         */
        Vector2(T x_val, T y_val) : x(x_val), y(y_val) {}

        /**
         * @brief Constructor de copia.
         * @param other El vector a copiar.
         * @note Complejidad: O(1)
         */
        Vector2(const Vector2& other) = default;

        /**
         * @brief Constructor de movimiento.
         * @param other El vector a mover.
         * @note Complejidad: O(1)
         */
        Vector2(Vector2&& other) noexcept = default;

        /**
         * @brief Operador de asignación por copia.
         * @param other El vector a copiar.
         * @return Vector2& Referencia a este vector.
         * @note Complejidad: O(1)
         */
        Vector2& operator=(const Vector2& other) = default;

        /**
         * @brief Operador de asignación por movimiento.
         * @param other El vector a mover.
         * @return Vector2& Referencia a este vector.
         * @note Complejidad: O(1)
         */
        Vector2& operator=(Vector2&& other) noexcept = default;

        /**
         * @brief Destructor.
         * @note Complejidad: O(1)
         */
        ~Vector2() = default;

        // -----------------------------------------------------------------
        // Operadores de Vectores (Aritmética)
        // -----------------------------------------------------------------

        /**
         * @brief Suma de vectores (this + other)
         * @param other El vector a sumar.
         * @return Vector2 Un nuevo Vector2 resultado de la suma.
         * @note Complejidad: O(1)
         */
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        /**
         * @brief Resta de vectores (this - other)
         * @param other El vector a restar.
         * @return Vector2 Un nuevo Vector2 resultado de la resta.
         * @note Complejidad: O(1)
         */
        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

        /**
         * @brief Negación unaria (-this)
         * @return Vector2 Un nuevo Vector2 con ambas componentes negadas.
         * @note Complejidad: O(1)
         */
        Vector2 operator-() const {
            return Vector2(-x, -y);
        }

        /**
         * @brief Multiplicación por escalar (this * scalar)
         * @param scalar El escalar por el cual multiplicar.
         * @return Vector2 Un nuevo Vector2 escalado.
         * @note Complejidad: O(1)
         */
        Vector2 operator*(T scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

        /**
         * @brief División por escalar (this / scalar)
         * @param scalar El escalar por el cual dividir.
         * @return Vector2 Un nuevo Vector2 escalado.
         * @note Complejidad: O(1)
         */
        Vector2 operator/(T scalar) const {
            // No se comprueba la división por cero, se comporta como
            // la división nativa de T (que puede ser inf o indefinida).
            return Vector2(x / scalar, y / scalar);
        }

        // -----------------------------------------------------------------
        // Operadores de Asignación (In-Place)
        // -----------------------------------------------------------------

        /**
         * @brief Suma y asignación (in-place).
         * @param other El vector a sumar.
         * @return Vector2& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        /**
         * @brief Resta y asignación (in-place).
         * @param other El vector a restar.
         * @return Vector2& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        /**
         * @brief Multiplicación por escalar y asignación (in-place).
         * @param scalar El escalar por el cual multiplicar.
         * @return Vector2& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector2& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief División por escalar y asignación (in-place).
         * @param scalar El escalar por el cual dividir.
         * @return Vector2& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector2& operator/=(T scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // -----------------------------------------------------------------
        // Operadores de Comparación
        // -----------------------------------------------------------------

        /**
         * @brief Comparación de igualdad exacta.
         * @param other El vector con el cual comparar.
         * @return true si x e y son exactamente iguales, false en caso contrario.
         * @warning Para T de punto flotante, esto puede fallar debido a
         * errores de precisión. Use alguna función 'are_equal' si necesita
         * comparación con tolerancia (epsilon).
         * @note Complejidad: O(1)
         */
        bool operator==(const Vector2& other) const {
            return (x == other.x) && (y == other.y);
        }

        /**
         * @brief Comparación de desigualdad exacta.
         * @param other El vector con el cual comparar.
         * @return true si x o y no son exactamente iguales, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool operator!=(const Vector2& other) const {
            return !(*this == other);
        }

        // -----------------------------------------------------------------
        // Métodos de Vector
        // -----------------------------------------------------------------

        /**
         * @brief Calcula el producto punto (dot product) con otro vector.
         * @param other El otro vector.
         * @return T El valor escalar del producto punto.
         * @note Complejidad: O(1)
         */
        T dot(const Vector2& other) const {
            return (x * other.x) + (y * other.y);
        }

        /**
         * @brief Calcula la magnitud (longitud) al cuadrado.
         * Más rápido que magnitude() ya que evita la raíz cuadrada.
         * @return T La magnitud al cuadrado.
         * @note Complejidad: O(1)
         */
        T magnitude_sq() const {
            return (x * x) + (y * y);
        }

        /**
         * @brief Calcula la magnitud (longitud) del vector.
         *
         * Usa Math::sqrt (que opera con 'double') y luego convierte
         * el resultado de nuevo a 'T'.
         * @return T La magnitud del vector.
         * @note Complejidad: O(1) (asumiendo O(1) para Math::sqrt)
         */
        T magnitude() const {
            // Convertimos a double para usar Math::sqrt de MathUtils.h
            double mag_sq_double = static_cast<double>(magnitude_sq());
            // Convertimos el resultado de nuevo a T
            return static_cast<T>(Math::sqrt(mag_sq_double));
        }

        /**
         * @brief Devuelve una copia normalizada (longitud 1) de este vector.
         * @return Vector2<T> El vector normalizado, o (0,0) si la magnitud era 0.
         * @note Complejidad: O(1)
         */
        Vector2 normalized() const {
            T mag = magnitude();
            if (mag == T{}) { // mag == 0
                return Vector2(T{}, T{});
            }
            return (*this) / mag;
        }

        /**
         * @brief Normaliza este vector (modifica in-place) para que tenga longitud 1.
         * @note Complejidad: O(1)
         */
        void normalize() {
            T mag = magnitude();
            if (mag == T{}) { // mag == 0
                x = T{};
                y = T{};
            } else {
                (*this) /= mag;
            }
        }

        /**
         * @brief Calcula la distancia a otro vector.
         * @param other El otro vector.
         * @return T La distancia escalar.
         * @note Complejidad: O(1)
         */
        T distance(const Vector2& other) const {
            // (this - other).magnitude()
            return (*this - other).magnitude();
        }

        /**
         * @brief Calcula el ángulo del vector en radianes respecto al eje X positivo.
         *
         * Usa Math::atan2 (que opera y devuelve 'double').
         * @return double El ángulo en radianes [-PI, PI].
         * @note Complejidad: O(1)
         */
        double angle() const {
            // Convertir T a double para Math::atan2
            return Math::atan2(static_cast<double>(y), static_cast<double>(x));
        }
    };

    // -----------------------------------------------------------------
    // Operadores Externos (Soporte para Scalar * Vector)
    // -----------------------------------------------------------------

/**
     * @brief Multiplicación por escalar (escalar * vector)
     * @tparam T El tipo numérico.
     * @param scalar El escalar.
     * @param vec El vector.
     * @return Vector2<T> Un nuevo Vector2 resultado de la multiplicación.
     * @note Complejidad: O(1)
     */
    template<Numeric T>
    Vector2<T> operator*(T scalar, const Vector2<T>& vec) {
        return vec * scalar; // Reutiliza el operador miembro (vec * scalar)
    }

    /**
     * @brief Operador de salida para std::ostream (para imprimir/depurar)
     * @tparam T El tipo numérico.
     * @param os El stream de salida.
     * @param vec El vector a imprimir.
     * @return std::ostream& Referencia al stream de salida.
     * @note Complejidad: O(1)
     */
    template<Numeric T>
    std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

}

#endif //VECTOR2_H