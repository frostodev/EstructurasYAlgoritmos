#ifndef TRIGONOMETRY_H
#define TRIGONOMETRY_H

namespace Math {

    /**
     * @brief Calcula el seno de un ángulo dado en radianes.
     *
     * @param x El ángulo de entrada en radianes.
     * @return El seno de x, un valor en el rango [-1.0, 1.0].
     * @note Complejidad: O(1), basado en una serie de Taylor de iteración fija.
     */
    double sin(double x);

    /**
     * @brief Calcula el coseno de un ángulo dado en radianes.
     *
     * @param x El ángulo de entrada en radianes.
     * @return El coseno de x, un valor en el rango [-1.0, 1.0].
     * @note Complejidad: O(1), ya que reutiliza la implementación O(1) de sin().
     */
    double cos(double x);

    /**
     * @brief Calcula la tangente de un ángulo dado en radianes.
     *
     * @param x El ángulo de entrada en radianes.
     * @return La tangente de x.
     * @note En sistemas IEEE 754, esto devolverá 'inf' (infinito) o '-inf'
     * si x es un múltiplo impar de PI/2, lo cual es matemáticamente
     * correcto.
     * @note Complejidad: O(1), ya que realiza una llamada a sin() y una a cos().
     */
    double tan(double x);

    /**
     * @brief Calcula el arcoseno (inversa del seno) de un valor.
     * @param x El valor de entrada, en el rango [-1.0, 1.0].
     * @return El ángulo en radianes, en el rango [-PI/2, PI/2].
     * @note Retorna NOT_A_NUMBER (NaN) si x está fuera del rango [-1, 1].
     * @note Complejidad: O(1), basado en una identidad que llama a atan() (que es O(1)).
     */
    double asin(double x);

    /**
     * @brief Calcula el arcocoseno (inversa del coseno) de un valor.
     * @param x El valor de entrada, en el rango [-1.0, 1.0].
     * @return El ángulo en radianes, en el rango [0, PI].
     * @note Retorna NOT_A_NUMBER (NaN) si x está fuera del rango [-1, 1].
     * @note Complejidad: O(1), basado en una identidad que llama a asin() (que es O(1)).
     */
    double acos(double x);

    /**
     * @brief Calcula el arcotangente (inversa de la tangente) de un valor.
     * @param x El valor de entrada.
     * @return El ángulo en radianes, en el rango [-PI/2, PI/2].
     * @note Complejidad: O(1), basado en una serie de Taylor de iteración fija.
     */
    double atan(double x);

    /**
     * @brief Calcula el arcotangente de y/x, usando los signos de ambos
     * argumentos para determinar el cuadrante correcto.
     *
     * @param y La coordenada y.
     * @param x La coordenada x.
     * @return El ángulo en radianes, en el rango [-PI, PI].
     * @note Complejidad: O(1), ya que consiste en comprobaciones O(1) y una llamada a atan() (que es O(1)).
     */
    double atan2(double y, double x);

}

#endif //TRIGONOMETRY_H