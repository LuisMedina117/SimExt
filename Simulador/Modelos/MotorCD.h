/*
MIT License

Copyright (c) 2022 Luis Medina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ############################################################################
 * Biblioteca del modelo de motor de CD - SimExt
 ########################################################################### */

#ifndef MOTORCD_H
#define MOTORCD_H

// Estructura de parámetros del motor
struct Parametros{
    double R;       // Resistencia del devanado de armadura [Ohm]
    double L;       // Inductancia del devanado de armadura [Henry]
    double J;       // Momento de inercia del rotor [kg·m^2]
    double B;       // Coeficiente de fricción [kg·m^2/s]
    double K1;      // Constante de par-corriente [Nm/A]
    double K2;      // Constante de velocidad-fem [V/(rad/s)]
};

// Estructura de variables de estado del motor
struct Estado{
    double ia;      // Corriente del devanado de armadura [A]
    double w;       // Velocidad angular del rotor [rad/s]
};

// Estructura de variables de entrada del motor
struct Entradas{
    double Vt;      // Tensión aplicada [V]
    double Tlc;     // Par de carga constante [Nm]
    double Tlw;     // Para de carga proporcional a velocidad [Nms]
};

// Método de integración
enum Metodo {EULER_PROG, TRAPEZOIDAL, RUNGEKUTTA4};

// Clase del motor de CD en derivación
class Motor_Derivacion{
    public:
        // Variables
        struct Estado edo;      // Variables de estado
        // Funciones
        Motor_Derivacion(struct Parametros);
        void Configuracion(double, enum Metodo);
        void Paso_Integracion(struct Entradas);

    private:
        // Variables
        struct Parametros PARAM;    // Parámetros del modelo
        enum Metodo METODO;         // Método de integración
        double PASO;                // Paso de integración
        // Funciones
        void Euler_progresivo(struct Entradas);
        void Trapezoidal(struct Entradas);
        void RungeKutta4(struct Entradas);

};

#endif