// pch.h: este es un archivo de encabezado precompilado.
// Los archivos que se muestran a continuación se compilan solo una vez, lo que mejora el rendimiento de la compilación en futuras compilaciones.
// Esto también afecta al rendimiento de IntelliSense, incluida la integridad del código y muchas funciones de exploración del código.
// Sin embargo, los archivos que se muestran aquí se vuelven TODOS a compilar si alguno de ellos se actualiza entre compilaciones.
// No agregue aquí los archivos que se vayan a actualizar con frecuencia, ya que esto invalida la ventaja de rendimiento.
#pragma once

#ifdef  PROJECTLIBRARY_EXPORTS 
#define PROJECTLIBRARY __declspec(dllexport)
#else
#define PROJECTLIBRARY __declspec(dllimport)
#endif



// agregue aquí los encabezados que desea precompilar
extern "C" PROJECTLIBRARY float CoalicionGM(char* nombre, int m, float pmutacion_treshold, float pr, int seed);
