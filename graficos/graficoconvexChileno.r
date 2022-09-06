
#--------------------------------------------------------------
library(jsonlite)
library(ggplot2)
library(ggConvexHull)

datosJson<-fromJSON("ejemplo2.json")
datosJson<-datosJson$diputados
lData<-length(datosJson$ID)
datosO<-data.frame(x=datosJson$coordX,y=datosJson$coordY,ID=datosJson$ID)


resultados<- fromJSON("resultados.json")
resultadosDf<-data.frame(coalicion=resultados$coalicion)

datosO$coaliciong<-c(1:lData)*0
solucion <- resultadosDf$coalicion

for(id in solucion){
  for(i in 1:lData){
    if(datosO$ID[i]==id){
      datosO$coaliciong[i]<-1
      print(datosO$ID[i])
    }
  }
}
datosO$coaliciong<-factor(datosO$coaliciong)

print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=coaliciong))+
        geom_convexhull(data=datosO[which(datosO$coaliciong==1),1:2],aes(x=x,y=y),alpha = 0.2,col="cyan",fill="cyan"))
