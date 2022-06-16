
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

datosO$coaliciong<-c(1:lData)*0


datosO$coaliciong[resultadosDf$coalicion+1]<-1
datosO$coaliciong<-factor(datosO$coaliciong)

print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=coaliciong))+
        geom_convexhull(data=datosO[which(datosO$coaliciong==1),1:2],aes(x=x,y=y),alpha = 0.2,col="cyan",fill="cyan"))

sum(datosO$coaliciong=="0")
