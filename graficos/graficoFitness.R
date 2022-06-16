library(ggplot2)

data <- read.csv(file = 'prueba.csv')
datos=as.data.frame(data)
names(datos) = c("m","p_tresh","pr","seed","iteraciones","fitness")
#print(datos)

#usar graficos de violin
#fitness
print(
  ggplot(data=datos)+geom_point(aes(x=1:nrow(datos),y=fitness))
)

print(
  ggplot(data=datos,aes(fitness))+geom_histogram()
)

print(
  ggplot(data=datos)+geom_point(aes(x=m,y=fitness))
)


print(
  ggplot(data=datos)+geom_point(aes(x=p_tresh,y=fitness))
)

print(
  ggplot(data=datos)+geom_point(aes(x=pr,y=fitness))
)


##iteraciones


print(
  ggplot(data=datos)+geom_point(aes(x=1:nrow(datos),y=iteraciones))
)

print(
  ggplot(data=datos,aes(iteraciones))+geom_histogram()
)

print(
  ggplot(data=datos)+geom_point(aes(x=m,y=iteraciones))
)


print(
  ggplot(data=datos)+geom_point(aes(x=p_tresh,y=iteraciones))
)

print(
  ggplot(data=datos)+geom_point(aes(x=pr,y=iteraciones))
)