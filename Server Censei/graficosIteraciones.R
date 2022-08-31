 library(ggplot2)
library(vioplot)
library(corrplot)
data<-read.csv(file = 'valores_verif_RH0750158-final-3000.csv',header = FALSE)

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

library(viridis)
library(viridisLite)
ggplot(datos, aes(x = fitness, y = -0.5)) +
  
  # horizontal boxplots & density plots
  geom_boxplot(aes(fill = fitness) ,alpha = 0.5) +
  geom_density(aes(x = fitness), inherit.aes = FALSE,fill="#feb24c",alpha=0.6,col="#800026") +
  geom_jitter(aes(x=fitness,col=fitness),height=0.36,alpha=0.4)+
  # vertical lines at Q1 / Q2 / Q3
  #stat_boxplot(geom = "vline", aes(xintercept = ..xlower..)) +
  #stat_boxplot(geom = "vline", aes(xintercept = ..xmiddle..)) +
  #stat_boxplot(geom = "vline", aes(xintercept = ..xupper..)) +
  
  #facet_grid(fitness ~ .) +
  ylab("")+
  scale_colour_gradient(low="#fed976",high="#800026")+theme(
    panel.background = element_rect(fill = "gray",
                                    colour = "black",
                                    size = 0.5, linetype = "solid")
  )

ggplot(datos, aes(x = fitness, y = iteraciones)) +
  #geom_boxplot(aes(fill = fitness), alpha = 0.5) +
  #geom_density(aes(x = fitness), inherit.aes = FALSE) +
  geom_point(aes(col=fitness))+
  scale_colour_viridis_c()
# Graficos M
#GrafViolin(datos$m,"M")
# Graficos P Tresh
#GrafViolin(datos$p_tresh,"P_Tresh")
# Graficos PR
#GrafViolin(datos$pr,"Pr")

TablaCombFit<-function(mval,ptreshval,prval){
  datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  combtemp<-table(datostemp$fitness)/3000
  #if(combtemp[1]>0.85){
    print(paste("Combinacion:  m=",as.character(mval),"  pTresh=",as.character(ptreshval)," pr=",as.character(prval)))
    print(combtemp)
    print("")
    #print(datostemp$seed)
  #}
}
PromIter<-function(mval,ptreshval,prval){
  #datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  iters<-mean(datos$iteraciones)
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