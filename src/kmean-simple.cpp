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


// Converts the assignments vector to PGM vector ( 0 -255 )
void assignments2PGM(vector<unsigned int> &assignments, vector<dataPoint*> centroids, vector<unsigned int>& pgm){
	
	unsigned int N = assignments.size();
	unsigned int k = centroids.size();

	for(unsigned int i=0; i<N; ++i){
		for(unsigned int j=0; j<k; ++j){
			if(assignments[i] == j){
				pgm[i] = 255*(centroids[j]->point->x);				
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
	char* InFileName = NULL; // File name; NULL for stdin
	
	if(argv[1] == NULL){
	  cerr << "Usage:" << argv[0] << " Number_of_clusters" << endl;
	  cerr << "Please don't forget your clusters....:)" << endl;
	  exit(EXIT_FAILURE);
	}

	k = atoi(argv[1]);
	//InFileName = argv[1];
	
	vector<double> imageMat; // PGM image vector
	unsigned int N; // Number of data points (pixels) in image
	unsigned int height, width, maxValue; // properties of PGM image
	
	/* Read PGM data from stdin into a vector imageMat
	 * This will be later be used to populate the data structure
	 * for the kmean algorithm
	 */
        readPGM(InFileName, height, width, maxValue, imageMat);
	
	N = imageMat.size(); // Total number of data points in the PGM data


	double initCentroids[k]; // Array of PGM centroids

	vector<dataPoint*> data; // Vector for for storing Kmean data points
	vector<dataPoint*> centroids; // Vector for storing Kmean centroids
	vector<unsigned int> assignments(N,0); // Vector for storing assignments.

	vector<unsigned int> PGM(N,0); // Vector for storing PGM data
	
	/***************************************************
	 * Populate KMean data point and centroids vectors with PGM data
	 ***************************************************/
	pt point[N]; // N data points
	pt centroidPoints[k]; // K data points for centroids
	dataPoint* kmDataPoints[N]; // N Kmean data points
	dataPoint* kmCentroidPoints[k]; // N Kmean data points for centroids


	for(unsigned int i = 0; i< N; ++i){
		point[i].x = imageMat[i];
		point[i].y = 0.0;
		point[i].z = 0.0;
		kmDataPoints[i] = new dataPoint;
		kmDataPoints[i]->point = &point[i];
		kmDataPoints[i]->cluster = 0;
		data.push_back(kmDataPoints[i]);
	}

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
        /***************END vector initialization*************/
	
	/************************ K-MEAN ALGORITHM *********************/
	
	KMeans* km = new KMeans(data, centroids); // Instantiate KMeans class
	
	int maxIter = 50; // set maximum iterations
	
	km->cluster(maxIter, assignments); // run clustering algorithm.

	/********************** END K-MEAN ALGORITHM*********************/
	
	/***************************************************************
	 * Convert assignments to PGM data and write to file/stdout
	 * ************************************************************/
	
	assignments2PGM(assignments, centroids, PGM);	
	
	writePGM(PGM, width, height, maxValue, NULL);
	
	/**************************************************************
	 Delete dynamic allocations*/

	for(unsigned int i =0; i<N; ++i){
		delete[] kmDataPoints[i] ;
	}

	for(int i =0; i<k; ++i){
		delete[] kmCentroidPoints[i];
	}

	return 0;
}




