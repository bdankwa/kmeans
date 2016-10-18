/*
 * utils.cpp
 *
 *  Created on : Dec 1, 2013
 *      Author : bdankwa
 *  Description: Utilities for Kmean implementations
 */
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "utils.h"

using namespace std;

/* Read PGM file from a file or standard inputLine,
 * this assumes no comment line in the PGM data.
 * */
void readPGM(char* fileName, unsigned int& height, unsigned int& width,
	unsigned int& max, vector<double> &normImage){
	unsigned int pixel;
	char inputLine[20];
	FILE * fp;
  
	if(fileName == NULL){

		fp = stdin;
	}
	else{	
		fp = fopen(fileName,"r");
	}
	
	if(fp == NULL){
		perror("error reading");
		exit(EXIT_FAILURE);
	}

	fgets(inputLine,20,fp);

	if((inputLine[0] != 'P') ||(inputLine[1] != '2') ){ // Ensure input is PGM format
		perror("Input image is not in Plain PGM format");
		exit(EXIT_FAILURE);
	}
	fscanf(fp,"%u",&width);
	fscanf(fp,"%u",&height);
	fscanf(fp,"%u",&max);

	int i = 0;
	while((fscanf(fp,"%u",&pixel)) != EOF){
	  // Normalize image and save in image vector
	  normImage.push_back(pixel/static_cast<double>(max));
	  ++i;
	}

	fclose(fp);
}

// Write PGM data to a file or standard output
void writePGM(vector<unsigned int>& pgm, unsigned int width,
		unsigned int height, unsigned int max, char* fileName ){

	FILE * fp;

	if(fileName == NULL){ // If no output file, write to stdout
		fp = stdout;
	}
	else{
		fp = fopen(fileName,"w");
		if(fp == NULL){
			perror(fileName);
			exit(EXIT_FAILURE);
		}

	}

	fprintf(fp, "P2\n%u %u\n255\n", width, height);

	for(unsigned int i=0; i<pgm.size(); ++i){
		fprintf(fp, "%d\n", (pgm[i]));
		}

	if(fileName != NULL){
	  fclose(fp);
	}
}

// Returns index of max probability in the probability vector
int findMaxProbIdx(vector<double>& probability){
// find the maximum probability in the probability vector and return its index.
	int index = 0;
	int k = probability.size();
	double n = probability[0];

	for(int i=1; i<k; ++i){
		if(probability[i] > n){
			n = probability[i];
			index = i;
		}
	}
	return index;  
}


