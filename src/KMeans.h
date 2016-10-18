/*
 * KMeans.h
 *
 *  Created on : Nov 21, 2013
 *      Author : bdankwa
 *  Description: A class for implementing the KMeans algorithm.
 */

#ifndef KMEANS_H_
#define KMEANS_H_

#include <vector>
#include "Types.h"

using namespace std;

class KMeans {
public:
	KMeans(const vector<dataPoint*>& data,
		   vector<dataPoint*>& initialCentroids);
	virtual ~KMeans();
	// Method for calculating kmean
	bool cluster(int iterLimit, vector<unsigned int>& assignments); 
	// Method for calculating probability kmean
	void dataDistribution(int iterLimit, int runs, vector<vector<unsigned int> >& distribution); 
	void setCentroids(vector<dataPoint*> currCentroids);
	void getCentroids(vector<dataPoint*>& currCentroids);

private:
	// Method for calculating cluster assignments
	double assignment(vector<unsigned int>& assignments, vector<unsigned int>& clusterSize);
	// Method for calculating data centroids
	void calculateCentroid(vector<unsigned int>& clusterSize);
	// Method for calculating the euclaudian distance between two points
	double calculateDistance(dataPoint* centroid, const dataPoint* data);
	// Method to test if kmean has converged
	bool converged();
	// Method for finding the closest centroid from a given point.
	unsigned int findMinDistance(vector<double> distance, double & minDist);

	vector<dataPoint*> data; // Data structure for kmean
	vector<dataPoint*> centroids; // Current centroids of data
	unsigned int N; // Total number of data points
	unsigned int Nc; // Number of centroids
	bool isConverged;
};

#endif /* KMEANS_H_ */
