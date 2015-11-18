#################################################
# Arnab Sinha                                   #
# Lecture 4: Analysis of Seattle Weather Data   #
#################################################

#####################################
#    Materials from Lecture 3       #
#####################################

#my.path = "C:\\Users\\arsinha\\Documents\\IIT Kharagpur Course 2015\\Lecture 4 - Analysis of Seattle Weather Data\\"
file.name = "weather_data_2006_2009.csv"
weather_data_2006_2009 = read.csv(paste(file.name, sep=''))

names(weather_data_2006_2009)
x = weather_data_2006_2009
y = x[x$Month == 2, ]
z = y[y$Temp..F. >= 10, ]
z.hist = hist(z$Temp..F., freq=FALSE)
# Increasing the breaks - more accurate
#z.hist = hist(z$Temp..F., breaks=30, freq=FALSE)
length(z.hist$breaks) # Count the number of breaks

# Compute the fraction of data with temperatures below 40 F
z.hist$breaks[7+1]
delta = z.hist$breaks[2]-z.hist$breaks[1]
sum(z.hist$density[1:7] * delta) # About 23%

# Compute mean and standard deviation
m = mean(z$Temp..F.)
s = sd(z$Temp..F.)

# Plot histogram of temperature density and check if it follows normal distribution
plot(z.hist, freq=FALSE, xlab="T", ylab="Density")
# xlim and ylim used to denote the boundaries
plot(z.hist, freq=FALSE, xlim = c(25,60), ylim = c(0,0.1), xlab="T", ylab="Density")
par(new=TRUE) #  the next high-level plotting command (actually plot.new) should not clean the frame before drawing as if it were on a new device.
plot(function(x) dnorm(x, mean=m, sd=s), xlim = c(25,60), ylim = c(0,0.1))

zTemp = z$Temp..F.
z.2007 = z[z$Year == 2007,]
z.2008 = z[z$Year == 2008,]

# Quantile-quantile plot - 
# The q-q plot is used to answer the following questions:
#  Do two data sets come from populations with a common distribution?
#  Do two data sets have common location and scale?
#  Do two data sets have similar distributional shapes?
#  Do two data sets have similar tail behavior?

qqplot(z.2007$Temp..F., z.2008$Temp..F.)
abline(0,1,col="red")

qqplot(z.2007$Temp..F., z.2007$RHum....)
abline(0,1,col="red")

######################################################
# Conditional Probability, pnorm and qnorm functions #
######################################################

# Find the following probability: P ((T < 40) | (T < 45)) in the given normal distribution (m, s).
# pnorm gives the cumulative probability P(X <= q) in a normal distribution 
lt40=pnorm(q=40, mean=m, sd=s) # probability of T < 40 in a normal distribution with mean m and sd s
lt45=pnorm(q=45, mean=m, sd=s) # probability of T < 45 in a normal distribution with mean m and sd s
lt40/lt45 # prob of T < 40 given that T < 45

# Computes quantile function - the quantile function specifies, for a given probability in the 
# probability distribution of a random variable, the value at which the probability of the random  
# variable will be less than or equal to that probability 
qnorm(p=0.5, mean=m, sd=s) 

# Exercise: What is the relation between pnorm and qnorm?
# Exercise: What is the value of the following compositions: pnorm(qnorm(x)) and qnorm(pnorm(x))?

############################################
#       Dealing with Outliers              # 
############################################

#weather.data.select = read.csv(paste(my.path, file.name, sep=''))
weather.data.select = read.csv(paste(file.name, sep=''))
head(weather.data.select)

weather.data.select = weather.data.select[weather.data.select$Year == 2008,] 
weather.data.select = weather.data.select[weather.data.select$Month == 8,] 

summary(weather.data.select)

# package for outliers
if(!require(outliers)) {
  install.packages("outliers", dep=TRUE, repos=reposURL)
} else {
  "Package outliers is already Installed."
} #load / install+load outliers
library(outliers)

# Find the observation furthest from the sample mean
outlier(weather.data.select$Temp..F.)
# Find the observation that is tail-end in the opposite direction
outlier(weather.data.select$Temp..F., opposite = TRUE)

# Grubbs Test: Tests whether the tail value is an outlier
# Default: Test the hypothesis whether the observation furthest from mean is not an outlier.
grubbs.test(weather.data.select$Temp..F.)
# Check if the opposite tail observation is an outlier
grubbs.test(weather.data.select$Temp..F., opposite = TRUE)
# We can also check for two outliers on opposite tails - might not be that useful
grubbs.test(weather.data.select$Temp..F., opposite = TRUE, type=11)


# package extremevalues
if(!require(extremevalues)) {
  install.packages("extremevalues", dep=TRUE)
} else {
  "Package extremevalues is already Installed."
} #load / install+load extremevalues
library(extremevalues)

plot(hist(weather.data.select$Temp..F., freq=FALSE, breaks=15), freq=FALSE, xlim = c(45,95), ylim = c(0,0.1), xlab="T", ylab="Density")
par(new=TRUE) #  the next high-level plotting command (actually plot.new) should not clean the frame before drawing as if it were on a new device.
plot(function(x) dnorm(x, mean=mean(weather.data.select$Temp..F.), sd=sd(weather.data.select$Temp..F.)), xlim = c(45,95), ylim = c(0,0.1))

# Even more sophisticated way for finding the outliers
# This method attempts to estimate the distribution looking at a subset of observations.
# Also generates the qqplot and outlier (along with its index). 
ol <- getOutliers(weather.data.select$Temp..F.)
ol
summary(ol)
outlierPlot(weather.data.select$Temp..F., ol)

#######################################
#      Dealing with Missing Data      #
#######################################

# Artifically add some (1%) missing values to the humidity variable - as a function of temperature
# Hence the missing observations are not completely random, but are correlated to the temperature. 
weather.data.na = weather.data.select
n = nrow(weather.data.na)
# Select 1% of data where temperature is greater than 70F and replace its humidity with NA
weather.data.na$RHum....[sample(which(weather.data.na$Temp..F. > 70), n/100)] = NA

# find percent missing
sum(is.na(weather.data.na$RHum....))/length(weather.data.na$RHum....)

# Exercise: Explain the following histogram.
hist(which(is.na(weather.data.na$RHum....)))

temp_with_hum_observed = weather.data.na$Temp..F.[!is.na(weather.data.na$RHum....)]
temp_with_hum_missing = weather.data.na$Temp..F.[is.na(weather.data.na$RHum....)]

# Exercise: Can you explain why the following qqplot is mostly below y=x line?
qqplot(temp_with_hum_missing, temp_with_hum_observed)
abline(0,1, col="red")
# Shouldn't they be from the same distribution? 
qqplot(temp_with_hum_missing, temp_with_hum_observed[temp_with_hum_observed > 70])
abline(0,1, col="red")

qqplot(temp_with_hum_missing, temp_with_hum_observed, ylim=c(50,90))
abline(0,1, col="red")

# Look for runs of missing data to check if the missing data is 
# Missing Completely At Random (MCAR)
ind <- sapply(weather.data.na$RHum...., function(x) if(is.na(x)){1}else{0})
runs <- rle(ind) # run length encoding
runs
run.df <- data.frame(runs$lengths, runs$values)
run.df = run.df[with(run.df, order(-runs.values, -runs.lengths)),]
run.df[1:10,]

# is it normal?
hist(weather.data.select$RHum....)

# eyeball norm says: could be worse, let's press on with Amelia for imputation
# package Amelia
if(!require(Amelia)) {
  install.packages("Amelia", dep=TRUE)
} else {
  "Package Amelia is already Installed."
} #load / install+load extremevalues
library(Amelia)

# drop the empty column for pressure
weather.data.na = weather.data.na[,-which(names(weather.data.na)=="Pres.mbar.")]
amelia.results = amelia(weather.data.na, idvars=c("Year", "Month", "Day", "Time"))

# Inspecting individual imputations
amelia.results$imputations[[1]]
amelia.results$imputations[[3]]
amelia.results$imputations$imp3

# Show the distribution of the observed and imputed values
plot(amelia.results)
# Recall the relationship between missing values and temperature
plot(weather.data.na$Temp..F. ~ weather.data.na$RHum....)

# Compare means before and after imputation 
mean(weather.data.na$RHum....)
mean(weather.data.na$RHum...., na.rm=TRUE)
for(i in 1:5) {
  print(mean(amelia.results$imputations[[i]]$RHum....))
}

# Compare distributions before and after imputation
hist(weather.data.na$RHum...., xlim=c(0,100))
hist(amelia.results$imputations[[4]]$RHum...., xlim=c(0,100))
qqplot(weather.data.select$RHum...., amelia.results$imputations[[1]]$RHum....)
abline(0,1,col="red")
