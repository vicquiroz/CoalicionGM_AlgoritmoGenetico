import numpy as np
import math
import ctypes as c
import time
import random
from gradient_free_optimizers import BayesianOptimizer

seed = math.trunc(time.time())
print(seed)

mydll = c.cdll.LoadLibrary("C:\\Users\\skarm\\Desktop\\Trabajos\\QueVotan\\optiPython\\Dll1.dll")
#result1= mydll.main("./ingles.json",40,0.30,0.14,1234)
arch="ingles.json"
prototype = c.CFUNCTYPE (
    c.c_float,      # Return type.
    c.c_char_p,
    c.c_int,
    c.c_float,
    c.c_float,
    c.c_int)

coalicion = prototype (("CoalicionGM", mydll))

#val = coalicion(c.c_char_p(arch.encode('utf-8')),c.c_int(40),c.c_float(0.30),c.c_float(0.14),c.c_int(1234))


#seed = time = 0
def ackley_function(pos_new):
    m = pos_new["m"]
    pt = pos_new["p_tresh"]
    pr = pos_new["pr"]
    #n iteraciones
    # 30/60              20/60              10/60
    #min fitness, desviacion estandar, iteraciones promedio
    score = coalicion(c.c_char_p(arch.encode('utf-8')),c.c_int(m),c.c_float(pr),c.c_float(pt),c.c_int(seed))
    return -score


search_space = {
    "m": np.arange(20, 60, 2),
    "p_tresh": np.arange(0.1, 0.5, 0.1),
    "pr": np.arange(0.1, 0.5, 0.1),
}

opt = BayesianOptimizer(search_space)
opt.search(ackley_function, n_iter=150)