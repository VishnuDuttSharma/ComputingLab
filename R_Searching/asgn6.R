#Text Mining Package
require(tm)
#Stemming package
require(SnowballC)
#Package to help in storing data in HDD rather that RAM space
require(ff)
#Package for on ff objects
require(ffbase)

#Try to use vectors rather than lists (Performance issue)


# Split the file(stored as char array) by newline
fileRead <- function(fileName){
  res <- tolower(readChar(fileName,file.info(fileName)$size))
  res <- unlist(strsplit(res, "\r*\n"))
  pat <- "^(review/summary)|^(review/text)"
  r <- grep(pattern = pat, res, value = TRUE)
  r <- lapply(r, function(x) strsplit(x, ':')[[1]][2])
  
  rNew <- lapply(1:(length(r)/2), function(x) paste(r[2*x-1], r[2*x]))
  
  return (rNew)
}

#Read in the data
fileName <- "smallData.txt"

res <- fileRead(fileName)


#Parsing
#res <- gsub("[^[:alnum:]:]+", " ", gsub("^[[:alnum:]]*\\/", "", res))


#PART 1
# selects all rows containing summary and text    
pat <- "^(review/summary)|^(review/text)"
r <- grep(pattern = pat, res, value = TRUE)
r <- lapply(r, function(x) strsplit(x, ':')[[1]][2])

rNew <- lapply(1:(length(r)/2), function(x) paste(r[2*x-1], r[2*x]))
#r <- r[!is.na(r)]
write.table(rNew, file = "MyData.csv",row.names=FALSE, na="",col.names=FALSE, sep="\n")



# Switch from list to vector
AllText <- unlist(AllText)
#Remove stopwords from merged text
system.time(AllText <- removeWords(AllText, stopwords())) # <3 min


# Get the Unigrams and sort them for ease :P + Stemmer
# Split up the texts into words
system.time(SplittedText <- strsplit(AllText, "^ +| +| +$")) # 1~2 min

#Get unique words in each document : Helps in getting IDF
system.time(
  {
    uSplitted <- lapply(SplittedText, FUN = unique)
    DF <- table(unlist(uSplitted))
    IDF <- mainListSize/(DF + 1)
  }
) #25 sec

#Get tf of individual terms within documents
system.time(TF_Table <- lapply(SplittedText, FUN = table))
#Get unigrams
system.time(Unigrams <- sort(unique(stemDocument(unique(unlist(SplittedText))))))

#TODO:
## CREATE NEW DTM FROM THE TF_TABLE - SHOULD BE OPTIMIZED


#HAVE TO GET PAGERANK
# 1. CREATE INFLUENCE MATRIX OF AUTHORS

# -- TAKE OUT PID, UID, HLP, RATING
rall <- unlist(lapply(list(gsub("^(?!(productid|helpfulness|score|userid))[^\n]*", replacement = NA, resProcessed, perl = T)), function(x) x[!is.na(x)]))
# -- PID INTO LIST
rpid <- unlist(lapply(list(gsub("^(?!(productid))[^\n]*", replacement = NA, rall, perl = T)), function(x) x[!is.na(x)]))
rpid <- gsub("productid:[ ]+", "", rpid)

# -- UID INTO LIST
ruid <- unlist(lapply(list(gsub("^(?!(userid))[^\n]*", replacement = NA, rall, perl = T)), function(x) x[!is.na(x)]))
ruid <- gsub("userid:[ ]+", "", ruid)

# -- RATING/SCORE INTO LIST
rscore <- unlist(lapply(list(gsub("^(?!(score))[^\n]*", replacement = NA, rall, perl = T)), function(x) x[!is.na(x)]))
rscore <- as.vector(gsub("score:[ ]+|[ ]+", "", rscore), mode = "integer")/10

# -- HELPFULNESS INTO LIST
rhlp <- unlist(lapply(list(gsub("^(?!(helpfulness))[^\n]*", replacement = NA, rall, perl = T)), function(x) x[!is.na(x)]))
rhlp <- gsub("helpfulness:[ ]+", "", rhlp)
lapply(strsplit(rhlp, " "), as.numeric)
rhlp <- matrix(unlist(lapply(strsplit(rhlp, " "), as.numeric)), ncol = 2, byrow = T)
rhlp <- rhlp[,1] / rhlp[,2] #GET HELPFULNESS AS FRACTION
rhlp[is.nan(rhlp)] <- 0 #Replace NaNs in rhlp by zero

#ADJ MATRIX
uniqUids <- unique(ruid) #ALL DISTINCT UIDs
influenceMat <- matrix(0.0, nrow = length(uniqUids), ncol = length(uniqUids))
# influenceMat <- matrix(1:length(uniqUids)^2, nrow = length(uniqUids), ncol = length(uniqUids))
rownames(influenceMat) <- uniqUids
colnames(influenceMat) <- uniqUids

uniqPids <- unique(rpid) #ALL DISTINCT PIDS
# FIND OUT ALL THE REVIEWS FOR A PROD-ID
pIndexInMainList <- lapply(uniqPids, function(x) which(rpid %in% x))
names(pIndexInMainList) <- uniqPids

# CHECKING FOR EACH PID WHOEVER HAS REVIEWED IT AND ADDING EDGE TO POSSIBLE N(C)2 EDGES
temp <- lapply(uniqPids, function(pid){
  # CHECK WHICH USERS HAS REVIEWED THIS ITEM
  allPRS_MList <- pIndexInMainList[[pid]]
  Vp <- length(allPRS_MList)
  tk <- lapply(1:Vp, function(i){
    #FORM PAIRS OF REVIEWS FROM MAINLIST WITH SAME PROD ID
    prs <- cbind(allPRS_MList[i], allPRS_MList[i:length(allPRS_MList)])
    #GET THE USERID PAIRS
    prsUid <- cbind(ruid[prs[,1]], ruid[prs[,2]])
    # CALCULATE INFLUENCE(I,J,P)
    influenceMat[prsUid] <<- influenceMat[prsUid] + (rhlp[prs[,1]]*(5 - abs(rscore[prs[,1]] - rscore[prs[,2]]))/(5*Vp))
    # print(influenceMat[prsUid])
  })
})
rm(temp)
gc()
influenceMat <- influenceMat/apply(influenceMat, MARGIN = 1, sum)
#InfluenceMat is ready
influenceMat[is.na(influenceMat)] <- 0