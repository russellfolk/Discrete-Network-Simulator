
#include "poisson_random_variable.h"

double
poisson_random_variable ( double lambda )
{
	return ( -1.0f / lambda ) * log ( standard_uniform_random_variable() ) ;
}