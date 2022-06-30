library(ggplot2)
library(vioplot)
library(corrplot)
data<-read.csv(file = 'pruebaTresh04.csv')
datos<-as.data.frame(data)
names(datos) = c("m","p_tresh","pr","seed","iteraciones","fitness")
# Graficos Violin Fitness vs Iteraciones
#par(mfrow = c(1, 2))
GrafViolin<-function(val2,title){
  vioplot(datos$fitness~val2,col=2:length(levels(data$m)),xlab=title,ylab="Fitness")
  stripchart(datos$fitness~val2,vertical=TRUE,method="jitter",pch=19,add=TRUE,col=3:8)
  #vioplot(datos$iteraciones~val2,col=2:length(levels(data$m)),xlab="M",ylab="Iteraciones")
  #stripchart(datos$iteraciones~val2,vertical=TRUE,method="jitter",pch=19,add=TRUE,col=3:8)
}


#vioplot(datos$fitness,horizontal=TRUE,xlab="Fitness",ylab="")
#vioplot(datos$iteraciones,horizontal=TRUE,xlab="Iteraciones",ylab="")
# Graficos M
#GrafViolin(datos$m,"M")
# Graficos P Tresh
#GrafViolin(datos$p_tresh,"P_Tresh")
# Graficos PR
#GrafViolin(datos$pr,"Pr")

TablaComb<-function(mval,ptreshval,prval){
  datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  combtemp<-table(datostemp$fitness)/50
  if(combtemp[1]>0.85){
    print(paste("Combinacion:  m=",as.character(mval),"  pTresh=",as.character(ptreshval)," pr=",as.character(prval)))
    print(combtemp)
    print("")
    print(datostemp$seed)
  }
}

mV=c(40)
pTreshV=c(0.4)
prV=c(0.14)
for(mValue in mV){
  for(pTreshValue in pTreshV){
    for(prValue in prV){
      TablaComb(mValue,pTreshValue,prValue)
    }
  }
}

subdata1<-datos[which(datos$m==40 & datos$p_tresh==0.3 & datos$pr==0.14),]
subdata2<-datos[which(datos$m==50 & datos$p_tresh==0.1 & datos$pr==0.15),]

print("Calculo Varianza")
print("Subdata M=40, PTresh=0.3, Pr=0.14")
print(var(subdata1$fitness))
print("")
print("Subdata M=50, PTresh=0.1, Pr=0.15")
print(var(subdata2$fitness))
print("Varianza a mano")

print("Calculo Desviacion Estandar")
print("Subdata M=40, PTresh=0.3, Pr=0.14")
pca<-prcomp(subdata1[6],scale=FALSE)     
print(pca$sdev)
print("")
print("Subdata M=50, PTresh=0.1, Pr=0.15")
pca<-prcomp(subdata2[6],scale=FALSE)     
print(pca$sdev)
print("")

