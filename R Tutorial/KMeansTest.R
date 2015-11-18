# KMeansTest.R

# Function to plot points and centroids
plotClustering <- function(points=testPoints, centroids=testCentroids, clusterID=testClusterID)
{
  # Create an empty plot
  xMin <- min(points[,1], centroids[,1], na.rm=T)
  xMax <- max(points[,1], centroids[,1], na.rm=T)
  yMin <- min(points[,2], centroids[,2], na.rm=T)
  yMax <- max(points[,2], centroids[,2], na.rm=T)
  yLocation <- xLocation <- c()
  plot(c(), xlab='X', ylab='Y', ylim=c(yMin, yMax), xlim=c(xMin, xMax))
  
  # Add points
  for (label in 1:nrow(centroids))
  {
    pointsOfCluster <- points[clusterID == label,]
    if(nrow(pointsOfCluster) > 0)
    {
      points(pointsOfCluster, pch=21, col=label+1, cex=4, lwd=2)
      text(x=pointsOfCluster[,1], y=pointsOfCluster[,2], label, cex=1, col=label+1)
    }
  } # for
  
  # Add Centroids
  iter<-1:nrow(centroids)
  points(centroids, pch=24, cex=5, col=iter+1, bg='lightgrey')
  text(x=centroids[iter,1], y=centroids[iter,2], iter, cex=2, col=iter+1)
  
  Sys.sleep(2)
} # plotClustering

# The following can be useful for testing
testPoints <- matrix(nrow=83, ncol=2, byrow= T, data=c(
  1.91,1.43,
  0.9,0.79,
  1.26,0.52,
  0.61,1.55,
  1.25,0.66,
  1.04,0.62,
  0.53,1.33,
  0.99,1.27,
  1.11,1.04,
  0.1,2.41,
  -0.15,1.83,
  0.83,1.02,
  0.72,1.17,
  0.69,0.97,
  0.74,0.91,
  0.72,0.14,
  1.09,0.53,
  0.68,1.15,
  0.67,0.96,
  0.82,0.87,
  0.74,0.27,
  0.94,-0.15,
  0.64,0.82,
  1.44,0.72,
  0.76,0.84,
  1.06,1.52,
  0.79,0.93,
  0.88,0.91,
  0.76,0.87,
  0.85,0.93,
  0.88,0.97,
  0.75,1,
  0.83,0.86,
  0.85,0.88,
  0.35,0.55,
  0.63,-1.99,
  -0.14,-0.78,
  -0.04,-0.32,
  0.3,0.67,
  -0.52,-1.75,
  -0.27,-0.7,
  -0.32,-0.51,
  -0.08,-0.37,
  -0.39,-0.55,
  -0.06,-0.42,
  0.09,-0.48,
  -0.51,0.64,
  -0.22,-0.49,
  -0.03,-0.51,
  -0.12,-0.32,
  0.01,-0.48,
  -0.21,-0.57,
  -0.21,-0.32,
  0.37,-0.28,
  1.18,-1.51,
  0,-0.41,
  0,-0.44,
  -0.66,-2.27,
  -0.1,-0.67,
  1.01,-0.32,
  1.19,0.43,
  -0.3,-1.26,
  -2.2,-1.85,
  -1.82,-0.16,
  -1.33,-0.89,
  -0.84,0.05,
  -2.17,-0.38,
  -1.67,-0.53,
  -1.38,-1.75,
  -1.39,-0.98,
  -1.32,-0.33,
  -1.49,-1.41,
  -2.16,-1.33,
  -1.64,-0.9,
  -1.44,-0.72,
  -1.58,-0.77,
  -1.53,-0.66,
  -1.53,-0.81,
  -0.27,-0.87,
  -1.32,-0.94,
  -0.89,-1.73,
  -0.33,0.55,
  -1.29,-0.7))