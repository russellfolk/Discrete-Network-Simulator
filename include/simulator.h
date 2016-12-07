/**
	* Simulator Definition File
	*
	*
	*
	* Created: 8 December 2013
	* Revised:
	*/

	#include <iostream>
	#include <fstream>
#include <string>
#include <cmath>
	#include <queue>

#include "router.h"
#include "poisson_random_variable.h"
#include "uniform_random_variable.h"

void
Path ( int , int , int , int ) ;

void
dfs  ( int ) ;

int numberOfNodes ;
double links[ NUMBER_OF_NODES ][ NUMBER_OF_NODES ] ;  //used for floyd warshal
double delay[ NUMBER_OF_NODES ][ NUMBER_OF_NODES ] ;  //holds link delays
int next[ NUMBER_OF_NODES ][ NUMBER_OF_NODES ] ;      //eventually holds routing table

bool seen [ NUMBER_OF_NODES ] ;
int seed ;
int counter = 0 ;

int global_time ;
std::priority_queue<Packet *> WIRE[ NUMBER_OF_NODES ] ;