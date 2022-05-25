library(jsonlite)
library(ggplot2)

histo <- fromJSON("hist.json")
histo<-as.data.frame(histo)

print(ggplot(data=histo)+
  geom_line(aes(iteraciones,fitness)))

