/**
	* Fixed constants used throughout the Discrete Network Simulator.
	*
	*	Created: 18 November 2013
	*	Revised: 29 November 2013
	*/

#ifndef __PROGRAM_CONSTANTS__
	#define __PROGRAM_CONSTANTS__

	#include <queue>

	#include "packet.h"

	const int NUMBER_OF_NODES = 150 ;
	const int QUEUE_SIZE = 30 ;
	const int SECONDS_TO_RUN = 1000 ;
	const double EULER = 2.71828182845904523536f ;
	const double LAMBDA = 0.5f ;

	extern int global_time ; // used to store the current runtime of the program
	                         // see main for loop of the program for use.

	extern std::priority_queue<Packet *> WIRE[ NUMBER_OF_NODES ] ;

#endif
