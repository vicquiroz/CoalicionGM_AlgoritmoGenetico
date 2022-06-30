library(ggplot2)
library(vioplot)
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
  print(paste("Combinacion:  m=",as.character(mval),"  pTresh=",as.character(ptreshval)," pr=",as.character(prval)))
  print(combtemp)
  print("")
  
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



