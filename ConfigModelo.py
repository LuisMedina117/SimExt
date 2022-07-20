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
Configuración de modelo
 
Este script construye el modelo de la simulación en C para ser compilado. 
Las fuciones que se busca implementar son:
     > Configurar el modelo del simulador a través de JSON o similar
     > Compilar el programa de simulación para generar el ejecutable
     
"""

"""
> El archivo JSON contiene una lista de los modelos a simular
> Cada entrada de la lista tiene:
    > Identificador del modelo y tipo
    > Parámetros del modelo
    > Método de integración (si no es igual que el del resto de la sim.)
> La última entrada de la lista contiene la configuración de la simulación:
    > Paso de integración base (h)
    > Periodo de muestreo (# de pasos)
    > Variables para la bitácora

"""

import json
from os.path import exists


def ConfiguraModelo():
    
    # Carga datos de modelos implementados
    with open('./Simulador/Modelos/Libreria.json', 'r') as f:
        Libreria = json.load(f)
        f.close()
    
    # Carga datos de métodos de integración implementados
    with open('./Simulador/Modelos/Metodos.json', 'r') as f:
        Metodos = json.load(f)
        f.close()
    
    # Verifica que el archivo de configuración existe
    if not exists('./Simulador/Configuracion.json'):
        raise FileNotFoundError('Archivo de configuración Configuracion.json no encontrado.')
        
    # Carga configuración del modelo de simulación
    with open('./Simulador/Configuracion.json', 'r') as f:
        Mod_config = json.load(f)
        f.close()
    
    # Separa cofiguración de la simulación
    Sim_config = Mod_config.pop(-1)
    
    # Verifica si el elemento final de la lista es de tipo Config_Sim
    if Sim_config['Modulo'] != 'Config_Sim':
        raise TypeError('Modulo incorrecto al final de la lista, debe ser Config_Sim.')
        
    # Verifica elementos de configuración de simulación
    # Paso de integración (tipo float)
    if not isinstance(Sim_config['Config']['h'], float):
        raise TypeError(f"El paso de integración debe ser float. Se obtuvo {type(Sim_config['Config']['h'])}")
    # Múltiplo de paso de integración (tipo int)
    if not isinstance(Sim_config['Config']['kh'], int):
        raise TypeError('El múltiplo del paso de integración debe ser int. Se obtuvo '
                        + f"{type(Sim_config['Config']['kh'])}")
    # Método de integración (en la lista de métodos implementados)
    if Sim_config['Config']['metodo'] not in Metodos:
        raise NotImplementedError(f"El método {Sim_config['Config']['metodo']} no está implementado. "
                                  + 'Revisar en la documentación los métodos de integración implementados.')
    
    # Verifica que los elementos de la lista Mod_config tengan tipo correcto y modelo correcto. Si no, indica el ID del elemento y cuál es el error
    
    
    
    
    # Verifica que las variables de la bitácora coincidan con la clase del elemento
    # Algo como: elemento.variables_bitacora in Libreria['elemento.Modulo']['elemento.Clase'].keys()
        
    


# Si se ejecuta este script, llama a la función para generar el modelo
if __name__ == "__main__":
    ConfiguraModelo()
