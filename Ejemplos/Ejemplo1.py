# -*- coding: utf-8 -*-
"""
Código de ejemplo(prueba) para SimExt.
 
Motor de CD con control de velocidad.
 
"""

#%% Crea objeto del simulador

# Añade directorio para importar biblioteca
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from SimExt import Simulador as SimExt
import matplotlib.pyplot as plt
import numpy as np


simulador = SimExt()

#%% Define controlador de velocidad

class control_Velocidad():
    def __init__(self):
        self.Kp = 6
        self.Ki = 30
        self.Kd = 0.5
        self.x1 = 0
        self.x2 = 0
        self.h = 0.01
        
    def accion(self, referencia, medicion):
        # Calcula acción de control
        resultado = self.Kp*(referencia-medicion) + self.Ki*self.x1 \
                    + self.Kd*(self.x2-medicion)
        # Actualiza variable de estado itegral
        self.x1 += self.h*(referencia-medicion)
        # Guarda valor anterior para acción derivativa
        self.x2 = medicion        
        # Retorna resultado
        return resultado

#%% Simulación
T = 5
h = 0.01
kh = 1

# Objeto del control de velocidad
control = control_Velocidad()

# Primer paso de simulación
observacion = simulador.inicio()

# Ciclo de simulación
for i in range(int(T/(h*kh))):
    # Avanza en la simulación. Entrada: [Vt, Tlc, Tlw]
    if i < 2.5/h:
        # observacion = simulador.avanza([240,0,0])
        observacion = simulador.avanza([control.accion(1750*2*np.pi/60,observacion[1]),0,0])
    else:
        # observacion = simulador.avanza([240,20,0])
        observacion = simulador.avanza([control.accion(1750*2*np.pi/60,observacion[1]),40,0])

simulador.termina()
        
#%% Genera gráficas
plt.plot(np.array(simulador.bitacora['Tiempo']), 
         np.array(simulador.bitacora['Observacion']['w'])*60/(2*np.pi))