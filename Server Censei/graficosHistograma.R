library(ggplot2)
library(vioplot)
library(corrplot)
library(jsonlite)
data<-fromJSON("hist.json")
datos<-as.data.frame(data)
library(viridis)
library(viridisLite)
ggplot(datos, aes(x = iteraciones, y = fitness)) +
  geom_boxplot(aes(fill = fitness) ,alpha = 0.5) +
  geom_density(aes(x = iteraciones), inherit.aes = FALSE,fill="#feb24c",alpha=0.6,col="#800026") +
  geom_jitter(aes(x=iteraciones,col=iteraciones),height=0.36,alpha=0.1,size=0.2)+
  ylab("")+
  scale_colour_gradient(low="#fed976",high="#800026")+theme(
    panel.background = element_rect(fill = "gray",
                                    colour = "black",
                                    size = 0.5, linetype = "solid")
  )
histo <- fromJSON("hist.json")
histo<-as.data.frame(histo)
ggplot(histo, aes(x = iteraciones, y = fitness))+
        geom_line(aes(iteraciones,fitness))+ylab("")+
    scale_colour_gradient(low="#fed976",high="#800026")+theme(
      panel.background = element_rect(fill = "gray",
                                      colour = "black",
                                      size = 0.5, linetype = "solid")
    )+  geom_path()+
  geom_point()
      

  