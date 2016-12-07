/**
	*	Poisson Distribution
	*
	*	Provides functionality for generating a random number using the Poisson
	*	Distribution
	*
	*	Created: 18 November 2013
	*	Revised: 29 November 2013
	*/

#ifndef __POISSON_RANDOM_VARIABLE__
	#define __POISSON_RANDOM_VARIABLE__

	#include <cstdlib>
	#include <cmath>

	#include "uniform_random_variable.h"

	/**
		*	Poisson Distribution Random Variable
		*	Returns a variable x that is generated using the Poisson Distribution with
		*	a set variable lambda that defines the exponential random variable.
		*
		* @return double variable x
		*/
	double poisson_random_variable ( double ) ;

#endif
