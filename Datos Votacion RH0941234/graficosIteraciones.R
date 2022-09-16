 library(ggplot2)
library(vioplot)
library(corrplot)
data<-read.csv(file = 'valores_verif_votacionRH0941234.csv',header = FALSE)

m<-as.numeric(data$V1[1])
pTresh<-as.numeric(data$V2[1])
pr<-as.numeric(data$V3[1])

datos<-as.data.frame(data)
names(datos) = c("seed","iteraciones","fitness","tiempo")

library(viridis)
library(viridisLite)
ggplot(datos, aes(x = fitness, y = -0.5)) +
  geom_boxplot(aes(fill = fitness) ,alpha = 0.5) +
  geom_density(aes(x = fitness), inherit.aes = FALSE,fill="#feb24c",alpha=0.6,col="#800026") +
  geom_jitter(aes(x=fitness,col=fitness),height=0.36,alpha=0.2,size=0.3)+
  ylab("")+
  scale_colour_gradient(low="#fed976",high="#800026")+
  theme(aspect.ratio = 4/4,
    panel.background = element_rect(fill = "gray",
                                    colour = "black",
                                    size = 0.5, linetype = "solid")
  )

ggplot(datos, aes(x = fitness, y = iteraciones)) +
  geom_point(aes(col=fitness))+
  scale_colour_viridis_c()

TablaCombFit<-function(){
  datostemp<-datos
  combtemp<-table(datostemp$fitness)/10000
  #if(combtemp[1]>0.85){
    print(paste("Combinacion:"))
    print(combtemp)
    print("")
    #print(datostemp$seed)
  #}
}
PromIter<-function(){
  #datostemp<-datos[which(datos$m==mval & datos$p_tresh==ptreshval & datos$pr==prval),]
  iters<-mean(datos$iteraciones)
  print(iters)
}
print("Para Fitness")
TablaCombFit()

#subdata1<-datos[which(datos$m==m & datos$p_tresh==pTresh & datos$pr==pr),]
subdata1<-datos
print("Calculo Varianza")
print(var(subdata1$fitness))

print("Calculo Desviacion Estandar")
pca<-prcomp(subdata1[3],scale=FALSE)     
print(pca$sdev)
print("")


print("Para Iter")
 PromIter()

print("Calculo Varianza")
print(var(subdata1$iteraciones))

print("Calculo Desviacion Estandar")
pca<-prcomp(subdata1[2],scale=FALSE)     
print(pca$sdev)
print("")