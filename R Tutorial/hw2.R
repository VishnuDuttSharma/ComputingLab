# *****************************
#     Mini-course on R 
#     Class/Homework 2
# *****************************

# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")

# Get the test data from KMeansTest.R
source("KMeansTest.R")

# simpleKMeans is a 2D K-means implementation.  
# The function takes points and initial centroids and 
# returns K-mean centroids.  The function does not
# normalize the inputs.
simpleKMeans <- function(points = testPoints, centroids = testCentroids)
{ 
  # Get ridiculous values for the initial cluster ids
  clusterIDOld <- -1
  # Determine the number of centroids
  numberOfCentroids <- nrow (centroids)
  # repeat the following processes using a loop.  Use a
  # for loop of size 20 to prevent infinite loops
  for(iter1 in 1:20)
  {
    # For each point find its closest cluster centre aka centroid
    clusterID <- simpleAssignToCentroids(points, centroids)
    # Plot the points and centroids
    plotClustering(points, centroids, clusterID)
    # If there was no change in cluster assignments, then stop
    if (sum(clusterID != clusterIDOld) < 1)
    {
      # Use "break" to break out of the loop
      break
    } # ends the if clause
    # For each cluster of points determine its centroid
    centroids <- simpleDetermineCentroids(points, clusterID)
    # remember clusterID before clusterID is re-assigned in the next iteration
    clusterIDOld <- clusterID
  } # ends the for loop
  # Return the centroids
  centroids
}

# For each cluster of points determine its centroid
# The inputs are the points and the cluster ids of the points
# The output is a vector of the new centroids
simpleDetermineCentroids <- function(points=testPoints, clusterID=testClusterID)
{
  # How many centroids will we make?  What is the maximum cluster label? 
  numberOfCentroids <- max(clusterID)
  # Create a matrix where each row is a centroid of 2 dimensions
  centroidMatrix <- matrix(nrow = numberOfCentroids, ncol = 2)
  # For loop through each cluster id 
  for (clusterNo in 1:numberOfCentroids)
  {
    # Get only the points from one cluster
    #pointInCluster <- points[clusterID == clusterNo]
    # Determine the mean of that cluster in the 1st dimension
    meanX <- mean(points[clusterNo == clusterID, 1] )
    # Determine the mean of that cluster in the 2nd dimension
    meanY <- mean( points[clusterNo == clusterID,2] )
    # Assign the mean in the 1st dimension to the centroid
    centroidMatrix[clusterNo,1] <- meanX
    # Assign the mean in the 2nd dimension to the centroid
    centroidMatrix[clusterNo,2] <- meanY
  } # Ends the for loop through each cluster id
  # Return the centroids
  return (centroidMatrix)
} # simpleDetermineCentroids

# A function that returns the cluster IDs for each point
# The function takes the points
# The function takes centroids 
# The cluster that is closest to each point will determine the cluster ID for that point
# A cluster ID indicates the allegiance of a point to a cluster
simpleAssignToCentroids <- function(points = testPoints, centroids=testCentroids)
{
  # Get the number of centroids
  numberOfCentroids <- nrow (centroids)
  
  # Get the number of points
  numberOfPoints <- nrow(points)
  # Create a matrix that will contain the squared distances from each point to each centroid
  # The matrix has numberOfPoints rows and numberOfCentroids columns
  
  distMatrix <- matrix(0, nrow = numberOfPoints, ncol = numberOfCentroids)
  
  # Determine the distance from the centroid to each point
  # For loop for each point number
  for (pointNo in 1:numberOfPoints)
  {
    # For loop for each centroid number
    for (centroidNo in 1:numberOfCentroids)
    {
      # What is the difference between the current point and the current centroid?
      # In other words: What is the vector between the point and centroid?
      currentDiff <- (testPoints[pointNo,] - centroids[centroidNo,])
      # What is the distance squared of this vector?
      # In other words: what is the sum of the squares of the vector elements?
      currentDist <- sum( currentDiff ^ 2 )
      # If the distance squared was NA then make it infinite
      # Assign the distance squared to the proper element in the matrix created above
      currentDist[is.na(currentDist)] <- Inf
      
      distMatrix[ pointNo, centroidNo] <- currentDist
      # End the for loop for each centroid number
    } # Ends the for loop for each centroid number
  } # Ends the for loop for each point number
  # Determine the clusterIDs of the closest
    clusterIDs <- apply(distMatrix, MARGIN = 1 , function(x){ return (which(x == min(x)) )})
    
    return ( clusterIDs )
} # simpleAssignToCentroids

# Test with three centroids
# First example: Initial centroids are at (1,-2), (-2,1) and (0,0)
testCentroids <- matrix(c(1, -2, 0, -2, 1, 0), nrow=3)
simpleKMeans(centroids = testCentroids)

#           [,1]       [,2]
#[1,] -0.0332000 -0.6508000
#[2,] -1.5163158 -1.0057895
#[3,]  0.7610256  0.9071795


# Second example: Initial centroids are at (-2,-2), (-1,-2) and (1,1)
testCentroids <- matrix(c(-2, -1, 1, -2, -2, 1), nrow=3)
simpleKMeans(centroids = testCentroids)

#           [,1]       [,2]
#[1,] -1.5163158 -1.0057895
#[2,] -0.0332000 -0.6508000
#[3,]  0.7610256  0.9071795


