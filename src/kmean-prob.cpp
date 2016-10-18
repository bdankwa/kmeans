/*
 * kmean-simple.cpp
 *
 *  Created on  : Dec 1, 2013
 *      Author  : Boakye Dankwa
 *  Description : CSCI362 Project 1 - Simple K-means implementation in C++, Ansi-style
 */

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "KMeans.h"
#include "utils.h"

using namespace std;


// Converts the assignments vector to PGM vector scaled to [ 0 -255 ]
void assignments2PGM(vector<unsigned int> &assignments, int k, double centroids[], vector<unsigned int>& pgm){
	
	unsigned int N = assignments.size();	

	for(unsigned int i=0; i<N; ++i){
		for(int j=0; j<k; ++j){
			if(assignments[i] == static_cast<unsigned int>(j)){
				pgm[i] = 255*(centroids[j]);				
			}
		}
	}
}
/********************************************************************************
 * This method calculates the initial centroids for the kmean algorithm
 * Two possible stratagies:
 * 	1. Select the minimum and maximum of the data points,
 * 	   then equally space the remaining of the centroids between them.
 * 	2. Forgy method - Randomly selected data points as initial centroids.
 * Only one method can be used at a time.
 */
void calcInitialCentroids(const vector<double>& normImage,double initialCentroids[], unsigned int N, int k ){
	double min;
	double max;

	//Centroids equally spaced between min and max
	min = *(min_element(normImage.begin(),normImage.end()));
	max = *(max_element(normImage.begin(),normImage.end()));

        initialCentroids[0] = min;
	initialCentroids[k-1] = max;
	for(int i = 1; i< k-1; ++i){
		initialCentroids[i] = min + i*(max -min)/(k-1);

	}

	// Forgy method (random)
	/*srand(time(NULL));
	for(unsigned int i = 0; i< k; ++i){
		// select k random observations as centroids
		randIdx = (rand() % N);
		initialCentroids[i] = normImage[randIdx];
		cout << "indexes = " << randIdx << endl;
		cout <<"centroids = " << initialCentroids[i] << endl;

	}*/
}

int main(int argc, char ** argv) {
        
	int k; // Number of clusters
	unsigned int N; // Number of data points (pixels) in image
	char* InFileName = NULL; // File name; NULL for stdin
	//char outFileName[10]; // Fine name; NULL for stdout TODO comment out for release
	
	
	
	// Error check value for k
	if(argv[1] == NULL){
	  cerr << "Usage:" << argv[0] << " Number_of_clusters" << endl;
	  cerr << "Please don't forget your clusters...:)" << endl;
	  exit(EXIT_FAILURE);
	}
	
	k = atoi(argv[1]);
	
		
	int maxIter = 1000; // set maximum iterations
	int kmeanRuns = 10; // Number of times to run Kmean.
	
	if(k > kmeanRuns){
	  cerr << " Maximum k-mean runs is set to 10, number of centroids cannot be more than 10 " << endl;
	  exit(EXIT_FAILURE);
	}
	
	/* ******************************************************************
	 * Read PGM data from stdin into a vector imageMat
	 * This will be later be used to populate the data structure
	 * for the kmean algorithm
	 *******************************************************************/
	unsigned int height, width, maxValue; // properties of PGM image
	vector<double> imageMat; // PGM image vector 
        readPGM(InFileName, height, width, maxValue, imageMat);
	
	N = imageMat.size(); // Total number of data points in the PGM data 	
	
	double initCentroids[k]; // Array of PGM centroids
	vector<dataPoint*> data; // Vector for for storing Kmean data points
	vector<dataPoint*> centroids; // Vector for storing Kmean centroids
	vector<unsigned int> assignments(N,0); // Vector for storing assignments.
	vector<vector<unsigned int> > distribution; // assignment matrix : assignments x kmeanRuns
	unsigned int probOccurance[k][N]; // Occurances of data points in the clusters
	double probMatrix[k][N]; // Probabilities of data points in clusters
	vector<double> probability(k,0); // Probabilities of a data point across clusters
	
	vector<unsigned int> PGM(N,0); // Vector for storing PGM data
	vector<double> normGrayScale(2,0); // Range of normalized gray scale values
	double probKmeanGrayVals[k]; // Scaled gray values for probability kmean arbitrary clusters
	
	pt point[N]; // N data points
	pt centroidPoints[k]; // K data points for centroids
	dataPoint* kmDataPoints[N]; // N Kmean data points
	dataPoint* kmCentroidPoints[k]; // N Kmean data points for centroids	

	
	/***************************************************
	 * Populate KMean data point and centroids vectors with PGM data
	 ***************************************************/
	// Data points
	for(unsigned int i = 0; i< N; ++i){
		point[i].x = imageMat[i];
		point[i].y = 0.0;
		point[i].z = 0.0;
		kmDataPoints[i] = new dataPoint;
		kmDataPoints[i]->point = &point[i];
		kmDataPoints[i]->cluster = 0;
		data.push_back(kmDataPoints[i]);
	}
	// Initial Centroids
	calcInitialCentroids(imageMat, initCentroids,N, k );

		for(int i = 0; i< k; ++i){
		centroidPoints[i].x = initCentroids[i];
		centroidPoints[i].y = 0.0;
		centroidPoints[i].z = 0.0;
		kmCentroidPoints[i] = new dataPoint;
		kmCentroidPoints[i]->point = &centroidPoints[i];
		kmCentroidPoints[i]->cluster = 0;
		centroids.push_back(kmCentroidPoints[i]);
	}
        /***************END vector initializations*************/
	
	/************************ K-MEAN PROBABILITY ALGORITHM **************************************/
	
	KMeans* km = new KMeans(data, centroids); // Instantiate KMeans class

	
	/* Run kmeans 'kmeanRuns' times and save the cluster assignments in 
	 * the distribution matrix*/
	km->dataDistribution(maxIter, kmeanRuns, distribution);
	
	//Initialize occurance matrix
	for(int i=0; i<k; ++i){
	    for (unsigned int j=0; j<N; ++j){
	      probOccurance[i][j] = 0;	
	    }
	  }
	
	/*****************************************************************************************
	 * Iterate through distribution matrix and count occurances of data points in the clusters.
	 * Divide each occurance by the number of Kmean runs to get probabilities of the data points
	 * in the clusters.
	 * ************************************************************************************/
	for(int i = 0; i<kmeanRuns; ++i){ // runs	  
	  for(unsigned int j = 0; j<N; ++j){ // data points	   	
	    probOccurance[(distribution.at(i)).at(j)][j]++; // populate occurance matrix
	  }	  
	}	
	for(unsigned int i=0; i< N; ++i){
	  for (int j=0; j<k; ++j){	    
	    probMatrix[j][i] = probOccurance[j][i]/static_cast<double>(kmeanRuns); // populate probability matrix.
	    probability[j] = probMatrix[j][i];
	    }
	    // create assignments based on highest probability
	    assignments[i] = findMaxProbIdx(probability);
	  }
	// set range for gray values 
	normGrayScale[0] = 0.0;
	normGrayScale[1] = 1.0;
	
	/* Choose gray values evenly spaced between [0 1] for the abitrary centroids
	 * for displaying probability kmeans results*/
	calcInitialCentroids(normGrayScale, probKmeanGrayVals,N, k );
	
	// Convert assignments to PGM data for display
	assignments2PGM(assignments,k, probKmeanGrayVals, PGM);	
	
	// Write PGM data to standard output
	writePGM(PGM, width, height, maxValue, NULL);
	
	/************************************************************************************
	 * For demonstration only: TODO comment out for release.
	 * Scale probabilities to PGM format and write each cluster to stdout.
	 * There should be a total of kmeanRuns pgm outpouts.
	 * ********************************************************************************/
	/*for(int i=0; i<k; ++i){
	  for(unsigned int j=0; j<N; ++j){
	    PGM[j] = static_cast<unsigned int>(255*probMatrix[i][j]);
	  }
	  sprintf(outFileName,"output_%d.pgm",(i+1)); 
	  //cout << "file name = " << outFileName << endl;
	  writePGM(PGM, width, height, maxValue, outFileName);
	}*/

	/********************** END K-MEAN PROBABILITY ALGORITHM***********************************/	

	 /*Delete dynamic allocations*/

	for(unsigned int i =0; i<N; ++i){
		delete[] kmDataPoints[i] ;
	}

	for(int i =0; i<k; ++i){
		delete[] kmCentroidPoints[i];
	}

	return 0;
}




