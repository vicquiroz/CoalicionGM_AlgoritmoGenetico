 library(ggplot2)
library(vioplot)
library(corrplot)
data<-read.csv(file = 'valores_verif_RH0750158.csv',header = FALSE)

m<-as.numeric(data$V1[1])
pTresh<-as.numeric(data$V2[1])
pr<-as.numeric(data$V3[1])

datos<-as.data.frame(data)
names(datos) = c("m","p_tresh","pr","seed","iteraciones","fitness")
# Graficos Violin Fitness vs Iteraciones
#par(mfrow = c(1, 2))
GrafViolin<-function(val2,title){
  vioplot(datos$fitness~val2,col=2:length(levels(data$m)),xlab=title,ylab="Fitness")
  stripchart(datos$fitness~val2,vertical=TRUE,method="jitter",pch=19,add=TRUE,col=3:8)
  vioplot(datos$iteraciones~val2,col=2:length(levels(data$m)),xlab="M",ylab="Iteraciones")
  stripchart(datos$iteraciones~val2,vertical=TRUE,method="jitter",pch=19,add=TRUE,col=3:8)
}


vioplot(datos$fitness,horizontal=TRUE,xlab="Fitness",ylab="")
vioplot(datos$iteraciones,horizontal=TRUE,xlab="Iteraciones",ylab="")
# Graficos M
#GrafViolin(datos$m,"M")
# Graficos P Tresh
#GrafViolin(datos$p_tresh,"P_Tresh")
# Graficos PR
#GrafViolin(datos$pr,"Pr")

TablaCombFit<-function(mval,ptreshval,prval){
  datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  combtemp<-table(datostemp$fitness)/1000
  #if(combtemp[1]>0.85){
    print(paste("Combinacion:  m=",as.character(mval),"  pTresh=",as.character(ptreshval)," pr=",as.character(prval)))
    print(combtemp)
    print("")
    #print(datostemp$seed)
  #}
}
PromIter<-function(mval,ptreshval,prval){
  # datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  iters<-mean(datostemp$iteraciones)
  print(iters)
}
print("Para Fitness")
mV=c(m)
pTreshV=c(pTresh)
prV=c(pr)
for(mValue in mV){
  for(pTreshValue in pTreshV){
    for(prValue in prV){
      TablaCombFit(mValue,pTreshValue,prValue)
    }
  }
}

subdata1<-datos[which(datos$m==m & datos$p_tresh==pTresh & datos$pr==pr),]

print("Calculo Varianza")
print(var(subdata1$fitness))

print("Calculo Desviacion Estandar")
pca<-prcomp(subdata1[6],scale=FALSE)     
print(pca$sdev)
print("")


print("Para Iter")
mV=c(m)
pTreshV=c(pTresh)
prV=c(pr)
for(mValue in mV){
  for(pTreshValue in pTreshV){
    for(prValue in prV){
      PromIter(mValue,pTreshValue,prValue)
    }
  }
}
print("Calculo Varianza")
print(var(subdata1$iteraciones))

print("Calculo Desviacion Estandar")
pca<-prcomp(subdata1[5],scale=FALSE)     
print(pca$sdev)
print("")