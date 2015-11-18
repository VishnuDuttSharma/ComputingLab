library(rmr2)
wordcount = function(input, output = "/user/mtech/14CS60R20/Out3", pattern = " "){
  
  wc.map=
          function(., lines){
            
            keyval(
              unlist(
                strsplit(
                  x = lines,
                  split = pattern)),
              1)
            
          }
  wc.reduce =
              function(word, counts ){
                keyval(word, sum(counts))
              }
  mapreduce(
            input = input ,
            output = output,
            input.format = "text",
            map = wc.map,
            reduce = wc.reduce,
            combine = T
    )
}

wordcount("/user/mtech/14CS60R20/abc.txt")

results <- from.dfs ("/user/mtech/14CS60R20/Out2")
results.df <- as.data.frame (results, stringsAsFactors=F)
colnames(results.df) <- c ('word', 'count')
head (results.df)