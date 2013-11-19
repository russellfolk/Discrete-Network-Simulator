/**
	*	Poisson Distribution
	*
	*	Provides functionality for generating a random number using the Poisson
	*	Distribution
	*
	*	Created: 18 November 2013
	*	Revised:
	*/

#ifndef __POISSON_RANDOM_VARIABLE__
	#define __POISSON_RANDOM_VARIABLE__

	#include "uniform_random_variable.h"

	#ifndef __CSTDLIB__
		#define __CSTDLIB__
		#include <cstdlib>
	#endif

	#ifndef __CMATH__
		#define __CMATH__
		#include <cmath>
	#endif

	const double EULER = 2.71828182845904523536f ;
	const double LAMBDA = 0.5f ;

	/**
		*	Poisson Distribution Random Variable
		*	Returns a variable x that is generated using the Poisson Distribution with
		*	a set variable lambda that defines the exponential random variable.
		*
		* @return variable x
		*/
	double poisson_random_variable ()
	{
		return ( -1.0f / LAMBDA ) * log ( standard_uniform_random_variable() ) ;
	}

#endif
