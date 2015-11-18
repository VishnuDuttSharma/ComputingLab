library(tm)
library(NLP)
library(SnowballC)

## Function to mimic printf()
printf <- function(...) cat(sprintf(...))



## Function to read data form the file
# Filename: Name of the file to be read
readData <- function(fileName){
  myFile <- file(fileName)
  mytxt <- readLines(myFile)
  mytable <- read.table(text=gsub("^([^:]*:)|:", "\\1", mytxt), sep = ":", quote = "")
  mytable$id <- rep(1:(nrow(mytable)/8), each = 8)
  res <- reshape(mytable, direction = "wide", timevar = "V1", idvar = "id")
  on.exit(close(myFile))
  return (res)
}

## Function to perform stemming on the corpus
# dfCorpus: Corpus
stemCorpus <- function(dfCorpus){
  PlainTextDocument(paste(stemDocument(unlist(strsplit(as.character(dfCorpus), " "))),collapse=" "))
}

## Function to make a named vector with names made of elements in vocabulary
# vocab: Vovabulary, a vector containing unigrams/bigrams
makeVector <- function(vocab){
  weightList = c(rep(0, length(vocab)))
  names(weightList) = vocab
  return (weightList)
}

## Function to convert into dataframe without any processing
## myTable: All text joined together
makeRawFrame <- function(myTable){
  x = myTable
  dfCorpus = Corpus(VectorSource(x))
  dataframe <- data.frame(text=unlist(sapply(dfCorpus, '[', "content")), stringsAsFactors=F)
  
  return(dataframe)
}

## Function to convert into dataframe after processing
## myTable: All text joined together
makeFrame <- function(myTable){
  #x = myTable$'V2.review/text'
  #df = as.data.frame(x)
  
  x = myTable
  
  pat <- "[?[:punct:]0-9]+"
  x <- gsub(pattern = pat, '', x)
  
  
  dfCorpus = Corpus(VectorSource(x))
  dfCorpus <- tm_map(dfCorpus, stemCorpus)
  #dfCorpus <- tm_map(dfCorpus, removePunctuation)
  #dfCorpus <- tm_map(dfCorpus, removeNumbers)
  #dfCorpus <- tm_map(dfCorpus, tolower)
  dfCorpus <- tm_map(dfCorpus, removeWords, stopwords("english"))
  dfCorpus <- tm_map(dfCorpus, stripWhitespace)
  dfCorpus <- tm_map(dfCorpus, PlainTextDocument)

  
  dataframe <- data.frame(text=unlist(sapply(dfCorpus, '[', "content")), stringsAsFactors=F)

  return(dataframe)
}


## Function to make vocabulary for unigrams
## dataframe: dataframe containing text
makeVocab <- function( dataframe ){
  #dataframe = makeFrame(myTable)
  #print (length(dataframe[[1]]))
  k <- ""
  for (i in 1:length(dataframe[[1]]))
    k <- paste(k,dataframe[[1]][i])

  rawVocab = strsplit(k, " ")
  vocab = unique( rawVocab )
  
  
  lp = wordStem(vocab, language="porter")
  lis = unique(vocab[[1]])
  lis = lis[lis != ""]
  
  lis = sort(lis, decreasing = FALSE)
  
  
  
  return (lis)  
}

## Function to make vocabulary for biigrams
## dataframe: dataframe containing text
makeBigramVocab <- function(dataframe){

  k <- ""
  for (i in 1:length(dataframe[[1]]))
    k <- paste(k,dataframe[[1]][i])
  
  #rawVocab = as.list(strsplit(k, " ")[[1]])
  rawVocab = strsplit(k, " ")
  vocab = unique( rawVocab )
  
  
  lp = wordStem(vocab, language="porter")
  
  #lis = unique(vocab[[1]])
  lis = vocab[[1]]
  lis = lis[lis != ""]
  
  vec <- sapply(1:(length(lis)-1), function(x) paste(lis[x], lis[x+1]) )
  
  vec <- unique(vec)
  
  vec = sort(vec, decreasing = FALSE)
  
  return (vec)
}

## Function to make vocabulary for biigrams
## dataframe: dataframe containing text
# makeBigramVocabMod <- function(dataframe, dtm_uni, vocab_uni){
#  
#  k <- ""
#  for (i in 1:length(dataframe[[1]]))
#    k <- paste(k,dataframe[[1]][i])
#  
#  #rawVocab = as.list(strsplit(k, " ")[[1]])
#  rawVocab = strsplit(k, " ")
#  vocab = unique( rawVocab )
#  
#  
#  lp = wordStem(vocab, language="porter")
#  
#  #lis = unique(vocab[[1]])
#  lis = vocab[[1]]
#  lis = lis[lis != ""]
#  
#  p <- colSums(dtm_uni)
#  lis <- lis[p[lis] > 2]
#  
#  vec <- sapply(1:(length(lis)-1), function(x) paste(lis[x], lis[x+1]) )
#  
#  vec <- unique(vec)
#  
#  vec = sort(vec, decreasing = FALSE)
#  
#  return (vec)
#}


## Function to create a vector in Vector Space for unigrams
## element: text corresponding to a single document 
## vocab: Vocabulary (unigram)
makeVS <- function(element, vocab){
  
    vec <- makeVector(vocab)
    dataset <- strsplit(element," ")
    for (i in vocab){
      vec[[i]] <- length((grep(i, dataset[[1]])))
    } 
    return (vec)
}

## Function to create a vector in Vector Space for bigrams
## element: text corresponding to a single document 
## vocab: Vocabulary (bigram)
makeBigramVS <- function(element, vocab){
  
  vec <- makeVector(vocab)
  dataset <- strsplit(element," ")
  
  newDataset <- sapply(1:(length(dataset[[1]])-1), function(x) paste(dataset[[1]][x], dataset[[1]][x+1]) )
  
  for (i in vocab){
    
    vec[[i]] <- length((grep(i, newDataset)))
  } 
  return (vec)
}

## Function to make Document Term Matrix for unigram
## myTable: All text (in all documents) joined together
## vocab: Vocabulary
makeDTM <- function(myTable, vocab){
  
  vec <- makeVS(myTable[[1]][1], vocab) 
  #vec <- makeBigramVS(myTable$text[[1]], vocab) 
  if( length(myTable[[1]]) > 1){
    for (i in 2:length(myTable[[1]])){
      vec <- c(vec, makeVS(myTable[[1]][i], vocab))
    }
  }
  
  mat = matrix(vec, nrow =length(myTable[[1]]), ncol = length(vocab), byrow = T)
  colnames(mat) <- vocab
  
  return (mat)
}


## Function to make Document Term Matrix for bigram
## myTable: All text (in all documents) joined together
## Vocab: Vocaulary (bigram)
makeBigramDTM <- function(myTable, vocab){
  
  #vec <- makeVS(myTable$text[[1]], vocab) 
  vec <- makeBigramVS(myTable[[1]][1], vocab) 
  
  if( length(myTable[[1]]) > 1){
    for (i in 2:length(myTable[[1]])){
      vec <- c(vec, makeBigramVS(myTable[[1]][i], vocab))
    }
  }
  
  mat = matrix(vec, nrow =length(myTable[[1]]), ncol = length(vocab), byrow = T)
  colnames(mat) <- vocab
  
  return (mat)
}

## Function to calculate IDF
## mat: DTM for which IDF is to be determined
getIDF <- function(mat){
  idf <- (nrow(mat)/( 1 + colSums( mat  > 0)))
  return (idf)
}

## Function to make TF-IDF matrix for query
## mat: DTM for which TF-IDF is to be determined
getQueryTF_IDF <- function(mat){
  idf = getIDF(mat)
  
  tf_idf <- sweep(mat, 2, idf, '*')
  tf_idf <- replace(tf_idf, tf_idf == 0, 1)
  tf_idf <- log(tf_idf)
  
  return (tf_idf)
}

## Function to make TF-IDF matrix
## mat: DTM for which TF-IDF is to be determined
getTF_IDF <- function(mat){
  idf = getIDF(mat)
  
  tf_idf <- sweep(mat, 2, idf, '*')
  tf_idf <- replace(tf_idf, tf_idf == 0, 1)
  tf_idf <- log(tf_idf)
  
  return (tf_idf)
}

## Function to find cosine product between two matrices
## mat1: First input matrix
## mat2: Second input matrix
cosProduct <- function(mat1, mat2){
  
}


## Function to calculate score for similarity for a query
## queryVec: Vector in our vector space model for query
## dtm: DTM obtained for corpussimil = sum(mat1 * mat2)
  k1 = sum(mat1^2)
  if (k1 == 0)
    return (0)
  
  k2 = sum(mat2^2)
  if (k2 == 0)
    return (0)
  
  simil = simil/sqrt( k1 * k2 )
  
  return (simil)
findMatch <- function( queryVec, dtm ){
  score <- c( cosProduct( queryVec, dtm[1,]))
  
  for ( i in 2:nrow(dtm) ){
    score <- c(score,  cosProduct( queryVec, dtm[i,]) )
  }
  return (score)
}

## Function to print top 'returnCount' number of matching results
## score: Score vector calculted by taking cosine product of query vectorfor each document with DTM
## returnCount: number of results to be printed
## dataTable: Vector containg all text joined (from all documents)
getResult = function(score, returnCount, dataTable){
  sortScore = unique(sort(score, decreasing = TRUE))
  #indices <- which( score == max(score) )
  indices <- list()
  for (i in 1:length(sortScore)){
    indices <- c(indices, which(score == sortScore[i], arr.ind = TRUE))
  }
  
  # dataframe <- makeRawFrame(dataTable)
  
  count = 0
  for (i in indices){
    if( count >= returnCount)
      break
    count = count + 1
    printf ("Result %d: \n", count)
    print(dataTable[i])
    
  }
}


## Function to read file in optimum time
## fileName: file to be read
readOptm <- function(fileName){
  res <- tolower( readChar( fileName, file.info(fileName)$size))
  res <- unlist(strsplit(res, "\r*\n"))
  
  pat <- "^(review/summary)|^(review/text)"
  r <- grep(pattern = pat, res, value = TRUE)
  r <- lapply(r, function(x) strsplit(x, ':')[[1]][2])
  
  rNew <- lapply(1:(length(r)/2), function(x) paste(r[2*x-1], r[2*x]))
  
  return (rNew)
}


#####################################################
# Read file
rest1 <- readOptm( "smallData.txt" )

#write.table(rNew, file = "MyData.csv",row.names=FALSE, na="",col.names=FALSE, sep="\n")


# Convert into dataframes
rest = makeFrame(rest1)

# Make unigram vocaulary
vocab_Uni = makeVocab(rest)
# Make Vector space model with vocabulary
vector_Uni = makeVector(vocab_Uni)
# Make DTM for all documents
DTM_Uni = makeDTM(rest, vocab_Uni)
# Matrix TF-IDF matrix from DTM
tf_idf_Uni = getTF_IDF(DTM_Uni)

# Make bigram vocaulary
vocab_Bi = makeBigramVocab(rest)
# Make Vector space model with vocabulary
vector_Bi = makeVector(vocab_Bi)
# Make DTM for all documents
DTM_Bi = makeBigramDTM(rest, vocab_Bi)
# Matrix TF-IDF matrix from DTM
tf_idf_Bi = getTF_IDF(DTM_Bi)

## Document to Document similarity heatmap
mat1 <- matrix(0, nrow =  nrow(DTM_Uni), ncol = nrow(DTM_Uni))

for (i in 1:nrow(DTM_Uni)){
  for (j in 1:nrow(DTM_Uni)){
    mat1[i,j] <- cosProduct(DTM_Uni[i,], DTM_Uni[j,])
  }
}


# make heatmap for unigram model 
png(file="unigram.png")
hMapUni <-  heatmap(mat1, Rowv=NA, Colv=NA, col = cm.colors(256), scale="column", margins=c(5,10))
dev.off()

for (i in 1:nrow(DTM_Bi)){
  for (j in 1:nrow(DTM_Bi)){
    mat1[i,j] <- cosProduct(DTM_Uni[i,], DTM_Uni[j,])
  }
}


# make heatmap for bigram model 
png(file="bigram.png")
hMapBi <- heatmap(mat1, Rowv=NA, Colv=NA, col = cm.colors(256), scale="column", margins=c(5,10))
dev.off()



####  Query ####
## Get query as user input
query <- readline(prompt="Enter single query: ")
query <- tolower(query)
print (query)

## Make data frame from query after processing it
queryRest = makeFrame(unlist(query)  )

# Make unigram vocaulary for query
queryVocab_Uni = makeVocab(queryRest)
# Get DTM for query for unigrams
queryMat_Uni = makeDTM( queryRest, vocab_Uni)
# Make TF-IDF marix for query (for unigrams)
queryTF_IDF_Uni = getQueryTF_IDF(queryMat_Uni)
# Get score for matching in unigram model for each document
score_Uni = findMatch(queryMat_Uni, DTM_Uni )

# Make bigram vocaulary for query
queryVocab_Bi = makeBigramVocab(queryRest)
# Get DTM for query for bigrams
queryMat_Bi = makeBigramDTM( queryRest, vocab_Bi)
# Make TF-IDF marix for query (for bigrams)
queryTF_IDF_Bi = getQueryTF_IDF(queryMat_Bi)
# Get score for matching in bigram model for each document
score_Bi = findMatch(queryMat_Bi, DTM_Bi)

# alpha: Weight for unigram model scores
# beta:  Weight for bigram model scores
alpha = 1.0
beta = 0.0

# get final score after providing weights to unigram and bigrams
score = alpha*score_Uni + beta*score_Bi
# get top 25 simlarity matches for query
getResult( score, 25, rest1)


