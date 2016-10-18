/*
 * KMeans.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: bdankwa
 */

#include <math.h>
#include <float.h>
#include "KMeans.h"
#include <iostream>
#include <cstdlib>

using namespace std;

KMeans::KMeans(const vector<dataPoint*>& rawData,
		       vector<dataPoint*>& initialCentroids) {
	// Initialize private data.
	data = rawData;
	N = rawData.size();
	Nc = initialCentroids.size();
	isConverged = false;
	centroids = initialCentroids;
	

}

KMeans::~KMeans() {
	// TODO Auto-generated destructor stub

}

// Method for calculating kmean
bool KMeans::cluster(int iterLimit, vector<unsigned int>& assignments) {
	bool tp_Converged; // Converged flag
	double totalDist = 0; // for debuging	
	vector<unsigned int> clusterSize(Nc,0); // Number of data points per cluster
	int iterNo = 0; // for debuging

	tp_Converged = false;
	
	// Determine assignments usign current centroids.
	totalDist = assignment(assignments, clusterSize);
	// Recalculate centroids
	calculateCentroid(clusterSize);
	
	while(!converged() && (iterLimit > 0)){
	  tp_Converged = cluster((iterLimit-1), assignments);
	}
	
	/*for(int i=0; i<iterLimit; ++i){ // for maximum allowable iterations,
	  iterNo++;
	  // Determine assignments usign current centroids.
	  totalDist = assignment(assignments, clusterSize);
	  // Recalculate centroids
	  calculateCentroid(clusterSize);

		if(converged()){ // If kmeans converge before iterLimit, terminate looping.
			tp_Converged = true;
			//cout << "Converged at iteration No.: " << iterNo << endl;
			break;
		}	
	}*/

	return tp_Converged;
}

// Method for calculating probability kmean
void KMeans::dataDistribution(int iterLimit, int runs, vector<vector<unsigned int> >& distribution) {
       vector<unsigned int> assignments(N,0); // assignments
       //Number of times clusters are visited by a given data point.
       vector<unsigned int> clusterOccurance(Nc,0);        
       unsigned int randIdx; // The index of a randomly selected data point.
       
       srand(time(NULL)); // seed suedo-random generator.
       
	for(int i =0; i<runs; ++i){ // for a given number of runs,
	   fill(assignments.begin(),assignments.end(),0); // Initialize assignments to 0s
	   //fill(clusterOccurance.begin(),clusterOccurance.end(),0);
	   
	   // Randonly select Nc number of data points as centroids - Forgy method
	    for(unsigned int j = 0; j< Nc; ++j){		
		randIdx = (rand() % N);
		centroids.at(j)->point->x = (data.at(randIdx))->point->x;
		centroids.at(j)->point->y = (data.at(randIdx))->point->y;
		centroids.at(j)->point->z = (data.at(randIdx))->point->z;		
	    }		
	    cluster(iterLimit, assignments); // Run Kmean
	    distribution.push_back(assignments); // Store the results.	
	}
}

// Method for calculating cluster assignments
double KMeans::assignment(vector<unsigned int>& assignments, vector<unsigned int>& clusterSize){

	static unsigned int minIdx = 0; // Index of the minimum distance of a data point from a given centroid
	double minDist = 0;
	double totalDistance = 0;
	static vector<unsigned int> prevCluster(N,0); // previous cluster of a data point.

	static vector<double> distance(Nc,0.0); // Distance vector

	static unsigned int moveCount = 0; // Number of moves after an assignment
	static unsigned int prevMoveCount = 0; // previous number of moves
	
	isConverged = false; // Initialize isConverged to false.

	// Clear ClusterSize
	fill(clusterSize.begin(),clusterSize.end(),0.0);
	
	/* For all data points, calculate distance from curren centroids.
	 * Find the closest centroid and switch cluster.
	 * Keep count of moves
	 * If no data point changes cluster from the previous assignments, report converged!
	 */
	for(unsigned int i = 0; i<N; ++i){
		for(unsigned int j = 0; j<Nc; ++j){
			 distance[j]= calculateDistance(centroids.at(j),data.at(i)) ;
		}
		prevCluster[i]= (data.at(i))->cluster; // Save previous cluster
		minIdx = findMinDistance(distance,minDist); // calculate distances from current centroids
		(data.at(i))->cluster = minIdx; // Save new assignment in data structure
		assignments.at(i) = minIdx; // Save mew assignment in assignments vector
		
		clusterSize[minIdx]++; // increment cluster cout
		
		totalDistance += minDist; // increment total distance (dubugging/research)

		if((prevCluster[i]) != ((data.at(i))->cluster)){ // If any data point changes clusters
			
		moveCount++; // keep track of moves
			
		}
		prevCluster[i] = minIdx;

	}
	if(moveCount == prevMoveCount){ // If no data point changes cluster, report convergence.
		isConverged = true;
	}
	prevMoveCount = moveCount; // save moveCount

	return totalDistance;
}

// Method for calculating data centroids
void KMeans::calculateCentroid(vector<unsigned int>& clusterSize){

	for(unsigned int k = 0; k<Nc; ++k){ // Clear centroids
		centroids.at(k)->point->x = 0.0;
		centroids.at(k)->point->y = 0.0;
		centroids.at(k)->point->z = 0.0;
	}

	for(unsigned int i = 0; i<Nc; ++i){ // calculate new centroids
		for(unsigned int j = 0; j<N;  ++j){
			if((data.at(j)->cluster) == i){
				centroids.at(i)->point->x += (data.at(j)->point->x)/(clusterSize[i]);
				centroids.at(i)->point->y += (data.at(j)->point->y)/(clusterSize[i]);
				centroids.at(i)->point->z += (data.at(j)->point->z)/(clusterSize[i]);
			}
		}
	}
}

// Method for calculating the euclaudian distance between two points
double KMeans::calculateDistance(dataPoint* cents, const dataPoint* pixel){
	// Calculate Eucludian distance between a data point and a centroid.
	return sqrt((cents->point->x - pixel->point->x)*(cents->point->x - pixel->point->x)+
		   (cents->point->y - pixel->point->y)*(cents->point->y - pixel->point->y)+
		   (cents->point->z - pixel->point->z)*(cents->point->z - pixel->point->z));
}

// Method to test if kmean has converged
bool KMeans::converged(){

	return isConverged;
}

// Method for finding the closest centroid from a given point.
unsigned int KMeans::findMinDistance(vector<double> distance, double & minDist){
	// find the miminum distance in the distance vector and return its index.
	unsigned int index = 0;
	double n = distance[0];

	for(unsigned int i=1; i<Nc; ++i){
		if(distance[i] < n){
			n = distance[i];
			index = i;
		}
	}
	minDist = n;
	return index;
}

// Set new centroids
void KMeans::setCentroids(vector<dataPoint*> currCentroids){
  
  centroids = currCentroids;  
}

// Read current centroids
void KMeans::getCentroids(vector<dataPoint*>& currCentroids){
  
  centroids = currCentroids;
}