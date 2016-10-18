/*
 * Types.h
 *
 *  Created on : Nov 22, 2013
 *      Author : bdankwa
 *  Description: Data structure for KMeans algorithm.
 */

#ifndef TYPES_H_
#define TYPES_H_

	struct pt {
		double x;
		double y;
		double z;
	}; // A structure representing the coordinates of a point in 3D space.

	struct dataPoint{
		pt * point; // Pointer to a point
		unsigned int cluster; // cluster which the point belongs to
	}; // A data structure for a point in the KMean algorithm.

#endif /* TYPES_H_ */
