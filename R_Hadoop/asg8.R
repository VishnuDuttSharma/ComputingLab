library(NLP)
fileName <- "Mydata.csv"



mydata = read.csv( fileName , header = FALSE)

mydata[2] = sapply(mydata[2][[1]], function(x) length(unlist(strsplit(as.String(x),";"))))


names(mydata)[1] <- paste("name")
names(mydata)[2] <- paste("tag")
names(mydata)[3] <- paste("titles")
names(mydata)[4] <- paste("songId")


df <- data.frame(mydata )



############### PART- 1 ####################
NUM_TAGS = 25
resultTitle <- df$titles[df$tag > NUM_TAGS]
resultName <- df$songId[df$tag > NUM_TAGS]
############################################

############## PART - 2 ####################
refined <- sapply(1:length(df$name), function(x) {
                                kl <- vector()
                                kl <- unlist(strsplit(as.String(df$name[x]), ";"))
                                ret <- vector()
                                for(i in 1:length(kl)){
                                  #ret <- c(ret, c(kl[i], df$tag[x], df$songId[x]))
                                  ret <- c( ret, c(kl[i], i))
                                }
                                  return (ret)

                                    } )

df1 <- as.data.frame(refined)
names(df1) <- c("name", "tag", "songId")
#############################################
