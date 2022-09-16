
#--------------------------------------------------------------
library(jsonlite)
library(ggplot2)
library(ggConvexHull)
library(distances)
library(ggforce)
library(dismo)
datosJson<-fromJSON("votacionRH0941234.json")
datosJson<-as.data.frame(datosJson$rollcalls$votes)
lData<-length(datosJson$x)
datosO<-data.frame(x=datosJson$x,y=datosJson$y,ID=c(1:lData))
datosO$party<-datosJson$party

print(ggplot(data=datosO)+
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        geom_point(aes(x=x,y=y,color=factor(party))))+
        scale_color_manual(
        labels = c("Demócrata","Republicano"),
        values = c("blue","red")) +
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-1,1)+
        ylim(-1,1)+
        labs(color="Partido")

# COALICION GENETICA
resultados<- fromJSON("resultados.json")
resultadosDf<-data.frame(coalicion=resultados$coalicion)
datosO$CGM<-c(1:lData)*0
datosO$party<-datosJson$party
datosO$CGM[resultadosDf$coalicion+1]<-1
datosO$CGM<-factor(datosO$CGM)
mat_dist<-dist(datosO[,1:2],method="euclidean",diag=F,upper=F)
mat_dist_sol <-
dist(datosO[which(datosO$CGM==1),1:2],
method = "euclidean",
diag = F,
upper = F)
sum(mat_dist_sol)
print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
        scale_color_manual(
          labels = c("Demócrata","Republicano"),
          values = c("blue","red")) +
        scale_shape_manual(
          labels=c("No pertenece", "Pertenece"),
          values = c(4,16)) +
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-1,1)+
        ylim(-1,1)+
        labs(color="Partido")+
        geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="purple",fill="#cf4ca5"))

# COALICION NUEVO ALGORITMO
resultadosDf<-data.frame(coalicion=c(
  1,6,10,17,18,19,20,22,23,26,27,29,30,31,33,37,39,40,41,44,45,46,47,48,50,51,52,54,58,61,62,65,66,67,74,78,82,84,85,86,90,91,97,98,101,102,103,105,107,108,109,111,115,117,122,123,124,125,126,127,128,129,130,132,133,134,136,137,139,143,145,146,147,148,149,151,152,153,155,165,166,168,173,174,176,178,179,181,187,188,189,190,194,195,196,197,198,199,200,205,206,207,208,209,216,217,218,219,222,224,225,229,230,232,233,234,235,239,240,241,242,243,244,245,246,249,250,251,254,255,257,258,259,260,261,263,265,267,270,271,275,276,277,279,280,281,282,284,285,287,291,292,293,294,299,305,308,313,317,318,319,320,321,322,327,328,329,331,332,333,335,340,341,343,344,349,350,351,352,353,355,356,359,360,361,367,369,372,374,380,382,390,393,397,398,407,408,409,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,427,428,430
  ))
datosO$CGM<-c(1:lData)*0
datosO$party<-datosJson$party
datosO$CGM[resultadosDf$coalicion+1]<-1
datosO$CGM<-factor(datosO$CGM)
print(ggplot(data=datosO)+
        geom_point(aes(x=x,y=y,col=factor(party),shape=CGM))+ #Antiguo CGM
        scale_color_manual(
          labels = c("Demócrata","Republicano"),
          values = c("blue","red")) +
        scale_shape_manual(
          labels=c("No pertenece", "Pertenece"),
          values = c(4,16)) +
        theme(aspect.ratio = 4/4,
              panel.background = element_rect(fill = "gray",
                                              colour = "black",
                                              size = 0.5, linetype = "solid")
        )+
        xlab("Dimensión 1")+
        ylab("Dimensión 2")+
        xlim(-1,1)+
        ylim(-1,1)+
        labs(color="Partido")+
        geom_convexhull(data=datosO[which(datosO$CGM==1),1:2],aes(x=x,y=y),alpha = 0.2,col="purple",fill="#cf4ca5"))
