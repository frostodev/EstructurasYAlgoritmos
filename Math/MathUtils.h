#ifndef MATH_UTILS_H
#define MATH_UTILS_H

namespace Math {

    /**
     * @brief Trunca un valor de punto flotante, descartando la parte decimal.
     *
     * El valor se trunca hacia cero.
     * @param x El valor de entrada (p.ej., 3.8 o -3.8).
     * @return La parte entera de x (p.ej., 3.0 o -3.0).
     * @note Complejidad: O(1)
     */
    double trunc(double x);

    /**
     * @brief Redondea un valor de punto flotante al entero más cercano.
     *
     * Redondea alejándose de cero en los casos .5 (comportamiento estándar).
     * @param x El valor de entrada (p.ej., 3.2, 3.8, -3.5).
     * @return El valor redondeado (p.ej., 3.0, 4.0, -4.0).
     * @note Complejidad: O(1)
     */
    double round(double x);

    /**
     * @brief Redondea x hacia abajo, al entero más cercano no mayor que x.
     *
     * @param x El valor de entrada (p.ej., 3.8 o -3.2).
     * @return El valor redondeado (p.ej., 3.0 o -4.0).
     * @note Complejidad: O(1)
     */
    double floor(double x);

    /**
     * @brief Redondea x hacia arriba, al entero más cercano no menor que x.
     *
     * @param x El valor de entrada (p.ej., 3.2 o -3.8).
     * @return El valor redondeado (p.ej., 4.0 o -3.0).
     * @note Complejidad: O(1)
     */
    double ceil(double x);

    /**
     * @brief Retorna el menor de dos valores de punto flotante.
     * @param a El primer valor.
     * @param b El segundo valor.
     * @return El valor más pequeño (a o b).
     * @note Complejidad: O(1)
     */
    double min(double a, double b);

    /**
     * @brief Retorna el mayor de dos valores de punto flotante.
     * @param a El primer valor.
     * @param b El segundo valor.
     * @return El valor más grande (a o b).
     * @note Complejidad: O(1)
     */
    double max(double a, double b);

    /**
     * @brief Calcula e (base del logaritmo natural) elevado a la potencia x.
     *
     * @param x El valor del exponente.
     * @return El valor de e^x.
     * @note Complejidad: O(k) donde k ≈ |x / ln(2)|. El costo es dominado por
     * el bucle que calcula 2^k. El bucle de la serie de Taylor tiene
     * iteraciones constantes (O(1)).
     */
    double exp(double x);

    /**
     * @brief Calcula el logaritmo natural (base e) de x.
     *
     * @param x El valor de entrada.
     * @return El logaritmo natural de x.
     * @note Retorna NEGATIVE_INFINITY si x es 0.
     * @note Retorna NOT_A_NUMBER (NaN) si x es negativo.
     * @note Complejidad: O(k) donde k es el exponente binario de x.
     * El costo es dominado por los bucles 'while' de reducción de rango.
     * El bucle de la serie tiene iteraciones constantes (O(1)).
     */
    double log(double x);

    /**
     * @brief Eleva un valor 'base' a la potencia 'exp'.
     *
     * @param base El valor de la base.
     * @param exp El valor del exponente.
     * @return El resultado de base^exp.
     * @note Complejidad: O(|exp|) para exponentes enteros (debido al bucle
     * de multiplicación). Para exponentes no enteros, la complejidad
     * depende de las implementaciones O(k) de log() y exp().
     */
    double pow(double base, double exp);

    /**
     * @brief Calcula la raíz cuadrada de x (sqrt(x)).
     *
     * Implementado usando el método de Newton-Raphson para convergencia rápida.
     * @param x El valor de entrada (no debe ser negativo).
     * @return La raíz cuadrada de x.
     * @note Retorna NOT_A_NUMBER (NaN) si x es negativo.
     * @note Esta implementación no hace uso de Math::nth_root() por eficiencia.
     * @note Complejidad: O(1) (por número fijo de iteraciones)
     */
    double sqrt(double x);

    /**
     * @brief Calcula la raíz cúbica de x (cbrt(x)).
     *
     * Implementado usando el método de Newton-Raphson.
     * Maneja correctamente valores de entrada negativos (p.ej., cbrt(-8.0) = -2.0).
     * @param x El valor de entrada.
     * @return La raíz cúbica de x.
     * @note Esta implementación no hace uso de Math::nth_root() por eficiencia
     * @note Complejidad: O(1)
     */
    double cbrt(double x);

    /**
     * @brief Calcula la raíz n-ésima de x.
     *
     * Implementado usando el método de Newton-Raphson generalizado.
     * @param x El valor de entrada.
     * @param n El índice de la raíz (2 para sqrt, 3 para cbrt, etc.).
     * @return La raíz n-ésima de x.
     * @note Retorna NOT_A_NUMBER (NaN) si n es par y x es negativo.
     * @note Retorna NOT_A_NUMBER (NaN) si n es 0.
     * @note Complejidad: O(n) (El bucle llama a Math::pow())
     */
    double nth_root(double x, int n);

    /**
     * @brief Calcula el valor absoluto de cualquier tipo numérico.
     * Reemplaza a las versiones específicas de int y double.
     * Al estar en el header como template, el compilador genera
     * la versión correcta (int, float, double) bajo demanda.
     * @tparam T Tipo del valor (int, float, double, etc.)
     * @param x El valor de entrada.
     * @return El valor absoluto de x.
     */
    template <typename T>
    T abs(T x) {
        // Simple, eficiente y funciona para todo tipo con signo
        // T{0} asegura que comparamos con el cero del mismo tipo
        return (x < T{0}) ? -x : x;
    }

    /**
     * @brief Calcula el resto de punto flotante de la división x / y.
     *
     * Esta función imita el comportamiento de `fmod(x, y)` de <cmath>.
     * El resultado `r` se calcula tal que `x = n*y + r`, donde `n` es la
     * parte entera de `x/y` (truncada hacia cero) y `r` tiene el
     * mismo signo que `x`.
     *
     * @param x El numerador (dividendo).
     * @param y El denominador (divisor).
     * @return El resto de x/y. Retorna 0 si y es 0.
     * @note Complejidad: O(1)
     */
    double fmod(double x, double y);

    /**
     * @brief Calcula el factorial de un número entero no negativo.
     *
     * Esta función plantilla computa el factorial de un número entero `n`
     * utilizando un enfoque iterativo. Es eficiente y evita el uso de recursión,
     * lo que elimina el riesgo de desbordamiento de pila.
     *
     * El resultado se devuelve en el tipo genérico `T`, lo que permite calcular
     * factoriales con distintos tipos numéricos (por ejemplo, `unsigned long long`
     * para enteros pequeños o `boost::multiprecision::cpp_int` para enteros grandes).
     *
     * @tparam T Tipo del resultado (debe soportar la multiplicación y asignación).
     * @param n Número entero no negativo cuyo factorial se desea calcular.
     * @return El valor de n! como una instancia del tipo `T`.
     *
     * @note Para valores grandes de `n`, se recomienda usar un tipo de precisión
     * arbitraria como `boost::multiprecision::cpp_int` para evitar overflow.
     * Si `n` es 0 o 1, la función devuelve 1.
     *
     * @note Complejidad: O(n)
     */
    template <typename T>
    T factorial(unsigned int n);

}

#endif //MATH_UTILS_H