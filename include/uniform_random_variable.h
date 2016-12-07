/**
	*	Uniform Random Variable
	*
	*	Provides values for a uniform distribution on either the standardard range
	*	of 0 to 1 or a given range of alpha to beta.
	*
	*	Created: 18 November 2013
	*	Revised:
	*/

#ifndef __UNIFORM_RANDOM_VARIABLE__
	#define __UNIFORM_RANDOM_VARIABLE__

	#include <cstdlib>

	/**
		*	Standard Uniform Random Variable
		*	Returns a variable x that is generated using the standard uniform
		*	distribution with a range [0,1]
		*
		*	@return variable x
		*/
	double standard_uniform_random_variable ( void ) ;

	/**
		*	Nonstandard Uniform Random Variable
		*	Returns a variable x that is generated using uniform distribution with a
		*	range [alpha,beta]. Requires the standard uniform distribution.
		*
		*	@param a value of alpha
		*	@param b value of beta
		*	@return variable x
		*/
	double nonstandard_uniform_random_variable ( int a , int b ) ;

#endif
