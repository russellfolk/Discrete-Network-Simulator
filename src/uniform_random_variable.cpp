


#include "uniform_random_variable.h"

double
standard_uniform_random_variable ()
{
	return rand() / ( ( double ) RAND_MAX ) ;
}

double
nonstandard_uniform_random_variable ( int a , int b )
{
	return a + ( standard_uniform_random_variable() * ( b - a ) ) ;
}