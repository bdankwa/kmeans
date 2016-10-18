/*
 * utils.h
 *
 *  Created on: Dec 1, 2013
 *      Author: bdankwa
 * Description: Utilities for Kmean implementations
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>

using namespace std;

// Read PGM file from a file or standard input
void readPGM(char* fileName, unsigned int& height, unsigned int& width,
	unsigned int& max, vector<double> &normImage);

// Write PGM file to a file or standard output
void writePGM(vector<unsigned int>& pgm, unsigned int width,
		unsigned int height, unsigned int max, char* fileName );

// Returns index of max probability in the probability vector
int findMaxProbIdx(vector<double>& probability);


#endif /* UTILS_H_ */
