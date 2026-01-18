#include "MathUtils.h"
#include "Constants.h"

#include <limits>

namespace Math {

    double trunc(double x) {
        // La conversión a un entero (long long) trunca la parte decimal
        // (corta los decimales, moviéndose hacia cero).
        // Luego lo convertimos de nuevo a double
        // p.ej. static_cast<long long>(3.8)  -> 3
        // p.ej. static_cast<long long>(-3.8) -> -3
        return static_cast<double>(static_cast<long long>(x));
    }

    double round(double x) {
        // Para redondear, añadimos o restamos 0.5 y *luego* truncamos
        // Esto mueve el número al "cubo" de enteros correcto antes de cortar
        if (x >= 0.0) {
            // p.ej. 3.2 + 0.5 = 3.7 -> trunc(3.7) = 3.0
            // p.ej. 3.8 + 0.5 = 4.3 -> trunc(4.3) = 4.0
            return trunc(x + 0.5);
        } else {
            // p.ej. -3.2 - 0.5 = -3.7 -> trunc(-3.7) = -3.0
            // p.ej. -3.8 - 0.5 = -4.3 -> trunc(-4.3) = -4.0
            return trunc(x - 0.5);
        }
    }

    double floor(double x) {
        // Obtener la parte entera truncando (hacia cero)
        double int_part = Math::trunc(x);

        // Si el número es negativo y tiene decimales (p.ej., -3.2),
        // trunc() da -3.0, pero floor() debe dar -4.0.
        // x != int_part comprueba si hay decimales
        if (x < 0.0 && x != int_part) {
            return int_part - 1.0;
        }

        // Para números positivos (3.8 -> 3.0) o enteros (-3.0 -> -3.0),
        // trunc() ya da el resultado correcto
        return int_part;
    }

    double ceil(double x) {
        // Obtener la parte entera truncando (hacia cero)
        double int_part = Math::trunc(x);

        // Si el número es positivo y tiene decimales (p.ej., 3.2),
        // trunc() da 3.0, pero ceil() debe dar 4.0
        if (x > 0.0 && x != int_part) {
            return int_part + 1.0;
        }

        // Para números negativos (-3.2 -> -3.0) o enteros (3.0 -> 3.0),
        // trunc() ya da el resultado correcto
        return int_part;
    }

    double min(double a, double b) {
        return (a < b) ? a : b;
    }

    double max(double a, double b) {
        return (a > b) ? a : b;
    }

    double exp(double x) {
        // --- Casos Especiales ---
        if (x == 0.0) {
            return 1.0;
        }
        // Manejo de desbordamiento (Overflow)
        if (x > 709.782712893384) { // Límite aprox. para double
            return POSITIVE_INFINITY;
        }
        // Manejo de subdesbordamiento (Underflow)
        if (x < -709.782712893384) {
            return 0.0;
        }

        // --- Reducción de Rango ---
        // Usamos la identidad e^x = 2^k * e^r, donde x = k*ln(2) + r.
        // Queremos que 'r' sea pequeño (en [-ln(2)/2, ln(2)/2])
        // para que la serie de Taylor converja rápidamente.
        // x / ln(2) = k + (r / ln(2))
        // k = round(x / LN2)
        // r = x - k * LN2

        long long k = static_cast<long long>(Math::round(x / LN2));
        double r = x - static_cast<double>(k) * LN2;

        // --- Cálculo de la Serie de Taylor para e^r ---
        // e^r = 1 + r + r^2/2! + r^3/3! + ...
        //
        // Paso iterativo:
        // termino_n = termino_{n-1} * r / n
        double sum = 1.0;
        double term = 1.0;

        for (int n = 1; n < 30; ++n) { // 30 iteraciones son más que suficientes
            const double old_sum = sum;

            term = term * r / static_cast<double>(n);
            sum += term;

            // Comprobación de convergencia
            if (sum == old_sum) {
                break;
            }
        }

        // --- Calcular 2^k ---
        // Esto es más rápido y preciso que un bucle
        // Usando la función C estándar ldexp(1.0, k)
        double power_of_two = 1.0;
        if (k > 0) {
            for (long long i = 0; i < k; ++i) {
                power_of_two *= 2.0;
            }
        } else if (k < 0) {
            for (long long i = 0; i < -k; ++i) {
                power_of_two /= 2.0;
            }
        }

        // e^x = e^r * 2^k
        return sum * power_of_two;
    }

    double log(double x) {
        // --- Casos Especiales ---
        if (x < 0.0) {
            return NOT_A_NUMBER; // log de negativos es NaN
        }
        if (x == 0.0) {
            return NEGATIVE_INFINITY; // log(0) es -inf
        }
        if (x == 1.0) {
            return 0.0; // log(1) es 0
        }

        // --- Reducción de Rango ---
        // Usamos la identidad log(x) = log(m * 2^k) = log(m) + k*log(2)
        // Descomponemos x en su mantisa 'm' (en [0.5, 1.0)) y exponente 'k'
        // Esto es básicamente lo que hace frexp() de <cmath>
        int k = 0;
        double m = x;

        // Normalizar m al rango [0.5, 1.0)
        while (m >= 1.0) {
            m /= 2.0;
            k++;
        }
        while (m < 0.5) {
            m *= 2.0;
            k--;
        }

        // --- Cálculo de la Serie (Serie de Halley-Newton / Pade) ---
        // La serie de Taylor para log(m) converge lento.
        // Usamos una aproximación mucho más rápida (serie para atanh):
        // log(m) = log( (1+z)/(1-z) ) donde z = (m-1)/(m+1)
        // log(m) = 2 * (z + z^3/3 + z^5/5 + ...)

        const double z = (m - 1.0) / (m + 1.0);
        const double z_squared = z * z;

        double sum = z;
        double term = z;

        for (int n = 1; n < 20; ++n) {      // 20 iteraciones deberían ser suficientes
            const double old_sum = sum;

            // Siguiente término (z^3, z^5, z^7...)
            term *= z_squared;

            // Añadir el término (term / (2n+1))
            sum += term / (2.0 * n + 1.0);

            if (sum == old_sum) {
                break;
            }
        }

        // log(m) = 2 * sum
        // log(x) = k*LN2 + log(m)
        return static_cast<double>(k) * LN2 + 2.0 * sum;
    }

    double pow(double base, double exp) {
        // --- Casos Especiales ---
        if (exp == 0.0) {
            return 1.0; // b^0 = 1
        }
        if (base == 1.0) {
            return 1.0; // 1^e = 1
        }
        if (base == 0.0) {
            if (exp > 0.0) {
                return 0.0; // 0^(+e) = 0
            } else {
                return POSITIVE_INFINITY; // 0^(-e) = 1/0 = +Inf
            }
        }

        // --- Optimización para Exponentes Enteros ---
        // Es mucho más rápido y preciso calcular x^3 como x*x*x
        // que como exp(3 * log(x)).
        // También maneja correctamente bases negativas (p.ej. (-2)^3 = -8)

        // Comprobar si 'exp' es un entero
        if (fmod(exp, 1.0) == 0.0) {
            long long int_exp = static_cast<long long>(exp);
            double result = 1.0;

            if (int_exp > 0) {
                for (long long i = 0; i < int_exp; ++i) {
                    result *= base;
                }
            } else {
                // Exponente negativo (p.ej., 2^-3 = 1 / (2^3))
                for (long long i = 0; i < -int_exp; ++i) {
                    result /= base;
                }
            }
            return result;
        }

        // --- Caso General (Exponente no entero) ---
        // Si la base es negativa y el exponente no es entero,
        // el resultado es un número complejo (p.ej. (-4)^0.5 = 2i).
        if (base < 0.0) {
            return NOT_A_NUMBER;
        }

        // Usamos la identidad: b^e = exp(e * log(b))
        return Math::exp(exp * Math::log(base));
    }

    double sqrt(double x) {
        // --- Casos Especiales ---
        if (x < 0.0) {
            return NOT_A_NUMBER; // Raíz de negativo
        }
        if (x == 0.0) {
            return 0.0;
        }

        // --- Método de Newton-Raphson ---
        // Queremos encontrar la raíz de f(g) = g^2 - x
        // La fórmula de iteración es: g_next = g - f(g) / f'(g)
        // f'(g) = 2*g
        // g_next = g - (g^2 - x) / (2*g)
        // g_next = g - g/2 + x/(2*g)
        // g_next = 0.5 * (g + x / g)

        double guess = x;
        // Evitar empezar en 0; 1.0 es una suposición segura.
        if (guess == 0.0 || guess < 1.0) {
            guess = 1.0;
        }

        double old_guess = 0.0;

        // Iterar hasta que la suposición converja (deje de cambiar)
        for (int i = 0; i < 50; ++i) { // Límite de seguridad de 50 iteraciones
            old_guess = guess;
            guess = 0.5 * (guess + x / guess);

            if (guess == old_guess) {
                break; // Convergencia alcanzada
            }
        }
        return guess;
    }

    double cbrt(double x) {
        if (x == 0.0) {
            return 0.0;
        }

        // --- Manejo de Signo ---
        // cbrt(-x) = -cbrt(x). Calculamos sobre el valor absoluto
        double sign = 1.0;
        double val = x;
        if (x < 0.0) {
            sign = -1.0;
            val = -x; // val = fabs(x)
        }

        // --- Método de Newton-Raphson ---
        // Queremos encontrar la raíz de f(g) = g^3 - val
        // f'(g) = 3*g^2
        // g_next = g - (g^3 - val) / (3*g^2)
        // g_next = (2*g + val / (g*g)) / 3.0

        double guess = val / 3.0; // Suposición inicial
        if (guess == 0.0) {
            guess = 1.0;
        }

        double old_guess = 0.0;

        for (int i = 0; i < 50; ++i) { // Límite de seguridad de 50 iteraciones
            old_guess = guess;

            // Evitar división por cero si la suposición llega a 0
            if (guess == 0.0) guess = 0.0001;

            guess = (2.0 * guess + val / (guess * guess)) / 3.0;

            if (guess == old_guess) {
                break;
            }
        }

        // Aplicar el signo original
        return sign * guess;
    }

    double nth_root(double x, int n) {
        // --- Casos Especiales ---
        if (n == 0) {
            return NOT_A_NUMBER; // Raíz 0-ésima no definida
        }
        if (x == 0.0) {
            return 0.0;
        }
        if (n == 1) {
            return x;
        }
        // Usar las funciones especializadas en caso n = 2 y n = 3
        if (n == 2) {
            return sqrt(x);
        }
        if (n == 3) {
            return cbrt(x);
        }

        // --- Manejo de Dominio y Signo ---
        // Comprobar si n es par
        bool is_even = (Math::fmod(static_cast<double>(n), 2.0) == 0.0);

        // Raíz par de un número negativo no es real
        if (is_even && x < 0.0) {
            return NOT_A_NUMBER;
        }

        // Raíz impar de un número negativo: raiz_n(-x) = -raiz_n(x)
        double sign = 1.0;
        double val = x;
        if (!is_even && x < 0.0) {
            sign = -1.0;
            val = -x; // val = fabs(x)
        }

        // --- Método de Newton-Raphson Generalizado ---
        // Queremos encontrar la raíz de f(g) = g^n - val
        // f'(g) = n * g^(n-1)
        // g_next = g - (g^n - val) / (n * g^(n-1))
        // g_next = ((n-1)*g + val / g^(n-1)) / n

        double guess = val / static_cast<double>(n);
        if (guess == 0.0) {
            guess = 1.0;
        }

        double old_guess = 0.0;
        double n_double = static_cast<double>(n);

        for (int i = 0; i < 100; ++i) { // Iteraciones extra para raíces altas
            old_guess = guess;

            // Math::pow(base, exp)
            double g_pow = Math::pow(guess, static_cast<double>(n - 1));

            // Evitar división por cero
            if (g_pow == 0.0) {
                guess = 0.0001;
                continue;
            }

            guess = ((n_double - 1.0) * guess + val / g_pow) / n_double;

            if (guess == old_guess) {
                break;
            }
        }

        return sign * guess;
    }

    double fmod(double x, double y) {
        // La división por cero es indefinida, retornar 0 como valor seguro
        if (y == 0.0) {
            return 0.0;
        }

        // static_cast a long long trunca la parte decimal,
        // que es el comportamiento requerido para fmod (truncamiento hacia cero)
        auto int_part = static_cast<long long>(x / y);
        return x - (static_cast<double>(int_part) * y);
    }

    template<typename T>
    T factorial(unsigned int n) {
        T result = 1;
        for (unsigned int i = 2; i <= n; ++i)
            result *= i;
        return result;
    }

}