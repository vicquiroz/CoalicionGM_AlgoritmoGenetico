library(geometry)
library(ggplot2)
library(remotes)
#install_github("cmartin/ggConvexHull")
library(ggConvexHull)

evalsol <- function(solucion) {
# Funci�n para evaluar la suma de distancias de una soluci�n dada
  mat_dist_sol <-
    dist(datos[solucion,1:2],
         method = "euclidean",
         diag = F,
         upper = F)
  return(sum(mat_dist_sol))
}


smallest_greater <- function(seq, value){
# Funci�n para encontrar el valor de un vector ordenado (seq) m�s peque�o pero mayor a un valor dado (value)
  i<-0
  flag<-T
  while(flag){
    i<-i+1
    if(seq[i]>value){
      return(i)
    } else {
      if(i==length(seq)){
        return(NA)
      }
    }
  }
}

discent <- function(puntos,centroide){
  return(sqrt((centroide[1]-puntos$x)^2+(centroide[2]-puntos$y)^2))
}


"%!in%" <- Negate("%in%")

#dev.off()

datos <- read.table("Matriz50.csv",
                    sep = ",",
                    header = T,
                    row.names = 1)

colnames(datos) <- c("x", "y")
n <- nrow(datos)
quorum <- 24
m <- 30
pmutacion_threshold <- 0.2
#--------------------------------------------------------------
### Generacion de poblacion inicial
poblacion <- data.frame(cromosoma=I(list(sort(sample(1:n, quorum)))),fitness=0)
poblacion$fitness<-evalsol(poblacion$cromosoma[1][[1]])

for(i in 2:m){
  cromosoma <- sort(sample(1:n, quorum))
  fitness <- evalsol(cromosoma)
  poblacion<-rbind(poblacion,list(I(list(cromosoma=cromosoma)),fitness))
}

#Se ordena la poblaci�n por su fitness.
poblacion<-poblacion[order(poblacion$fitness),]

# Creaci�n del vector de probabilidad de selecci�n de cada cromosoma de la poblaci�n
p<-.1 #Probabilidad de selecci�n de mejor cromosoma,
poblacion$cump<-poblacion$p<-p
for(i in 2:(m-1)){
  poblacion$p[i]<-p*(1-p)^(i-1) #Las probabilidades de selecci�n de los dem�s cromosomas.
  poblacion$cump[i]<-sum(poblacion$p[1:i])
}
poblacion$cump[m]<-1

#Par�metros de ejecuci�n
max_k <- 10*(m-21) #El m�ximo n�mero de iteraciones sin cambios. Criterio de parada.
i<-k<-it<-0
#--------------------------------------------------------------
#Ejecuci�n
evol_fitness <- data.frame(it=it,fitness=poblacion$fitness[1])
while(k<max_k){
  it<-it+1
  #Selecci�n de dos cromosomas
  cual1 <- smallest_greater(poblacion$cump,runif(1))
  cual2 <- smallest_greater(poblacion$cump,runif(1))
  while(cual2==cual1){
    cual2 <- smallest_greater(poblacion$cump,runif(1))
  }
  cromosoma1<-poblacion$cromosoma[cual1]
  cromosoma2<-poblacion$cromosoma[cual2]
  if(length(cromosoma1$cromosoma)==0){
    names(cromosoma1)<-"cromosoma"
  }
  if(length(cromosoma2$cromosoma)==0){
    names(cromosoma2)<-"cromosoma"
  }
  
  #Ver genes disimilares para elegir cuales intercambiar
  disimilar12 <- cromosoma1$cromosoma[cromosoma1$cromosoma %!in% cromosoma2$cromosoma]
  disimilar21 <- cromosoma2$cromosoma[cromosoma2$cromosoma %!in% cromosoma1$cromosoma]
  
  #Seleccionar cuales genes intercambiar
  crossovern <- sort(sample(1:min(length(disimilar12),length(disimilar21)),1))
  crossover_12<-sort(disimilar12[sample(1:length(disimilar12),crossovern)])
  crossover_21<-sort(disimilar21[sample(1:length(disimilar21),crossovern)])
  
  cual_12 <- which(cromosoma1$cromosoma %in% crossover_12)
  cual_21 <- which(cromosoma2$cromosoma %in% crossover_21)
  
  #Intercambiar genes seleccionados
  cromosoma1$cromosoma[cual_12] <- crossover_21
  cromosoma2$cromosoma[cual_21] <- crossover_12
  
  cromosoma1$cromosoma <- sort(cromosoma1$cromosoma)
  cromosoma2$cromosoma <- sort(cromosoma2$cromosoma)
  
  #Verificar si hay una mutaci�n en el cromosoma 1
  pmutacion <- runif(1)
  if(pmutacion<pmutacion_threshold){
    cual_sacar <- sample(1:quorum,1)
    not_in_cromosoma1 <- (1:n)[1:n %!in% cromosoma1$cromosoma]
    cual_introducir <- sample(not_in_cromosoma1,1)
    cromosoma1$cromosoma[cual_sacar] <- cual_introducir
    cromosoma1$cromosoma <- sort(cromosoma1$cromosoma)
  }
  
  #Verificar si hay una mutaci�n en el cromosoma 2
  pmutacion <- runif(1)
  if(pmutacion<pmutacion_threshold){
    cual_sacar <- sample(1:quorum,1)
    not_in_cromosoma2 <- (1:n)[1:n %!in% cromosoma2$cromosoma]
    cual_introducir <- sample(not_in_cromosoma2,1)
    cromosoma2$cromosoma[cual_sacar] <- cual_introducir
    cromosoma2$cromosoma <- sort(cromosoma2$cromosoma)
  }
  
  #Crear o actualizar la poblaci�n nueva
  if(i==0){
    #browser()
    poblacion_nueva <- data.frame(cromosoma=I(list(cromosoma1$cromosoma)),fitness=evalsol(cromosoma1$cromosoma))
    poblacion_nueva <- rbind(poblacion_nueva,list(I(list(cromosoma2$cromosoma)),evalsol(cromosoma2$cromosoma)))
  } else {
    #Verificar que no los cromosomas no pertezcan a la poblaci�n nueva.
    #Si pertenecen, mutarlos hasta que no pertenezcan.
    flag2 <- T
    idxpop<-1
    #browser()
    while(flag2) {
      if(sum(cromosoma1$cromosoma %in% poblacion_nueva$cromosoma[idxpop][[1]])==quorum){
        #browser()
        cual_sacar <- sample(1:quorum,1)
        not_in_cromosoma1 <- (1:n)[1:n %!in% cromosoma1$cromosoma]
        cual_introducir <- sample(not_in_cromosoma1,1)
        cromosoma1$cromosoma[cual_sacar] <- cual_introducir
        cromosoma1$cromosoma <- sort(cromosoma1$cromosoma)
        idxpop=1
      } 
      else {
        if(idxpop==nrow(poblacion_nueva)){
          flag2 <- F
        } else {
          idxpop<-idxpop+1
        }
      }
    }
      
    flag2 <- T
    idxpop<-1
    while(flag2) {
      if(sum(cromosoma2$cromosoma %in% poblacion_nueva$cromosoma[idxpop][[1]])==quorum){
        #browser()
        cual_sacar <- sample(1:quorum,1)
        not_in_cromosoma2 <- (1:n)[1:n %!in% cromosoma2$cromosoma]
        cual_introducir <- sample(not_in_cromosoma2,1)
        cromosoma2$cromosoma[cual_sacar] <- cual_introducir
        cromosoma2$cromosoma <- sort(cromosoma2$cromosoma)
        idxpop=1
      } else {
        if(idxpop==nrow(poblacion_nueva)){
          flag2 <- F
        } else {
          idxpop<-idxpop+1
        }
      }
    }
    #Agregar los cromosomas a la poblaci�n nueva  
    poblacion_nueva <- rbind(poblacion_nueva,list(I(list(cromosoma1$cromosoma)),evalsol(cromosoma1$cromosoma)))
    poblacion_nueva <- rbind(poblacion_nueva,list(I(list(cromosoma2$cromosoma)),evalsol(cromosoma2$cromosoma)))
    
  }
  
  i<-i+1
  
  
  
  
  #Sustituir la poblaci�n original con la nueva
  if(i==m/2){
    #browser()
    i<-0
    poblacion_nueva <- poblacion_nueva[order(poblacion_nueva$fitness),]
    #Verificar que el mejor de la poblaci�n anterior no est� en esta poblaci�n
    flag2<-T
    flag3<-F
    idxpop<-1
    #####################################aqui nos quedamos####################################################################################
    #####################################aqui nos quedamos####################################################################################
    while(flag2){
      if(sum(poblacion$cromosoma[1][[1]] %in% poblacion_nueva$cromosoma[idxpop][[1]])==quorum){
        flag2<-F
        flag3<-T
      } else {
        idxpop<-idxpop+1
      }
      if(idxpop>m){
        flag2<-F
      }
    }
    #browser()
    #Si el mejor de la poblaci�n anterior no est� en esta poblaci�n, 
    #sustituir el peor de esta poblaci�n por el mejor de la anterior
    if(flag3==F){
      peor <- which.max(poblacion_nueva$fitness)
      poblacion_nueva[peor,]<-poblacion[1,1:2]
      poblacion_nueva <- poblacion_nueva[order(poblacion_nueva$fitness),]
      if(nrow(poblacion_nueva)>m){
        #browser()
        poblacion_nueva<-poblacion[1:m,]
      }
      poblacion_nueva$p <- poblacion$p
      poblacion_nueva$cump <- poblacion$cump
      if(poblacion_nueva$fitness[1]==poblacion$fitness[1]){
        k<-k+1
      }
    } else {
      poblacion_nueva$p <- poblacion$p
      poblacion_nueva$cump <- poblacion$cump
      if(poblacion_nueva$fitness[1]==poblacion$fitness[1]){
        k<-k+1
      }
    }
    poblacion <- poblacion_nueva
    
    #Sustituir los peores con nuevos
    for(j in round(1*m/2):m){
     cromosoma <- sort(sample(1:n, quorum))
     fitness <- evalsol(cromosoma)
     poblacion[j,1:2]<-list(I(list(cromosoma=cromosoma)),fitness)
    }
    #browser()
    poblacion<-poblacion[order(poblacion$fitness),]
    poblacion$cump<-poblacion$p<-p
    for(i in 2:(m)){
      poblacion$p[i]<-p*(1-p)^(i-1)
      poblacion$cump[i]<-sum(poblacion$p[1:i])
    }
    poblacion$cump[m]<-1
    i<-0
    evol_fitness <- rbind(evol_fitness,data.frame(it=it,fitness=poblacion$fitness[1]))
  }
}


#--------------------------------------------------------------


#Imprimir soluci�n encontrada
datos$coaliciong<-0
solucion <- poblacion$cromosoma[1][[1]]
datos$coaliciong[solucion]<-1
datos$coaliciong<-factor(datos$coaliciong)
library(ggplot2)
print(ggplot(data=datos)+
        geom_point(aes(x=x,y=y,col=coaliciong))+
        geom_convexhull(data=datos[which(datos$coaliciong==1),1:2],aes(x=x,y=y),alpha = 0.2,col="cyan",fill="cyan")+
        annotate("text",label=paste("FO =",evalsol(solucion)),x=0.5,y=0.8))


#Verificar que no existan puntos no seleccionados al interior del CH
#Si hay, cambiarlos y plotear nueva soluci�n


#--------------------------------------------------------------





#--------------------------------------------------------------
#Plotear la evoluci�n del fitness
#evol_fitness <- rbind(evol_fitness,data.frame(it=it+1,fitness=evalsol(solucion)))
print(ggplot(data=evol_fitness)+
  geom_line(aes(it,fitness)))