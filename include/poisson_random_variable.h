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

	#ifndef __UNIFORM_RANDOM__
		#define __UNIFORM_RANDOM__
		#include "uniform_random_variable.h"
	#endif

	#ifndef __CONSTANTS__
		#define __CONSTANTS__
		#include "dns_constants.h"
	#endif

	#ifndef __CSTDLIB__
		#define __CSTDLIB__
		#include <cstdlib>
	#endif

	#ifndef __CMATH__
		#define __CMATH__
		#include <cmath>
	#endif


	/**
		*	Poisson Distribution Random Variable
		*	Returns a variable x that is generated using the Poisson Distribution with
		*	a set variable lambda that defines the exponential random variable.
		*
		* @return double variable x
		*/
	double poisson_random_variable ()
	{
		return ( -1.0f / LAMBDA ) * poisson_random_variable() ;
	}

	/**
		*	Poisson Distribution Random Variable
		*	Returns a variable x that is generated using the Poisson Distribution with
		*	a set variable lambda of 1.0 that defines the exponential random variable.
		*
		* @return double variable x
		*/
	double poisson_random_variable_1 ()
	{
		return log ( standard_uniform_random_variable() ) ;
	}


#endif
