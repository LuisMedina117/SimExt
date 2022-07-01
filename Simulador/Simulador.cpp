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
    
    // Crea modelo y configura parámetros: 
    // Pn=10HP, n=1750rpm Vt=240V, Vf=300V, Tn=40Nm
    // Ra1.086, L0.01216, Rf180, Lf71.47, Laf0.6458, J0.04251, B0.003406
    // K1=K2=ifLaf=(300/180)*0.6458=1.0733
    struct Parametros params = {
        1.086,      // R (mayor R para más variación de velocidad con carga)
        0.01216,    // L
        0.04251,    // J
        0.003406,   // B
        1.07633,   // K1
        1.07633    // K2
    };
    Motor_Derivacion motor(params);

    // Configura paso y método de integración
    motor.Configuracion(0.001, EULER_PROG);  // 1ms, Euler progresivo
    //motor.Configuracion(0.001, RUNGEKUTTA4);  // 1ms, Runge-Kutta

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