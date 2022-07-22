import ctypes as c
import random

mydll = c.cdll.LoadLibrary("C:\\Users\\skarm\\Desktop\\Trabajos\\QueVotan\\optiPython\\Dll1.dll")
#result1= mydll.main("./ingles.json",40,0.30,0.14,1234)
arch="ingles.json"
prototype = c.CFUNCTYPE (
    c.c_void_p,      # Return type.
    c.c_char_p,
    c.c_int,
    c.c_float,
    c.c_float,
    c.c_int)

coalicion = prototype (("CoalicionGM", mydll))

coalicion(c.c_char_p(arch.encode('utf-8')),c.c_int(40),c.c_float(0.30),c.c_float(0.14),c.c_int(1234))


print(c.c_float.in_dll(mydll, "resultadoF").value)
print(c.c_int.in_dll(mydll, "resultadoI").value)


#func = mydll.CoalicionGM

#func.restype = c.py_object

#print(func(c.c_char_p(arch.encode('utf-8')),40,0.30,0.14,1234))





