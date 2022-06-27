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
 * Programa principal - SimExt
 ########################################################################### */

#include "../Modelos/MotorCD.h"
#include <iostream>
using namespace std;

// Función principal del programa
int main(){
    // Variable de entradas
    struct Entradas ent;
    
    // Crea modelo y configura parámetros: J, B, K1, K2
    // 5HP, 240V, 1750rpm, 300Vfield
    // Ra0.6, L0.012, Rf240, Lf120, Laf0.984, J1.0, B0.05
    // K1=K2=ifLaf=(300/240)*0.984=1.23
    struct Parametros params = {
        0.6,    // R
        0.012,  // L
        1.0,    // J
        0.05,   // B
        1.23,   // K1
        1.23    // K2
    };
    Motor_Derivacion motor(params);

    // Configura paso y método de integración
    motor.Configuracion(0.01, EULER_PROG);  // 10ms, Euler progresivo
    //motor.Configuracion(0.01, RUNGEKUTTA4);  // 10ms, Runge-Kutta

    // Espera entrada y ejecuta paso de integración
    while(true){

        // Comunica estado actual [ia, w]
        cout << motor.edo.ia << endl;
        cout << motor.edo.w << endl;

        // Recibe entradas [Vt, Tlc, Tlw]
        cin >> ent.Vt >> ent.Tlc >> ent.Tlw;

        // Ejecuta paso de integración
        motor.Paso_Integracion(ent);

    }
    return 0;
}