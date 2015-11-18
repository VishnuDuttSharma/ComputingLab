fileName <- "testdata.txt"
res <- readChar(fileName,file.info(fileName)$size)
res <- unlist(strsplit(res, "\r*\n"))
buildCorpus <- function(resource){
  corpus <- list(vector("character", 500000), vector("character", 500000), vector("character", 500000), vector("character", 500000), vector("character", 500000), vector("character", 500000), vector("character", 500000), vector("character", 500000))
  names(corpus) <- c("productId", "userId", "profileName", "helpfulness", "score", "time", "summary", "text")
  i <- 0
  for(file_line in res){
    if(nchar(file_line) < 5){
      next
    }
    s <- gsub("(^[^/]*/)", "",file_line)
    s <- strsplit(s, ": ")
    if(s[[1]][1] == "productId"){
      i <- i+1
    }
    corpus[[s[[1]][1]]][i] <- s[[1]][2]
  }
  print(i)
  return(corpus)}
system.time(corpus <- buildCorpus(res))
corpus <- buildCorpus(res)

#SO Solution
# mytable <- read.table(text=gsub("^([^:]*:)|:", "\\1", res), sep = ":", quote = "")
# mytable$id <- rep(1:(nrow(mytable)/8), each = 8)
# res <- reshape(mytable, direction = "wide", timevar = "V1", idvar = "id")


w <- gsub("[^:]|: ","", w)
wlist <- strsplit(w, " ")

token_indices <- wordpunct_tokenizer(w)
starts <- token_indices$start
ends <- token_indices$end
numWords <- length(starts)
wlist2 <- vector("list", numWords)
for(i in 1:numWords){
  wlist2[[i]] <- substr(w, start=starts[i], stop=ends[i])}
wvec <- unlist(wlist2)
stems <- wordStem(wvec, "english")

