#include "Trigonometry.h"

#include "MathUtils.h"
#include "Constants.h"

namespace Math {

    double sin(double x) {
        // --- Reducción de Rango ---
        // Primero, reducir el ángulo al rango (-2*PI, 2*PI) usando fmod
        x = fmod(x, TWO_PI);

        // Luego, plegar el rango a [-PI, PI].
        // Si x está en (PI, 2*PI), lo mapeamos a (-PI, 0)
        if (x > PI) {
            x -= TWO_PI;
        }
        // Si x está en (-2*PI, -PI), lo mapeamos a (0, PI)
        else if (x < -PI) {
            x += TWO_PI;
        }

        // Finalmente, plegar el rango a [-PI/2, PI/2] usando simetría
        // sin(x) = sin(PI - x) para x en (PI/2, PI]
        if (x > PI_OVER_TWO) {
            x = PI - x;
        }
        // sin(x) = sin(-PI - x) para x en [-PI, -PI/2)
        // Nota: sin(-PI - x) = -sin(PI + x). Y sin(x) = -sin(PI + x)
        else if (x < -PI_OVER_TWO) {
            x = -PI - x;
        }

        // En este punto, x está garantizado de estar en [-PI/2, PI/2],
        // donde la serie de Taylor converge más rápidamente

        // --- Cálculo de la Serie de Taylor ---
        // sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
        //
        // Calculamos esto iterativamente.
        // Sea termino_n el n-ésimo término (empezando en n=0 para x)
        // termino_0 = x
        // termino_1 = -x^3/3! = termino_0 * (-x^2) / (2*3)
        // termino_2 = +x^5/5! = termino_1 * (-x^2) / (4*5)
        //
        // Paso iterativo general:
        // termino_n = termino_{n-1} * (-x^2) / ( (2n) * (2n+1) )

        const double x_squared = x * x;
        double current_term = x;
        double sum = x;

        // Empezar el bucle desde n=1
        // Usamos un número fijo (pero alto) de iteraciones O una comprobación de convergencia
        // Una comprobación de convergencia es más robusta
        for (int n = 1; n < 20; ++n) { // 20 iteraciones es más que suficiente para precisión 'double'

            // Calcular la parte del denominador para esta iteración
            const double denominator = (2.0 * n) * (2.0 * n + 1.0);

            // Calcular el siguiente término a partir del término anterior
            current_term = current_term * (-1.0) * x_squared / denominator;

            // Almacenar la suma *antes* de añadir el nuevo término
            const double old_sum = sum;

            // Añadir el nuevo término a la suma total
            sum += current_term;

            // --- Comprobación de Convergencia ---
            // Si añadir el (muy pequeño) término actual no cambia el
            // valor de 'sum' (debido a los límites de precisión de punto flotante),
            // hemos convergido y podemos detenernos antes (trucazo)
            if (sum == old_sum) {
                break;
            }
        }

        return sum;
    }

    double cos(double x) {
        // Usamos la identidad de desplazamiento de fase:
        // cos(x) = sin(x + PI/2)
        //
        // Esto es extremadamente eficiente y robusto porque reutiliza
        // toda la lógica de reducción de rango y la serie de Taylor
        // de alta precisión que ya está implementada en sin()
        return sin(x + PI_OVER_TWO);
    }

    double tan(double x) {
        // Usamos la identidad fundamental por definición:
        // tan(x) = sin(x) / cos(x)
        //
        // No se requiere un manejo especial para la división por cero
        // Si cos(x) retorna 0.0 (lo cual sucederá en múltiplos
        // impares de PI/2 debido a la reducción de rango),
        // la aritmética de punto flotante estándar (IEEE 754)
        // producirá 'inf' (infinito) o '-inf', que es el resultado
        // matemáticamente correcto.
        return sin(x) / cos(x);
    }

    double asin(double x) {
        // --- Casos de Dominio ---
        if (x > 1.0 || x < -1.0) {
            return NOT_A_NUMBER;
        }
        // --- Casos de Borde ---
        if (x == 1.0) {
            return PI_OVER_TWO;
        }
        if (x == -1.0) {
            return -PI_OVER_TWO;
        }

        // --- Identidad con atan ---
        // Usamos la identidad: asin(x) = atan(x / sqrt(1 - x*x))
        double denominator = Math::sqrt(1.0 - x * x);
        return atan(x / denominator);
    }

    double acos(double x) {
        // --- Casos de Dominio ---
        // asin ya maneja (x > 1.0 || x < -1.0)

        // --- Identidad con asin ---
        // Usamos la identidad: acos(x) = PI/2 - asin(x)
        return PI_OVER_TWO - asin(x);
    }

    double atan(double x) {
        // --- Casos Especiales ---
        if (x == 0.0) return 0.0;
        if (x == 1.0) return PI_OVER_FOUR; // PI/4
        if (x == -1.0) return -PI_OVER_FOUR; // -PI/4

        // Serie de Taylor para atan(x) = x - x^3/3 + x^5/5 - x^7/7 + ...
        //
        // Paso iterativo:
        // termino_n = (-1)^n * x^(2n+1) / (2n+1)
        //
        // termino_0 = x / 1
        // termino_1 = -x^3 / 3 = (x) * (-x^2) * 1/3
        // termino_2 = +x^5 / 5 = (-x^3/3) * (-x^2) * 3/5
        //
        // termino_n = termino_{n-1} * (-x^2) * (2n-1) / (2n+1)
        auto atan_series = [](double x) {
            const double x_squared = x * x;
            double sum = x;
            double current_term = x;

            for (int n = 1; n < 25; ++n) {      // 25 iteraciones debería ser suficiente.
                const double old_sum = sum;
                current_term *= (-1.0) * x_squared;
                sum += current_term / (2.0 * n + 1.0);
                if (sum == old_sum) break;
            }
            return sum;
        };

        // --- Reducción de Rango ---
        // La serie de Taylor converge rápido solo en [-1, 1].
        // Usamos identidades para mapear valores fuera de este rango.

        if (Math::fabs(x) > 1.0) {
            // Usar identidad:
            // atan(x) = PI/2 - atan(1/x)       (para x > 1)
            // atan(x) = -PI/2 - atan(1/x)      (para x < -1)

            if (x > 1.0) {
                return PI_OVER_TWO - atan_series(1.0 / x);
            } else { // x < -1.0
                return -PI_OVER_TWO - atan_series(1.0 / x);
            }
        }

        // --- Cálculo de la Serie ---
        // Para |x| < 1
        return atan_series(x);
    }

    double atan2(double y, double x) {
        // --- Manejo de Casos Especiales (ejes) ---
        if (x == 0.0) {
            if (y > 0.0) {
                return PI_OVER_TWO; // 90 grados
            }
            if (y < 0.0) {
                return -PI_OVER_TWO; // -90 grados
            }
            return 0.0; // atan2(0, 0) es 0
        }

        // --- Cálculo por Cuadrantes ---
        // Cuadrantes 1 y 4 (x > 0)
        if (x > 0.0) {
            return atan(y / x);
        }

        // Cuadrante 2 (x < 0, y >= 0)
        if (x < 0.0 && y >= 0.0) {
            return atan(y / x) + PI;
        }

        // Cuadrante 3 (x < 0, y < 0)
        if (x < 0.0 && y < 0.0) {
            return atan(y / x) - PI;
        }

        // Caso final (y == 0, x < 0) -> 180 grados
        // (Este caso es cubierto por la lógica del Cuadrante 2)
        return 0.0; // No debería llegar aquí, pero C++ requiere un retorno
    }

}