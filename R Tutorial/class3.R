#################################################
# Arnab Sinha                                   #
# Lecture 3: Analysis of Real world datasets    #
#################################################

# Recap and other helpful R functions

help(table) # documentation page for table
?help       # documentation page for help
??help      # for finding help pages on a vague topic

# Different datatypes e.g. "character", "double"
a = "c"
typeof(a)       # determines the type or storage mode of any object
is(a)           # returns all the super-classes of this object's class
is(a, "vector") # tests whether object can be treated as from "vector"
is(a, "double")
b = 7
typeof(b)
is(b)

# Coercion (implicit type conversion) of various datatypes within an array
d = c(a,b)
typeof(d)
d[2]
as.numeric(d[2]) # Convert into numeric type

# functions: factor, ordered
sizes = c("s", "m", "m", "l", "s")

d = factor(sizes) # function 'factor' is used to encode a vector as a factor or categories
d[2]
as.numeric(d[2])
d[2] > d[1] # not meaningful since there is no order in default factor

d = ordered(sizes) # lexically ordered
d[2] > d[1] # false since "l" < "m" < "s"
d = factor(sizes, levels=c("s", "m", "l"), ordered=TRUE) # levels are assumed to be ordered since the flag ordered is set to true
d[2] > d[1] # true since "s" < "m" < "l"

colors = c("red", "blue", "green", "blue", "red")
# data.frame is tightly coupled collection of variables 
# which share many properties of matrices and lists
shirts = data.frame(sizes, colors)  
shirts
shirts[2,]
shirts$colors
shirts[shirts$colors == "blue",]
shirts$sizes = factor(shirts$sizes, levels=c("s", "m", "l"), ordered=TRUE)
shirts$id = c(1,2,3,4,5)
shirts = shirts[with(shirts, order(sizes)),] # Go to the Workspace tab to examine the result
shirts[with(shirts, order(colors)),] # sorted by color

# table with respect to two variables (size and color)
table(shirts$sizes, shirts$colors) # counts the frequencies
# w.r.t. all three variables
table(shirts)
ftable(shirts) # flat contingency table

size.col = table(shirts$sizes, shirts$colors)
margin.table(size.col)
margin.table(size.col,1) # 1 for rows
margin.table(size.col,2) # 2 for cols

# express table entries as fraction of marginal table
prop.table(size.col) 
prop.table(size.col,1)
prop.table(size.col,2)

# Declaration of function (introducing return)
my.addition = function(a,b) {
  return (a+b)
}

# Calling a function
my.addition(1,2)

# Declaring a function in the lambda function format
my.multiplication = function(a,b) a*b
my.multiplication(4,5)

#################################################
#  Analysis of Abalone data                     #
#################################################

# Actual data is at http://archive.ics.uci.edu/ml/machine-learning-databases/abalone/abalone.data
x = read.csv("http://www.cnergres.iitkgp.ac.in/~arnab/abalone.data", header=FALSE)
names(x)=c("Sex","Length","Diameter","Height","Whole.weight","Shucked.weight", "Viscera.weight", "Shell.weight", "Rings")
# View the top 6 (default) rows
head(x)
# Get a sense of the data-distribution (e.g. symmetry, skewness, outliers)
# max is 1.5 times the interquartile range (Q3-Q1) above Q3
# min is 1.5 times the interquartile range (Q3-Q1) below Q1
boxplot(x$Height)
# Summary shows the quartiles, mean, min-max information
summary(x$Height)
# Order them according to ascending Sex, Shell.weight and Rings
y = x[with(x, order(Sex, Shell.weight, Rings)),]
head(y, 10)
hist(x$Height)

# Select number of buckets for quantiles
qn = 3

# Map Height to quantiles
q = quantile(x$Height, probs=seq(0,1,1/qn), na.rm=T)
H = diff(range(x$Height, na.rm=T))
q[[1]] = q[[1]] - 0.1*H
q[[length(q)]] = q[[length(q)]] + 0.1*H
x$Height3 = as.numeric(cut(x$Height, breaks=q))

# Map WholeWeight to quantiles
q = quantile(x$Whole.weight, probs=seq(0,1,1/qn), na.rm=T)
H = diff(range(x$Whole.weight, na.rm=T))
q[[1]] = q[[1]] - 0.1*H
q[[length(q)]] = q[[length(q)]] + 0.1*H
x$WholeWeight3 = as.numeric(cut(x$Whole.weight, breaks=q))

# Exercise: compute the joint distribution of the two discretized variables
j = table(x$Height3, x$WholeWeight3)/nrow(x)


# Exercise: does the joint distribution indicate that the variables are independent?

# compute the individual distributions
h = table(x$Height3)/nrow(x)
w = table(x$WholeWeight3)/nrow(x)
h
w
# compute the outer product ()
i = outer(h, w)

# error
err = (i-j)/i
err
sum(abs(err))/(qn**2) # average relative error per entry

#################################################
#     Analysis of Seattle Weather Data          #
#################################################

#weather_data_2006_2009 = read.csv("http://www.cnergres.iitkgp.ac.in/~arnab/weather_data_2006_2009.csv", header=TRUE)
# Download the data in your local machine - ***** remember to change the following path to you local machine *****
weather_data_2006_2009 = read.csv("weather_data_2006_2009.csv", header=TRUE)

names(weather_data_2006_2009)
x = weather_data_2006_2009

# Excercise: Select records for the month of October only

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

# Quantile-quantile plot - 
# The q-q plot is used to answer the following questions:
#  Do two data sets come from populations with a common distribution?
#  Do two data sets have common location and scale?
#  Do two data sets have similar distributional shapes?
#  Do two data sets have similar tail behavior?

# rnorm generates random numbers from normal distribution
qqplot(rnorm(1000, 0, 1), rnorm(1000, 0, 3))
abline(0,1,col="red")

x1 = rnorm(1000, 0, 1)
x2 = rnorm(1000, 2, 3)
qqplot(x1, (x2-2)/3)
abline(0,1,col="red")

# Exercise: Visually inspect if the February temperatures of 2006 and 2007 are from the same distribution

y = x[x$Month == 2, ]
z.2006 = y[y$Year == 2006, ]
z.2007 = y[y$Year == 2007, ]


qqplot(z.2006$Temp..F., z.2007$Temp..F.)
abline(0,1,col="red")

qqplot(z.2006$Temp..F., z.2007$RHum....)
abline(0,1,col="red")
