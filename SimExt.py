# -*- coding: utf-8 -*-
"""
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
"""

"""
SimExt - Simulador Externo para Python
 
Este el archivo principal de la librería. Las fuciones que se busca implemen-
tar son:
     > Configurar el modelo del simulador a través de JSON o similar
     > Compilar el programa de simulación para generar el ejecutable
     > Gestionar la ejecución del modelo recibiendo señales de entrada y 
       comunicando señales de salida
     
"""

import subprocess
import os
from contextlib import contextmanager


# Función para cambiar temporalmente de directorio
@contextmanager
def cambia_directorio(dir_objetivo):
    dir_inicial = os.getcwd()
    os.chdir(dir_objetivo)
    try:
        yield
    finally:
        os.chdir(dir_inicial)


class Simulador():
    """
        Clase de objeto Simulador.
        
    """
    
    def __init__(self):
        """
        Función de inicialización.
        
        > Por ahora crea lo básico para el modelo del motor de CD.
        
        > Crea proceso del ejecutable del simulador.
        
        > En el futuro, en función de la configuración en el archivo JSON
          debería crear las variables necesarios para la bitácora, el paso
          de integración, el periodo de muestreo y el número de entradas y
          salidas
            
        """
        
        # Obtiene directorio raiz de la biblioteca
        dir_raiz = os.path.dirname(os.path.abspath(__file__))
        
        # Crea proceso de simulación
        with cambia_directorio(dir_raiz):
            self.prSIM = subprocess.Popen(['Simulador\Simulador.exe'], text=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        self.prSIM_ent = self.prSIM.stdin
        self.prSIM_sal = self.prSIM.stdout
        
        # Bitácora de simulación
        self.bitacora = {'Observacion': {'ia':  [],
                                         'w':   [] }, 
                         'Entradas':    {'Vt':  [],
                                         'Tlc': [],
                                         'Tlw': [] },
                         'Tiempo':      [] }
        
        # Número de señales de entrada
        self._nEnt = 3
        # Número de señales de salida
        self._nSal = 2
        
        # Paso de integración
        self._h = 0.01
        # Tiempo de muestreo (número de pasos de integración)
        self._kh = 1
        
        
    def avanza(self, entrada:list):
        """
        Función para avanzar en la simulación kh pasos de integración que 
        equivalen a un paso de muestreo.

        Parámetros
        ----------
        entrada : list
            Lista (vector) de variables de entrada.
            (Debe coincidir con la configuración de la simulación.)
            
        Retorno
        -------
        obs_sim : list
            Lista (vector) de variables de observación de la simulación

        """
        # Actualiza registro en la bitácora (observacion actual, entrada actual)
        self._actualiza_bitacora(entrada)
        
        # Avanza kh pasos de integración
        for i in range(self._kh):
            
            # Envía variables de entrada con 8 dígitos significativos
            for var_ent in entrada:
                self.prSIM_ent.write(f'{var_ent:.8g}\n')
            self.prSIM_ent.flush()
            
            # Recibe variables de observación
            self._obs_sim = []
            for _ in range(self._nSal):
                self._obs_sim.append(self.prSIM_sal.readline())
        
        # Asegura que los valores sean de tipo flotante
        self._obs_sim = [float(var) for var in self._obs_sim]
        
        # Comunica nuevas variables de observación
        return self._obs_sim
        
            
    def inicio(self):
        """
        Función para iniciar simulación y obtener valores iniciales de las
        variables de observación.
        
        Retorno
        -------
        obs_sim : list
            Lista (vector) de variables de observación de la simulación

        """
        self._obs_sim = []
        
        # Recibe variables de observación
        for _ in range(self._nSal):
            self._obs_sim.append(self.prSIM_sal.readline())
            
        # Asegura que los valores sean de tipo flotante
        self._obs_sim = [float(var) for var in self._obs_sim]
    
        return self._obs_sim
    
    
    def termina(self):
        """
        Función para terminar proceso de simulación.

        """
        self.prSIM.terminate()
    
    
    def _actualiza_bitacora(self, entrada:list):
        """
        Función para actualizar bitácora de simulación.
        
        Por ahora es el caso específico para el motor pero en el futuro se debería
        hacer el caso general según la configuración en el archivo JSON
        
        Parámetros
        ----------
        entrada : list
            Lista (vector) de variables de entrada.
            (Debe coincidir con la configuración de la simulación.)

        """
        self.bitacora['Observacion']['ia'].append(self._obs_sim[0]) 
        self.bitacora['Observacion']['w'].append(self._obs_sim[1])
        self.bitacora['Entradas']['Vt'].append(entrada[0])
        self.bitacora['Entradas']['Tlc'].append(entrada[1])
        self.bitacora['Entradas']['Tlw'].append(entrada[2])
        if(len(self.bitacora['Tiempo']) == 0):
            self.bitacora['Tiempo'].append(0)
        else:
            self.bitacora['Tiempo'].append(self.bitacora['Tiempo'][-1] + self._h*self._kh)
        

        
