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
/*
Motor_Derivacion::Motor_Derivacion(struct Parametros Prms){
    // Constructor: define parámetros del motor
    PARAM = Prms;
}

void Motor_Derivacion::Configuracion(double paso, enum Metodo metodo){
    // Configura paso y método de integración
    PASO = paso;
    METODO = metodo;
}

void Motor_Derivacion::Paso_Integracion(struct Entradas ent){
    // Ejecuta paso de integración según la configuración

    switch (METODO){
        case EULER_PROG:
            Euler_progresivo(ent);
            break;
        case TRAPEZOIDAL:
            Trapezoidal(ent);
            break;
        case RUNGEKUTTA4:
            RungeKutta4(ent);
            break;
        default:
            break;
    }
}

void Motor_Derivacion::Euler_progresivo(struct Entradas ent){
    // Ejecuta método de integración Euler regresivo

    struct Estado nuevo;    // Nuevo estado

    // ia[k+1] = ia[k] + h*(Vt[k]-Ea[k]-R*ia[k])/L
    nuevo.ia = edo.ia + PASO*(ent.Vt - PARAM.K2*edo.w - PARAM.R*edo.ia)/PARAM.L;

    // w[k+1] = w[k] + h*(Te[k]-Tlc[k]-Tlw[k]*w[k]-B*w[k])/J
    nuevo.w = edo.w + PASO*(PARAM.K1*edo.ia - ent.Tlc - ent.Tlw*edo.w - PARAM.B*edo.w)/PARAM.J;

    // Actualiza variables de estado
    edo.ia = nuevo.ia;
    edo.w = nuevo.w;
}

void Motor_Derivacion::RungeKutta4(struct Entradas ent){
    // Ejecuta método de integración Runge-Kutta de 4to orden

    struct Estado aux[4];   // Variables auxiliares de integración

    // k1 = f(t[k], x[k])
    aux[0].ia = (ent.Vt - PARAM.K2*edo.w - PARAM.R*edo.ia)/PARAM.L;
    aux[0].w = (PARAM.K1*edo.ia - ent.Tlc - (ent.Tlw + PARAM.B)*edo.w)/PARAM.J;

    // k2 = f(t[k]+h/2, x[k]+k1*h/2)
    aux[1].ia = (ent.Vt - PARAM.K2*(edo.w+aux[0].w*PASO/2) - PARAM.R*(edo.ia+aux[0].ia*PASO/2))/PARAM.L;
    aux[1].w = (PARAM.K1*(edo.ia+aux[0].ia*PASO/2) - ent.Tlc - (ent.Tlw + PARAM.B)*(edo.w+aux[0].w*PASO/2))/PARAM.J;

    // k3 = f(t[k]+h/2, x[k]+k2*h/2)
    aux[2].ia = (ent.Vt - PARAM.K2*(edo.w+aux[1].w*PASO/2) - PARAM.R*(edo.ia+aux[1].ia*PASO/2))/PARAM.L;
    aux[2].w = (PARAM.K1*(edo.ia+aux[1].ia*PASO/2) - ent.Tlc - (ent.Tlw + PARAM.B)*(edo.w+aux[1].w*PASO/2))/PARAM.J;

    // k4 = f(t[k]+h, x[k]+k3*h)
    aux[3].ia = (ent.Vt - PARAM.K2*(edo.w+aux[2].w*PASO) - PARAM.R*(edo.ia+aux[2].ia*PASO))/PARAM.L;
    aux[3].w = (PARAM.K1*(edo.ia+aux[2].ia*PASO) - ent.Tlc - (ent.Tlw + PARAM.B)*(edo.w+aux[2].w*PASO))/PARAM.J;

    // Actualiza variables de estado
    // x[k+1] = x[k] + h*(k1+2*k2+2*k3+k4)/6
    edo.ia = edo.ia + PASO*(aux[0].ia + 2*aux[1].ia + 2*aux[2].ia + aux[3].ia)/6;
    edo.w = edo.w + PASO*(aux[0].w + 2*aux[1].w + 2*aux[2].w + aux[3].w)/6;

}

void Motor_Derivacion::Trapezoidal(struct Entradas ent){
    // Ejecuta método de integración Regla trapezoidal
    ;
}*/

#endif