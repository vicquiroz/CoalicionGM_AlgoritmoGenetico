
#--------------------------------------------------------------
library(jsonlite)
library(ggplot2)
library(ggConvexHull)

datosJson<-fromJSON("ingles.json")
datosJson<-as.data.frame(datosJson$rollcalls$votes)
lData<-length(datosJson$x)
datosO<-data.frame(x=datosJson$x,y=datosJson$y,ID=c(1:lData))


resultados<- fromJSON("4Coaliciones/resultados_4_fit_9520.30.json")
resultadosDf<-data.frame(coalicion=resultados$coalicion)

datosO$CGM<-c(1:lData)*0
datosO$party<-datosJson$party

datosO$CGM[resultadosDf$coalicion+1]<-1
datosO$CGM<-factor(datosO$CGM)

#print(ggplot(data=datosO)+
        #theme(aspect.ratio = 4/3)+
        #geom_point(aes(x=x,y=y,color=factor(party))))+
        #scale_color_manual(
          #labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),
          #values = c("blue", "green","cyan","red")) +
        #labs(color="Partido")
print(ggplot(data=datosO)+
          geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
          scale_color_manual(
            labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),#c("No pertenece", "Pertenece")
            values = c("blue", "green","cyan","red")) +
          scale_shape_manual(
            labels=c("No pertenece", "Pertenece"),
            values = c(4,16)) +
          theme(aspect.ratio = 4/3,
          panel.background = element_rect(fill = "gray",
                                          colour = "black",
                                          size = 0.5, linetype = "solid")
          )+
      
          labs(color="Partido")+
          geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="cyan",fill="cyan"))
  

