
#--------------------------------------------------------------
library(jsonlite)
library(ggplot2)
library(ggConvexHull)
library(distances)
library(ggforce)
library(dismo)
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

print(ggplot(data=datosO)+
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        geom_point(aes(x=x,y=y,color=factor(party))))+
        scale_color_manual(
        labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),
        values = c("blue", "green","cyan","red")) +
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        labs(color="Partido")
CGM<-datosO[which(datosO$CGM==1),]
centroide<-c(-0.240322,0.0978458)
convex<-chull(CGM)
convexPoints<-CGM[convex,1:2]
convexPoints<-rbind(convexPoints,centroide)
distancia2<-as.matrix(dist(convexPoints,diag = T,upper = T))
distancia2<-distancia2[,nrow(distancia2)]
maximo<-which.max(distancia2)
#maximo<-distancia2[maximo]
maximo<-names(maximo)
maximo<-as.numeric(maximo)
puntoMaximo<-datosO[maximo,]
radio<-sqrt((centroide[1]-puntoMaximo$x)^2+(centroide[2]-puntoMaximo$y)^2)
print(ggplot(data=datosO)+
          geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
          scale_color_manual(
            labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),#c("No pertenece", "Pertenece")
            values = c("blue", "green","cyan","red")) +
          scale_shape_manual(
            labels=c("No pertenece", "Pertenece"),
            values = c(4,16)) +
          theme(aspect.ratio = 4/4,
          panel.background = element_rect(fill = "gray",
                                          colour = "black",
                                          size = 0.5, linetype = "solid")
          )+
          geom_point(aes(x=-0.240322, y=0.0978458), colour="yellow")+
          xlab("Dimensión 1")+
          ylab("Dimensión 2")+
          xlim(-0.9,0.9)+
          ylim(-0.9,0.9)+
          labs(color="Partido")+
          geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="purple",fill="#cf4ca5")+
          geom_circle(aes(x0=-0.240322, y0=0.0978458,r=radio), inherit.aes=FALSE)+
        geom_point(data=convexPoints,aes(x=x,y=y),colour="orange")+
        geom_point(data=puntoMaximo,aes(x=x,y=y),colour="black",size=3)
      )
#-----------------------------------------------------------------
CGM<-datosO[which(datosO$CGM==1),]
centroide2<-c(mean(CGM$x),mean(CGM$y))
convex<-chull(CGM)
convexPoints<-CGM[convex,1:2]
convexPoints<-rbind(convexPoints,centroide2)
distancia2<-as.matrix(dist(convexPoints,diag = T,upper = T))
distancia2<-distancia2[,nrow(distancia2)]
maximo<-which.max(distancia2)
#maximo<-distancia2[maximo]
maximo<-names(maximo)
maximo<-as.numeric(maximo)
puntoMaximo<-datosO[maximo,]
radio<-sqrt((centroide2[1]-puntoMaximo$x)^2+(centroide2[2]-puntoMaximo$y)^2)
print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
        scale_color_manual(
          labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),#c("No pertenece", "Pertenece")
          values = c("blue", "green","cyan","red")) +
        scale_shape_manual(
          labels=c("No pertenece", "Pertenece"),
          values = c(4,16)) +
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        geom_point(aes(x=centroide2[1], y=centroide2[2]), colour="yellow")+
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-0.9,0.9)+
        ylim(-0.9,0.9)+
        labs(color="Partido")+
        geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="purple",fill="#cf4ca5")+
        geom_circle(aes(x0=centroide2[1], y0=centroide2[2],r=radio), inherit.aes=FALSE)+
        geom_point(data=convexPoints,aes(x=x,y=y),colour="orange")+
        geom_point(data=puntoMaximo,aes(x=x,y=y),colour="black",size=3)
)

# *- - - - - - - -- 
resultados<- fromJSON("4Coaliciones/resultadosTEST.json")
resultadosDf<-data.frame(coalicion=resultados$coalicion)

datosO$CGM<-c(1:lData)*0
datosO$party<-datosJson$party

datosO$CGM[resultadosDf$coalicion+1]<-1
datosO$CGM<-factor(datosO$CGM)
print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
        scale_color_manual(
          labels = c("Demócrata","Agrario-Laborista","Progresivo","Republicano"),#c("No pertenece", "Pertenece")
          values = c("blue", "green","cyan","red")) +
        scale_shape_manual(
          labels=c("No pertenece", "Pertenece"),
          values = c(4,16)) +
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        geom_point(aes(x=-0.240322, y=0.0978458), colour="yellow")+
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-1,1)+
        ylim(-1,1)+
        labs(color="Partido")+
        geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="purple",fill="#cf4ca5"))

#- - - - - - -  - -- -  - - - - - - - - - 
# INSERTAR EN PTCC LOS PUNTOS PARA PROBAR CONVEXHULL
pts<-datosO
ptsC<-data.frame(x=c(),
                 y=c()
                 )
print(ggplot(data=pts)+
        geom_point(aes(x=x,y=y),colour="blue")+ #Antiguo CGM
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-1,1)+
        ylim(-1,1)+
        geom_point(data = ptsC,aes(x=x,y=y), colour="yellow")
      )
