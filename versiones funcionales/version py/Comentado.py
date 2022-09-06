import json
import random
import matplotlib.pyplot as plt
import time



####################inicio de variables############################
tiempo = time.time()
datos=[]
cromosoma=[]
fitnessPob=[]

cromosoma_nuevo=[]
fitnessPob_nuevo=[]

fitnessEvol=[]


itera=[]
crossover12=[]
crossover21=[]
####################fin de variables################################

with open('ejemplo2.json', encoding='utf-8') as dat: #carga de datos
    data = json.load(dat)

def disEuc(x1,y1,x2,y2):#calcula la distancia euclidiana
    return ((x2-x1)**2+(y2-y1)**2)**(1/2)

def suma(array):#suma los elementos de un arreglo
    suma=0
    for i in range(len(array)):
        suma=suma+array[i]
    return suma

def suma_bool(array):#suma los True de un arreglo
    suma = 0
    for i in range(len(array)):
        if array[i]:
            suma = suma+1
    return suma

def evalsol(pos): #evalua la solucion y retorna el fitness
    suma=0
    for i in range(0,len(pos)-1):
        for j in range(i+1,len(pos)):
            suma = suma+disEuc(data['diputados'][pos[i]]['coordX'], data['diputados'][pos[i]]['coordY'], data['diputados'][pos[j]]['coordX'], data['diputados'][pos[j]]['coordY'])
    return suma

def smallest_greater(seq,value):# Funcion para encontrar el valor de un vector ordenado (seq) mas pequeño pero mayor a un valor dado (value)
    i=0
    flag=True
    while(flag):
        i=i+1
        if(seq[i]>value):
            return(i)
        else:
            if(i==len(seq)):
                return(None)


def sample(limite,cant):#genera una muestra aleatoria de una lista
    valores=[]
    i=0
    repetido=False
    while i<cant:
        valor=random.randint(0,limite-1)
        for j in range(len(valores)):
            if valor==valores[j]:repetido=True
        if repetido==False:
            valores.append(valor)
            i=i+1
        repetido=False    
    return valores

def sort(array):#ordena un arreglo de menor a mayor
    n = len(array)
    for i in range(n):
        already_sorted = True
        for j in range(n - i - 1):
            if array[j] > array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]
                already_sorted = False
        if already_sorted:
            break
    return array

def order(array):#ordena 1 arreglo de arreglos de menor a mayor (array[1] ordena a array[0])
    n = len(array[1])
    for i in range(n):
        already_sorted = True
        for j in range(n-i-1):
            if array[1][j] > array[1][j+1]:
                array[0][j], array[0][j + 1] = array[0][j + 1], array[0][j]
                array[1][j], array[1][j + 1] = array[1][j + 1], array[1][j]
                already_sorted = False
        if already_sorted:
            break
    return array[0],array[1]

def notin(array1, array2):#retorna los elementos de array1 que no esten en array2
    result=[]
    flag=False
    for i in range(len(array1)):
        for j in range(len(array2)):
            if array1[i] == array2[j]:
                flag=True
        if flag==False:
            result.append(array1[i])
        flag=False
    return result

def in_boolean(array1,array2):#retorna True o False, si el elemento de array1 estan en array2
    result=[]
    flag=False
    for i in range(len(array1)):
        for j in range(len(array2)):
            if array1[i] == array2[j]:
                result.append(True)
                flag=True
        if flag==False:
            result.append(False)
        flag=False
    return result

def minimo(array):#retorna el minimo de un arreglo
    for i in range(len(array)-1):
        if array[i] < array[i+1]:
            mini = array[i]
        else: 
            mini = array[i+1]
    return mini

def maximo(array):#retorna el maximo de un arreglo
    for i in range(len(array)-1):
        if array[i] > array[i+1]:
            maxi = array[i]
        else: 
            maxi = array[i+1]
    return maxi

def which_max(array):#retorna el indice del maximo de un arreglo
    for i in range(len(array)-1):
        if array[i] > array[i+1]:
            maxi = array[i]
        else: 
            maxi = array[i+1]
    for i in range(len(array)):
        if array[i] == maxi:
            return i

def which(array):#retorna los indices de los elementos de un arreglo que sean True
    result=[]
    for i in range(len(array)):
        if array[i]:
            result.append(i)
    return result

def crear_arreglo(numero):#crea un arreglo de longitud numero de 0 hasta numero
    array=[]
    for i in range(numero):
        array.append(i)
    return array

def repetidos(lista):#busca repetidos en una lista
    for i in lista:
        if lista.count(i) > 1:
            lista.pop(lista.index(i))
    return lista

def sample_arreglo(array,cant):#genera un arreglo de muestras aleatorias de un arreglo
    valores=[]
    i=0
    repetido=False
    while i<cant:
        valor=random.choice(array)
        for j in range(len(valores)):
            if valor==valores[j]:repetido=True
        if repetido==False:
            valores.append(valor)
            i=i+1
        repetido=False    
    return valores

for i in range(len(data['diputados'])):#crea una lista con las coordenadas de los diputados
    linea=[]
    linea.append(data['diputados'][i]['coordX'])
    linea.append(data['diputados'][i]['coordY'])
    datos.append(linea)

n = len(datos)
quorum = 74
m = 40
pmutacion_threshold = 0.2

cromosoma.append(sort(sample(n, quorum)))#crea el primer cromosoma aleatorio
fitnessPob.append(evalsol(cromosoma[0]))#calcula el fitness del cromosoma


for i in range(1,m):#crea el resto de cromosomas aleatorios y sus respectivos fitness
    cromosoma.append(sort(sample(n, quorum)))
    fitnessPob.append(evalsol(cromosoma[i]))

cromosoma,fitnessPob=order([cromosoma,fitnessPob])#ordena los cromosomas de menor a mayor fitness

pr=0.1#probabilidad
p=[0.1]#arreglo de probabilidades
cump=[0.1]#arrego de probabilidades acumuladas


for i in range(1,m):#calcula el resto de las probabilidades y las probabilidades acumuladas
    p.append(pr*(1-pr)**(i))
    cump.append(suma(p))

cump[m-1]=1#corrige la ultima probabilidad acumulada

max_k = 10*(m-21)#maximo de iteraciones

i=0
k=0
it=0


itera.append(it)#registro de iteraciones
fitnessEvol.append(fitnessPob[0])#refistro de la evolucion de los fitness

while k<max_k:
    it=it+1
    cual1 = smallest_greater(cump, random.random())#selecciona una probabilidad acumulada aleatoria más pequeña
    cual2 = smallest_greater(cump, random.random())#selecciona una probabilidad acumulada aleatoria más pequeña
    while cual1==cual2:#estas no pueden ser iguales
        cual2 = smallest_greater(cump, random.random())
    cromosoma1=list(cromosoma[cual1])#selecciona un cromosoma en base a la probabilidad acumulada
    cromosoma2=list(cromosoma[cual2])#selecciona un cromosoma en base a la probabilidad acumulada

    disimilar12=notin(cromosoma1,cromosoma2)#selecciona los elementos que estan en 1 pero no en 2
    disimilar21=notin(cromosoma2,cromosoma1)#selecciona los elementos que estan en 2 pero no en 1

    crossovern = sample(minimo([len(disimilar12),len(disimilar21)]),1)#selecciona un elemento aleatorio

    selecCrossover12=sample(len(disimilar12), crossovern[0])#selecciona crossovern cantidad de elementos aleatorios no más grande que el largo de disimilar12
    selecCrossover21=sample(len(disimilar21), crossovern[0])#selecciona crossovern cantidad de elementos aleatorios no más grande que el largo de disimilar21

    crossover12=[]
    crossover21=[]

    for a in range(len(selecCrossover12)):
        crossover12.append(disimilar12[selecCrossover12[a]])#guarda los elementos que estan en 1 pero no en 2
        crossover21.append(disimilar21[selecCrossover21[a]])#guarda los elementos que estan en 2 pero no en 1

    crossover12 = sort(repetidos(crossover12))#elimina los elementos repetidos y los ordena
    crossover21 = sort(repetidos(crossover21))#elimina los elementos repetidos y los ordena

    cual12 = which(in_boolean(cromosoma1, crossover12))#obtiene los indices de los elementos que estan en cromosoma1 y en crossover12
    cual21 = which(in_boolean(cromosoma2, crossover21))#obtiene los indices de los elementos que estan en cromosoma2 y en crossover21

    for a in range(len(cual12)):
        cromosoma1[cual12[a]]=crossover21[a]#cambia los elementos que estan en cromosoma1 por los elementos que estan en crossover21
    for a in range(len(cual21)):
        cromosoma2[cual21[a]]=crossover12[a]#cambia los elementos que estan en cromosoma2 por los elementos que estan en crossover12

    cromosoma1=sort(cromosoma1)#ordena los elementos de cromosoma1
    cromosoma2=sort(cromosoma2)#ordena los elementos de cromosoma2
    
    pmutacion = random.random()#genera un numero aleatorio entre 0 y 1
    if pmutacion<pmutacion_threshold:#si el numero aleatorio es menor que el umbral de mutacion
        cual_sacar = sample(quorum,1) #selecciona un elemento aleatorio no más grande que quorum
        not_in_cromosoma1 = notin(crear_arreglo(n), cromosoma1)#selecciona los elementos que no estan en cromosoma1
        cual_introducir = sample_arreglo(not_in_cromosoma1,1)#selecciona un arreglo de largo 1 de elementos que no estan en cromosoma1 de manera aleatoria
        cromosoma1[cual_sacar[0]] = cual_introducir[0]#crea la mutación
        cromosoma1=sort(cromosoma1)#ordena los elementos de cromosoma1
    
    ##################lo mismo de antes pero con el cromosoma2############################
    pmutacion = random.random()
    if pmutacion<pmutacion_threshold:
        cual_sacar = sample(quorum,1)
        not_in_cromosoma2 = notin(crear_arreglo(n), cromosoma2)
        cual_introducir = sample_arreglo(not_in_cromosoma2,1)
        cromosoma2[cual_sacar[0]] = cual_introducir[0]
        cromosoma2=sort(cromosoma2)
    #######################################################################################

    if i==0:#si es la primera iteracion
        cromosoma_nuevo.append(list(cromosoma1))#guarda el cromosoma1 en un arreglo de cromosomas
        cromosoma_nuevo.append(list(cromosoma2))#guarda el cromosoma2 en un arreglo de cromosomas
        fitnessPob_nuevo.append(evalsol(cromosoma1))#guarda el fitness del cromosoma1 en un arreglo de fitness
        fitnessPob_nuevo.append(evalsol(cromosoma2))#guarda el fitness del cromosoma2 en un arreglo de fitness

    else:#si no es la primera iteracion
        flag2=True
        idxpop = 0
        while flag2:
            if suma_bool(in_boolean(cromosoma1, cromosoma_nuevo[idxpop]))==quorum:#si el cromosoma1 esta en el arreglo de cromosomas en la posición idxpop y la suma de estos es igual al quorum
                cual_sacar = sample(quorum,1)#selecciona un elemento aleatorio no más grande que quorum
                not_in_cromosoma1 = notin(crear_arreglo(n), cromosoma1)#selecciona los elementos que no estan en cromosoma1
                cual_introducir = sample_arreglo(not_in_cromosoma1,1)#selecciona un arreglo de largo 1 de elementos que no estan en cromosoma1 de manera aleatoria
                cromosoma1[cual_sacar[0]] = cual_introducir[0]#crea una mutación
                cromosoma1=sort(cromosoma1)#ordena los elementos de cromosoma1
                idxpop = 0#reinicia el contador de cromosomas
            else:
                if idxpop==len(cromosoma_nuevo)-1:#si el contador de cromosomas es igual al largo del arreglo de cromosomas-1
                    flag2=False#termina el ciclo
                else:
                    idxpop=idxpop+1

        ##################lo mismo de antes pero con el cromosoma2############################
        flag2=True
        idxpop = 0
        while flag2:
            if suma_bool(in_boolean(cromosoma2, cromosoma_nuevo[idxpop]))==quorum:
                cual_sacar = sample(quorum,1)
                not_in_cromosoma2 = notin(crear_arreglo(n), cromosoma2)
                cual_introducir = sample_arreglo(not_in_cromosoma2,1)
                cromosoma2[cual_sacar[0]] = cual_introducir[0]
                cromosoma2=sort(cromosoma2)
                idxpop = 0
            else:
                if idxpop==len(cromosoma_nuevo)-1:
                    flag2=False
                else:
                    idxpop=idxpop+1
        #######################################################################################
        cromosoma_nuevo.append(list(cromosoma1))#guarda el cromosoma1 en el arreglo de cromosomas
        cromosoma_nuevo.append(list(cromosoma2))#guarda el cromosoma2 en el arreglo de cromosomas
        fitnessPob_nuevo.append(evalsol(cromosoma1))#guarda el fitness del cromosoma1 en el arreglo de fitness
        fitnessPob_nuevo.append(evalsol(cromosoma2))#guarda el fitness del cromosoma2 en el arreglo de fitness

    i=i+1

    if i == m/2:#si el contador de iteraciones es igual a la mitad del largo de la poblacion
        i=0#reinicia el contador de iteraciones
        cromosoma_nuevo,fitnessPob_nuevo=order([cromosoma_nuevo,fitnessPob_nuevo])#ordena los cromosomas y los fitness de acuerdo al fitness

        flag2=True
        flag3=False
        idxpop=0

        while flag2:
            if suma_bool(in_boolean(cromosoma[0], cromosoma_nuevo[idxpop]))==quorum:#si el cromosoma1 esta en el arreglo de cromosomas en la posición idxpop y la suma de estos es igual al quorum
                flag2=False#termina el ciclo
                flag3=True
            else:
                idxpop=idxpop+1#incrementa el contador de cromosomas
            if idxpop>m-1:#si el contador de cromosomas es mayor al largo de la poblacion-1
                flag2=False#termina el ciclo
        if flag3==False:
            peor = which_max(fitnessPob_nuevo)#selecciona el cromosoma con el fitness peor
            cromosoma_nuevo[peor] = cromosoma[0]#el cromosoma con el fitness peor se reemplaza por el cromosoma1
            fitnessPob_nuevo[peor]= fitnessPob[0]#el fitness del cromosoma con el fitness peor se reemplaza por el fitness del cromosoma1
            cromosoma_nuevo,fitnessPob_nuevo=order([cromosoma_nuevo,fitnessPob_nuevo])#ordena los cromosomas y los fitness de acuerdo al fitness

            #esto no deberia ser necesario
            if len(cromosoma)>m:#si el largo del arreglo de cromosomas es mayor al largo de la poblacion (no deberia pasar nunca)
                cromosoma_nuevo=[]#reinicia el arreglo de cromosomas
                fitnessPob_nuevo=[]#reinicia el arreglo de fitness
                for i in range(len(cromosoma)):
                    cromosoma_nuevo.append(list(cromosoma[i]))#guarda los cromosomas originales en un nuevo arreglo de cromosomas
                    fitnessPob_nuevo.append(fitnessPob[i])#guarda los fitness originales en un nuevo arreglo de fitness
            #ver si se cambia en un futuro
            p_nuevo=list(p)#guarda el arreglo de probabilidades original en un nuevo arreglo
            cump_nuevo=list(cump)#guarda el arreglo de cumprobabilidades original en un nuevo arreglo

            if fitnessPob_nuevo[0]==fitnessPob[0]:#si el fitness del mejor cromosoma de la nueva población es igual al fitness del mejor cromosoma de la población original
                k=k+1#incrementa el contador de iteraciones
        else:#si la flag3 es verdadera
            p_nuevo=list(p)#guarda el arreglo de probabilidades original en un nuevo arreglo
            cump_nuevo=list(cump)#guarda el arreglo de cumprobabilidades original en un nuevo arreglo
            if fitnessPob_nuevo[0]==fitnessPob[0]:#si el fitness del mejor cromosoma de la nueva población es igual al fitness del mejor cromosoma de la población original
                k=k+1#incrementa el contador de iteraciones

        cromosoma=list(cromosoma_nuevo)#guarda el arreglo de cromosomas nuevo en el arreglo de cromosomas originales
        fitnessPob=list(fitnessPob_nuevo)#guarda el arreglo de fitness nuevo en el arreglo de fitness originales
        p=list(p_nuevo)#guarda el arreglo de probabilidades nuevo en el arreglo de probabilidades originales
        cump=list(cump_nuevo)#guarda el arreglo de cumprobabilidades nuevo en el arreglo de cumprobabilidades originales

        cromosoma_nuevo=[]#reinicia el arreglo de cromosomas nuevo
        fitnessPob_nuevo=[]#reinicia el arreglo de fitness nuevo
        p_nuevo=[]#reinicia el arreglo de probabilidades nuevo
        cump_nuevo=[]#reinicia el arreglo de cumprobabilidades nuevo

        for j in range(round(m/2),m):#recorre el arreglo de cromosomas de la mitad de la poblacion hasta el final
            cromosoma_cambio = sort(sample(n, quorum))#selecciona aleatoriamente una cantidad de cromosomas de la poblacion
            fitness_cambio = evalsol(cromosoma_cambio)#evalua el fitness de los cromosomas seleccionados
            cromosoma[j]=cromosoma_cambio#guarda los cromosomas seleccionados en el arreglo de cromosomas
            fitnessPob[j]=fitness_cambio#guarda los fitness de los cromosomas seleccionados en el arreglo de fitness

        cromosoma,fitnessPob=order([cromosoma,fitnessPob])#ordena los cromosomas y los fitness de acuerdo al fitness
        p[0]=pr#resetea la probabilidad de mutación con su valor original (0.1)
        cump[0]=pr#resetea la probabilidad acumulada de mutación con su valor original (0.1)
        for j in range(1,m):#recorre el arreglo de probabilidades
            p[j]=pr*(1-pr)**(j)#calcula la probabilidad de mutación
            cump[j]=suma(p[0:j])#calcula la probabilidad acumulada de mutación
        cump[m-1]=1#la probabilidad acumulada de mutación del ultimo cromosoma es 1
        i=0#reinicia el contador de iteraciones
        itera.append(it)#guarda el numero de iteraciones en un nuevo arreglo
        fitnessEvol.append(fitnessPob[0])#guarda el fitness del mejor cromosoma en un nuevo arreglo
tiempo=time.time()-tiempo#calcula el tiempo de ejecucion
print(tiempo)#imprime el tiempo de ejecucion

print(cromosoma[0])
print(fitnessPob[0])
plt.plot([x for x in itera],[x for x in fitnessEvol])#grafica el numero de iteraciones vs el fitness del mejor cromosoma
plt.show()