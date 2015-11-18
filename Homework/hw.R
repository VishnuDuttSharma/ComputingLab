
################## Assignment - 1 ###################
##################################################

#### Internet
#url <- "http://archive.ics.uci.edu/ml/machine-learning-databases/00225/Indian%20Liver%20Patient%20Dataset%20(ILPD).csv"
#ILPD <- read.csv(url, header=FALSE, stringsAsFactors=FALSE)

#### Local (To be read if Local file is available)
fileName <- "Indian Liver Patient Dataset (ILPD).csv"
ILPD <- read.csv(fileName, header=FALSE, stringsAsFactors=FALSE)

#head(ILPD)

# 1. Age: Age of the patient
# 2. Gender: Gender of the patient
# 3. TB: Total Bilirubin
# 4. DB: Direct Bilirubin
# 5. Alkphos: Alkaline Phosphotase
# 6. Sgpt: Alamine Aminotransferase
# 7. Sgot: Aspartate Aminotransferase
# 8. TP: Total Protiens
# 9. ALB: Albumin
# 10. A/G: Ratio Albumin and Globulin Ratio
# 11. Selector: field used to split the data into two sets (labeled by the experts) 

# Headers 
headers <- c("Age", "Gender", "TB", "DB", "Alkphos", "Sgpt", "Sgot", "TP", "ALB", "A-G(ratio)", "Selector")
names(ILPD) <- headers

# Convert Gender into numeric, Male = 1, Female = 0
ILPD[,2] = unlist(lapply(ILPD[,2], function(x) ifelse((x == "Male"), 1, 0)))

#ILPD.df <- as.data.frame(ILPD)
##################################################
################################################

################### Assignment - 2 #################
##################################################
## Find Mean, Median, SD
means <- colMeans(ILPD, na.rm = TRUE)
medians <- apply(ILPD, 2, median, na.rm = TRUE)
sds <- apply(ILPD, 2, sd, na.rm = TRUE)
# Note: In gender, data is obtained considering Male = 1 & Female = 0

## Histograms
for (i in 1:ncol(ILPD)){
  png( paste( headers[i], "_hist.png"))
  hist(ILPD[,i], axes = TRUE, main = paste("Histogram for ",headers[i]), ylab = "Frequency", xlab = headers[i], col = "Green")
  dev.off()
}



plot(ILPD, main = paste("Plot for ILPD"), cex = 0.5)
## Plot
#png( "ILPD_plot.png")
#plot(ILPD,  axes = TRUE, main = paste("Histogram for ILPD"), plot = TRUE, ylab = "ILPD", xlab = "Data Points")
#dev.off()

for (i in 1:ncol(ILPD)){
  png( paste( headers[i], "_plot.png"))
  plot(ILPD[,i], axes = TRUE, main = paste("Plot for ",headers[i]), plot = TRUE, ylab = "Value", xlab = "Datapoints")
  dev.off()
}

##################################################
##################################################

################## Assignment - 3 ##################
##################################################

## Removing Outliers
data <- c(-1, 1, 5, 1, 1, 17, -3, 1, 1, 3)
#data <- ILPD[,1]
x <- data
x <- x[(x < mean(x, na.rm=T) + 2*sd(x, na.rm=T)) & (x >  mean(x, na.rm=T) - 2*sd(x, na.rm=T))]
x
##################################################

## Relabelling ###############
x <- c('BS', 'MS', 'PhD', 'HS', 'Bachelors', 'Masters', 'High School', 'BS', 'MS', 'MS')

bachelor <- c('BS')
masters <- c('MS')
highSchool <- c('HS')
research <- c('PhD')

x[x %in% bachelor] <- 'Bachelors'
x[x %in% masters] <- 'Masters'
x[x %in% highSchool] <- 'High School'
x[x %in% research] <- 'Doctorate'

x
##################################################

## Min-Max Normalization ###############
x <- c(-1, 1, 5, 1, 1, 17, -3, 1, 1, 3)
normX <- (x - min(x)) / (max(x) - min(x))
normX
##################################################

## Z-score Normalization ###############
z_normX <- (x - mean(x)) / sd(x)
z_normX
##################################################

## Binarize ###############
x <- c('Red', 'Green', 'Blue', 'Blue', 'Blue', 'Blue', 'Blue', 'Red', 'Green', 'Blue')

isRed <- x == 'Red'
isGreen <- x == 'Green'
isBlue <- x == 'Blue'

isRed <- as.numeric(isRed)
isRed
isGreen <- as.numeric(isGreen)
isGreen
isBlue <- as.numeric(isBlue)
isBlue
##################################################

## Equal Range Bin Discretization ###############
x <- c(3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 12, 23, 23, 25, 81)

range <- max(x) - min(x)
binWidth <- range / 3

bin1Min <- -Inf
bin1Max <- min(x) + binWidth
bin2Max <- min(x) + 2*binWidth
bin3Max <- Inf

discretized <- rep(NA, length(x))

discretized[bin1Min < x & x <= bin1Max] <- "Low"
discretized[bin1Max < x & x <= bin2Max] <- "Medium"
discretized[bin2Max < x & x <= bin3Max] <- "High"

discretized
##################################################

## Equal Amount Bin Discretization ###############
bin1Min <- -Inf
bin1Max <- 5
bin2Max <- 7
bin3Max <- Inf

#discretized <- rep(NA, length(x))

discretized[bin1Min < x & x <= bin1Max] <- "Low"
discretized[bin1Max < x & x <= bin2Max] <- "Medium"
discretized[bin2Max < x & x <= bin3Max] <- "High"

discretized

##################################################
####################################################

