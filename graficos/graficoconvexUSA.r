
#--------------------------------------------------------------
library(jsonlite)
library(ggplot2)
library(ggConvexHull)

datosJson<-fromJSON("ingles.json")
datosJson<-as.data.frame(datosJson$rollcalls$votes)
lData<-length(datosJson$x)
datosO<-data.frame(x=datosJson$x,y=datosJson$y,ID=c(1:lData))


resultados<- fromJSON("resultados.json")
resultadosDf<-data.frame(coalicion=resultados$coalicion)

datosO$CGM<-c(1:lData)*0


datosO$CGM[resultadosDf$coalicion+1]<-1
datosO$CGM<-factor(datosO$CGM)

print(ggplot(data=datosO)+
        theme(aspect.ratio = 4/3)+
        geom_point(aes(x=x,y=y)))

print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=CGM))+
        scale_color_manual(labels = c("No pertenece", "Pertenece"),
                           values = c("red", "blue"))+
        theme(aspect.ratio = 4/3)+
        geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="cyan",fill="cyan"))

