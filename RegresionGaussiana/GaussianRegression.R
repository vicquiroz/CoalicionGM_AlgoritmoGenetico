print("Ejecucion")
data.object<-read.table("DatosEjecucion8.txt",sep=",",header=T)
data<-data.object
#y<-data.object$fitnes
#data.object<-data.object[,-c(1,2,4,8:10)]

library(np)
#bw <- npudensbw(dat = data.object)
bw <- npregbw(fitnes~m+p_thresh+p_r,data=data.object)
model.np <- npreg(bws = bw) #Cálculo del ancho de banda por variable
model.sig<-npsigtest(model.np,boot.num = 100) #Significancia de cada variable

# Creación del mallado para predicción
m<-seq(min(data$m),max(data$m),2)
p_thresh<-seq(min(data$p_thresh),max(data$p_thresh),.01)
p_r<-seq(min(data$p_r),max(data$p_r),.01)
datos_predict<-expand.grid(m=m,p_thresh=p_thresh,p_r=p_r)

# Predicción
pred.np<-predict(model.np, newdata = datos_predict)
best_parameters<-datos_predict[which(pred.np==min(pred.np)),]
print(model.sig)
print(summary(model.np))
print(best_parameters)
