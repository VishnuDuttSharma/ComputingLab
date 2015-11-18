# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")


removeOutlier <- function(x, mode = 'sd'){
  if (toString(mode) == 'sd'){
    range = 0.5*sd(x, na.rm=TRUE)
  }else{ 
    if (toString(mode) == 'quartile'){
      range = 1.5*IQR(x, na.rm = TRUE)
    }else{
      range = 3*mean(x, na.rm=TRUE) 
    }
  }
  
  x <- x[(x < mean(x, na.rm=TRUE) + range) & (x >  mean(x, na.rm=TRUE) - range)]
  
  return (x)
}

#################################################
#  Analysis of Abalone data                     #
#################################################

x = read.csv("abalone.csv.txt", header=FALSE)
names(x)=c("Sex","Length","Diameter","Height","Whole.weight","Shucked.weight", "Viscera.weight", "Shell.weight", "Rings")



# Get a sense of the data-distribution (e.g. symmetry, skewness, outliers)
# max is 1.5 times the interquartile range (Q3-Q1) above Q3
# min is 1.5 times the interquartile range (Q3-Q1) below Q1
#boxplot(x$Height)

#summary(x$Height)


# Select number of buckets for quantiles
qn = 10

# Map Height to quantiles
q = quantile(x$Height, probs=seq(0,1,1/qn), na.rm=T)
H = diff(range(x$Height, na.rm=T))
q[[1]] = q[[1]] - 0.1*H
q[[length(q)]] = q[[length(q)]] + 0.1*H
x$Height10 = as.numeric(cut(x$Height, breaks=q))


# Exercise: compute the joint distribution of the two discretized variables
j = table(x$Height10, x$Rings)/nrow(x)


# Exercise: does the joint distribution indicate that the variables are independent?
#timeVect = x$Time
#x$Time = as.numeric(x$Time)

# compute the individual distributions
h = table(x$Height10)/nrow(x)
w = table(x$Rings)/nrow(x)
#h
#w
# compute the outer product ()
#i = outer(h, w)

#
#j = table(x$cumlAge, x$Rings)/nrow(x)
#x$cumlAge = cumsum(x$Rings)/c(1:nrow(x))
#plot(x$cumlAge, x$Rings)

ua = sort(unique(x$Rings) , decreasing = TRUE)
ageMean = sapply(1:length(ua), function(p) mean(x$Whole.weight[which(x$Rings == p)]))

plot(ageMean, ua)


#################################################
#     Analysis of Seattle Weather Data          #
#################################################
#weather_data_2006_2009 = read.csv("http://www.cnergres.iitkgp.ac.in/~arnab/weather_data_2006_2009.csv", header=TRUE)
# Download the data in your local machine - ***** remember to change the following path to you local machine *****
weather_data = read.csv("weather_data_2000_2014.csv", header=TRUE)

names(weather_data)
x = weather_data

# Excercise: Select records for the month of October only
summary(x)

#boxplot(x)

#boxplot(x$Year)
#boxplot(x$Month)
#boxplot(x$Time)

## RHum ##
summary(x$RHum....)
# Min value is very low, noisy
# No negative data possible
data <- x$RHum....[which((x$RHum.... >= 0))]

length(x$RHum[is.na(x$RHum....)])
# No NA

boxplot(data)  
# Contains outlier

boxplot(removeOutlier(data, mode = 'sd'))
#boxplot(removeOutlier(x$RHum...., mode = 'sd'))


## Temp ##
summary(x$Temp..F.)
# Min-max value quite high

data<- x$Temp..F.[x$Temp..F. >= 10 ]
data <- data[data <= 200 ]

summary(data)

length(x$Temp..F.[is.na(x$Temp..F.)])
# No NA

boxplot(data)
#Contains Outliers

boxplot(removeOutlier(data, mode = 'sd'))



## Wind ##
summary(x$Wind.Direct)
# Max value too high

data <- x$Wind.Direct[x$Wind.Direct <= 1000]
summary(data)

boxplot(data)
## No outliers

length(x$Temp..F.[is.na(x$Temp..F.)])
# No NA


## Speed ##
summary(x$Speed..knot.)

data <- x$Speed..knot.[x$Speed..knot. <= 100]
summary(data)

length(x$Spees..knot.[is.na(x$Speed..knot.)])
# No NA

boxplot(data)
## Outliers are present

boxplot(removeOutlier(data, mode = 'sd'))


## Gust ##
summary(x$Gust..knot.)

data <- x$Gust..knot.[x$Gust..knot. <= 300]
summary(data)

length(x$Gust..knot.[is.na(x$Gust..knot.)])
# No NA

boxplot(data)
# Contains Outliers

boxplot(removeOutlier(data, mode = 'sd'))

## Rain ##
summary(x$Rain..inch.)

data <- x$Rain..inch.[x$Rain..inch. <= 5] #9
summary(data)

length(x$Rain..inch.[is.na(x$Rain..inch.)])
# No NA

boxplot(data)
# Contains Outliers

boxplot(removeOutlier(data, mode = 'sd'))

## Radiation ##
summary(x$Radiation..Watts.m.2.)

data <- x$Radiation..Watts.m.2.[x$Radiation..Watts.m.2. <= 2000] 
summary(data)

# No NA

boxplot(data)
# Contains Outliers

boxplot(removeOutlier(data, mode = 'sd'))

## Pres ##
summary(x$Pres.mbar.)

data <- x$Pres.mbar.[!is.na(x$Pres.mbar.)]
## Contains NA
summary(data)

boxplot(data)
# Contains Outliers

boxplot(removeOutlier(data, mode = 'sd'))


## Monthly Trend
meanRHum = vector()
sdRHum = vector()
for (i in  1:12){
  y = x$RHum....[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanRHum <- c(t1, meanRHum)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdRHum <- c(t2, sdRHum)
}
plot(c(1:12), meanRHum)


meanTemp = vector()
sdTemp = vector()
for (i in  1:12){
  y = x$Temp..F.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanTemp <- c(t1, meanTemp)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdTemp <- c(t2, sdTemp)
}
plot(c(1:12), meanTemp)

meanWind = vector()
sdWind = vector()
for (i in  1:12){
  y = x$Wind.Direct[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanWind <- c(t1, meanWind)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdWind <- c(t2, sdWind)
}
plot(c(1:12), meanWind)

meanSpeed = vector()
sdSpeed = vector()
for (i in  1:12){
  y = x$Speed..knot.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanSpeed <- c(t1, meanSpeed)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdSpeed <- c(t2, sdSpeed)
}
plot(c(1:12), meanSpeed)


meanGust = vector()
sdGust = vector()
for (i in  1:12){
  y = x$Gust..knot.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanGust <- c(t1, meanGust)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdGust <- c(t2, sdGust)
}
plot(c(1:12), meanGust)

meanRain = vector()
sdRain = vector()
for (i in  1:12){
  y = x$Rain..inch.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanRain <- c(t1, meanRain)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdRain <- c(t2, sdRain)
}
plot(c(1:12), meanRain)

meanRadiation = vector()
sdRadiation = vector()
for (i in  1:12){
  y = x$Radiation..Watts.m.2.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanRadiation <- c(t1, meanRadiation)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdRadiation <- c(t2, sdRadiation)
}
plot(c(1:12), meanRadiation)

meanPres = vector()
sdPres = vector()
for (i in  1:12){
  y = x$Pres.mbar.[x$Month == i ]
  
  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
  meanPres <- c(t1, meanPres)
  
  t2 = sd(removeOutlier(y, mode = 'quartile'))
  sdPres <- c(t2, sdPres)
}
plot(c(1:12), meanPres)



#dataList = vector()
#for (i in  1:12){
#  y = x$Rain..inch.[x$Month == i ]
#  
#  t1 = mean(removeOutlier(y, mode = 'quartile'), is.na = T)
#  meanList1 <- c(t1, dataList)
#}
#plot(c(1:12), meanList1)
qqplot(rnorm(1000, meanRHum, sdRHum), rnorm(1000, meanTemp, sdTemp))
abline(0,1,col="red")

qqplot(rnorm(1000, meanTemp, sdTemp), rnorm(1000, meanWind, sdWind))
abline(0,1,col="red")

qqplot(rnorm(1000, meanWind, sdWind), rnorm(1000, meanSpeed, sdSpeed))
abline(0,1,col="red")

qqplot(rnorm(1000, meanSpeed, sdSpeed), rnorm(1000, meanGust, sdGust))
abline(0,1,col="red")

qqplot(rnorm(1000, meanRHum, sdRHum), rnorm(1000, meanRain, sdRain))
abline(0,1,col="red")

qqplot(rnorm(1000, meanPres, sdPres), rnorm(1000, meanTemp, sdTemp))
abline(0,1,col="red")
