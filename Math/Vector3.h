#ifndef VECTOR3_H
#define VECTOR3_H

#include "MathUtils.h"
#include "Trigonometry.h"

#include <iomanip>
#include <iostream>

namespace Math {

    /**
     * @concept Numeric
     * @brief Restringe un tipo de plantilla T a ser un tipo aritmético (int, float, double, etc.).
     */
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * @class Vector3
     * @brief Representa un vector 3D (x, y, z).
     * @tparam T El tipo numérico (int, float, double) para las componentes.
     * Debe satisfacer el concept 'Numeric'.
     */
    template<Numeric T>
    class Vector3 {
    public:
        // --- Componentes ---
        T x;
        T y;
        T z;

        // -----------------------------------------------------------------
        // Constructores
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto (Vector Cero).
         * @note Complejidad: O(1)
         */
        Vector3() : x(T{}), y(T{}), z(T{}) {}

        /**
         * @brief Constructor con componentes.
         * @param x_val Valor inicial para la componente x.
         * @param y_val Valor inicial para la componente y.
         * @param z_val Valor inicial para la componente z.
         * @note Complejidad: O(1)
         */
        Vector3(T x_val, T y_val, T z_val) : x(x_val), y(y_val), z(z_val) {}

        /**
         * @brief Constructor de copia.
         * @param other El vector a copiar.
         * @note Complejidad: O(1)
         */
        Vector3(const Vector3& other) = default;

        /**
         * @brief Constructor de movimiento.
         * @param other El vector a mover.
         * @note Complejidad: O(1)
         */
        Vector3(Vector3&& other) noexcept = default;

        /**
         * @brief Operador de asignación por copia.
         * @param other El vector a copiar.
         * @return Vector3& Referencia a este vector.
         * @note Complejidad: O(1)
         */
        Vector3& operator=(const Vector3& other) = default;

        /**
         * @brief Operador de asignación por movimiento.
         * @param other El vector a mover.
         * @return Vector3& Referencia a este vector.
         * @note Complejidad: O(1)
         */
        Vector3& operator=(Vector3&& other) noexcept = default;

        /**
         * @brief Destructor.
         * @note Complejidad: O(1)
         */
        ~Vector3() = default;

        // -----------------------------------------------------------------
        // Operadores de Vectores (Aritmética)
        // -----------------------------------------------------------------

        /**
         * @brief Suma de vectores (this + other)
         * @param other El vector a sumar.
         * @return Vector3& Un nuevo Vector3 resultado de la suma.
         * @note Complejidad: O(1)
         */
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        /**
         * @brief Resta de vectores (this - other)
         * @param other El vector a restar.
         * @return Vector3& Un nuevo Vector3 resultado de la resta.
         * @note Complejidad: O(1)
         */
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        /**
         * @brief Negación unaria (-this)
         * @return Vector3& Un nuevo Vector3 con todas las componentes negadas.
         * @note Complejidad: O(1)
         */
        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        /**
         * @brief Multiplicación por escalar (this * scalar)
         * @param scalar El escalar por el cual multiplicar.
         * @return Vector3& Un nuevo Vector3 escalado.
         * @note Complejidad: O(1)
         */
        Vector3 operator*(T scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        /**
         * @brief División por escalar (this / scalar)
         * @param scalar El escalar por el cual dividir.
         * @return Vector3& Un nuevo Vector3 escalado.
         * @note Complejidad: O(1)
         */
        Vector3 operator/(T scalar) const {
            // No se comprueba la división por cero, se comporta como
            // la división nativa de T (que puede ser inf o indefinida).
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        // -----------------------------------------------------------------
        // Operadores de Asignación (In-Place)
        // -----------------------------------------------------------------

        /**
         * @brief Suma y asignación (in-place).
         * @param other El vector a sumar.
         * @return Vector3& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        /**
         * @brief Resta y asignación (in-place).
         * @param other El vector a restar.
         * @return Vector3& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        /**
         * @brief Multiplicación por escalar y asignación (in-place).
         * @param scalar El escalar por el cual multiplicar.
         * @return Vector3& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector3& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief División por escalar y asignación (in-place).
         * @param scalar El escalar por el cual dividir.
         * @return Vector3& Referencia a este vector modificado.
         * @note Complejidad: O(1)
         */
        Vector3& operator/=(T scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        // -----------------------------------------------------------------
        // Operadores de Comparación
        // -----------------------------------------------------------------

        /**
         * @brief Comparación de igualdad exacta.
         * @param other El vector con el cual comparar.
         * @return true si x, y, z son exactamente iguales, false en caso contrario.
         * @warning Para T de punto flotante, usar comparación con epsilon.
         * @note Complejidad: O(1)
         */
        bool operator==(const Vector3& other) const {
            return (x == other.x) && (y == other.y) && (z == other.z);
        }

        /**
         * @brief Comparación de desigualdad exacta.
         * @param other El vector con el cual comparar.
         * @return true si alguna componente no es exactamente igual, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool operator!=(const Vector3& other) const {
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
        T dot(const Vector3& other) const {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }

        /**
         * @brief Calcula el producto cruz (cross product) con otro vector.
         * @param other El otro vector.
         * @return Vector3 Un nuevo Vector3 perpendicular a this y other.
         * @note Complejidad: O(1)
         */
        Vector3 cross(const Vector3& other) const {
            return Vector3(
                (y * other.z) - (z * other.y),
                (z * other.x) - (x * other.z),
                (x * other.y) - (y * other.x)
            );
        }

        /**
         * @brief Calcula la magnitud (longitud) al cuadrado.
         * Más rápido que magnitude() ya que evita la raíz cuadrada.
         * @return T La magnitud al cuadrado.
         * @note Complejidad: O(1)
         */
        T magnitude_sq() const {
            return (x * x) + (y * y) + (z * z);
        }

        /**
         * @brief Calcula la magnitud (longitud) del vector.
         * @return T La magnitud del vector.
         * @note Complejidad: O(1) (asumiendo O(1) para Math::sqrt)
         */
        T magnitude() const {
            // Convertir a double para usar Math::sqrt de MathUtils.h
            double mag_sq_double = static_cast<double>(magnitude_sq());
            // Convertir el resultado de nuevo a T
            return static_cast<T>(Math::sqrt(mag_sq_double));
        }

        /**
         * @brief Devuelve una copia normalizada (longitud 1) de este vector.
         * @return Vector3<T> El vector normalizado, o (0,0,0) si la magnitud era 0.
         * @note Complejidad: O(1)
         */
        Vector3 normalized() const {
            T mag = magnitude();
            if (mag == T{}) { // mag == 0
                return Vector3(T{}, T{}, T{});
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
                z = T{};
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
        T distance(const Vector3& other) const {
            // (this - other).magnitude()
            return (*this - other).magnitude();
        }

        /**
         * @brief Calcula el ángulo en radianes entre este vector y otro.
         * @param other El otro vector.
         * @return double El ángulo en radianes [0, PI].
         * @note Complejidad: O(1)
         */
        double angle_to(const Vector3& other) const {
            // Usamos magnitude_sq() para evitar raíces si es posible
            T mag_sq_1 = magnitude_sq();
            T mag_sq_2 = other.magnitude_sq();

            // Si cualquiera de los vectores es cero, el ángulo es 0
            if (mag_sq_1 == T{} || mag_sq_2 == T{}) {
                return 0.0;
            }

            // Convertir a double para las funciones Math::
            double dot_prod = static_cast<double>(dot(other));
            double mags_prod = Math::sqrt(static_cast<double>(mag_sq_1)) * Math::sqrt(static_cast<double>(mag_sq_2));

            double cos_theta = dot_prod / mags_prod;

            // "Clamp": Corregir errores de precisión de punto flotante
            // (p.ej., 1.0000001) que harían fallar a acos()
            if (cos_theta > 1.0) cos_theta = 1.0;
            if (cos_theta < -1.0) cos_theta = -1.0;

            return Math::acos(cos_theta);
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
     * @return Vector3<T> Un nuevo Vector3 resultado de la multiplicación.
     * @note Complejidad: O(1)
     */
    template<Numeric T>
    Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
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
    std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

}

#endif //VECTOR3_H