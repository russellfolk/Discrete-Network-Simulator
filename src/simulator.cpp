/**
	* Simulator Definition File
	*
	*
	*
	* Created: 8 December 2013
	* Revised:
	*/

#include "simulator.h"

int
main ( int argc , char ** argv )
{
	if ( argc < 3 )
	{
		std::cout << "Invalid input. Please run './simulator <seed> <graph>' where <seed> is an int and <graph> is the filename." << std::endl ;
		exit (-1) ;
	}
	seed = atoi( argv[ 1 ] ) ;
	long num_created = 0 , num_dropped = 0 , num_received = 0 , num_out = 0 , num_transmitted = 0;
	int num_routers_routing = 0 ;
	int mat_i , mat_j ;
	//seed = 17 ;
	srand( seed ) ;
	std::ifstream graphFile ( argv[ 2 ] ) ;

	//check if file opened
	if ( !graphFile.is_open() )
	{
		std::cout << "Unable to open file" ;
		return 0 ;
	}
	std::cout << "Opened file\n" ;

	graphFile >> mat_i >> mat_j ;
	numberOfNodes = mat_i ;

	//fill adjacency matrix with all infinity except for
	//    links to self which are 0
	//fill next matrix will all -1 representing forward directly
	//    to that node
	for ( int k = 0 ; k < numberOfNodes ; ++ k )
	{
		for ( int l = 0 ; l < numberOfNodes ; ++ l )
		{
			if ( k == l )
				links[ k ][ l ] = 0 ;
			else
				links[ k ][ l ] = INFINITY ;
			next[ k ][ l ] = -1 ;
		}
	}
	std::cout << "Filled links and next with empty values\n" ;

	//read from file to update adjacency matrix
	//using non standard random generated values representing the link's delay
	//copy accross diagonal
	while ( graphFile >> mat_i >> mat_j )
	{
		links[ mat_i ][ mat_j ] = links[ mat_j ][ mat_i ] = delay[ mat_i ][ mat_j ] = delay[ mat_j ][ mat_i ] = nonstandard_uniform_random_variable( 1, 10 ) ;
	}
	graphFile.close();

	//std::cout << nonstandard_uniform_random_variable(1,10) << std::endl;

/*
	//output delay matrix
	int linkcounter = 0;
	std::cout << "Initial adacency matrix.\n";
	for ( int k = 0; k < numberOfNodes; k++){
			for ( int l = 0; l < numberOfNodes; l++){
					std::cout << delay[k][l] << " ";
					if (delay[k][l] != 0)
						linkcounter++;
			}
			std::cout << std::endl;
	}
	std::cout << std::endl << linkcounter << std::endl;
*/

	//fill seen with all 0's
	for ( int i = 0 ; i < numberOfNodes ; ++ i )
	{
		seen[ i ] = 0 ;
	}

	//dfs of links.  changes seen[i] to 1 if found
	dfs (0);

	//print out seen[] to see if dfs found a completely connected graph
	for ( int i = 0 ; i < numberOfNodes ; ++i )
	{
		//std::cout << seen[i] << " ";
		if ( seen[ i ] == 0 )
		{
			std::cout << "The graph is not fully connected\n" ;
			return 0 ;
		}
	}
	std::cout << "Graph is completely connected\n\n" ;

	//floyd warshall
	std::cout << "Floyd warshall\n" ;
	for ( int k = 0 ; k < numberOfNodes ; ++ k )
	{
		for ( int i = 0 ; i < numberOfNodes ; ++ i )
		{
			for ( int j = 0 ; j < numberOfNodes ; ++ j )
			{
				if ( links[ i ][ k ] + links[ k ][ j ] < links[ i ][ j ] )
				{
					links[ i ][ j ] = links[ i ][ k ] + links[ k ][ j ] ;
					next[ i ][ j ] = k ;
				}
			}
		}
	}

	/*
	//output floyd warshall distances
	std::cout << "Floyd warshall distances between nodes.\n";
	for ( int k = 0; k < numberOfNodes; k++){
			for ( int l = 0; l < numberOfNodes; l++){
					std::cout << links[k][l] << " ";
			}
			std::cout << std::endl;
	}
	std::cout << std::endl;
	*/

	//Build the routing table (next) and output results
	std::cout << "Build Routing table.\n" ;
	for ( int k = 0 ; k < numberOfNodes ; ++ k )
	{
		for ( int l = 0 ; l < numberOfNodes ; ++ l )
		{
			//std::cout << "path " << k << " " << l << ":";
			Path( k , l , k , l ) ;
			//std::cout << std::endl;
			//std::cout << next[k][l] << " ";
		}
		//std::cout << std::endl;
	}
	//std::cout << std::endl;

	//replace the -1's in the routing table (next) table with the actual next node number
	for ( int i = 0 ; i < numberOfNodes ; ++ i )
	{
		for ( int j = 0 ; j < numberOfNodes ; ++ j )
		{
			if ( next[ i ][ j ] == -1 )
			{
				next[ i ][ j ] = j ;
				//std::cout << "next[" << i << "][" << j << "] " << next[i][j] << " " ;
			}
		}
		//std::cout << std::endl ;
	}
	std::ofstream matrix ( "next" ) ;
	for ( int i = 0 ; i < numberOfNodes ; ++ i )
	{
		for ( int j = 0 ; j < numberOfNodes ; ++ j )
		{
				matrix << next[ i ][ j ] << " " ;
				//std::cout << "next[" << i << "][" << j << "] " << next[i][j] << " " ;
		}
		matrix << std::endl ;
	}
	matrix.close() ;

	//create routers
	Router * router[ NUMBER_OF_NODES ] ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		router[ i ] = new Router( i, next[ i ] , delay[ i ] ) ;
	}

	int tempRouterNumber ;
	int sourceRouter[ NUMBER_OF_NODES ] ;
	std::fill( sourceRouter , sourceRouter + NUMBER_OF_NODES , -1 ) ;
	//build source destination pairs
	for ( int i = 0 ; i < 20 ; ++ i )
	{
		tempRouterNumber = ( ( int ) ( nonstandard_uniform_random_variable( 0 , 149 ) ) ) ;
		if ( sourceRouter[tempRouterNumber] == -1 )
			sourceRouter[ tempRouterNumber ] = ( ( int ) ( nonstandard_uniform_random_variable( 0 , 149 ) ) ) ;
		else
			-- i ;
	}

//	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
//	{
//		if ( sourceRouter[ i ] != -1 )
//		{
//			router[ i ]->create_packet( sourceRouter[ i ] ) ;
//		}
//	}

// MAIN PROGRAM LOOP
	for ( global_time = 0 ; global_time < 1000 ; ++ global_time )
	{
		//std::cout << "Global time: " << global_time << std::endl ;
		for ( int r_id = 0 ; r_id < NUMBER_OF_NODES ; ++ r_id )
		{
			//std::cout << "Router " << r_id << std::endl ;
			// transmit waiting packets
			//std::cout << "Prep transmit" << std::endl ;
			router[ r_id ]->transmit_packet() ;

			if ( sourceRouter[ r_id ] != -1 )
			{
				//std::cout << "Prep lambda" << std::endl ;
				if ( poisson_random_variable( LAMBDA ) >= 0.5 )
				{
					//std::cout<< "create packet for router " << c_id << std::endl ;
					router[ r_id ]->create_packet( sourceRouter[ r_id ] ) ;
				}
			}

			// switch ready packets
			//std::cout << "Prep switch" << std::endl ;
			router[ r_id ]->switch_packet() ;

			// receive new packets
			//std::cout << "Prep receive" << std::endl ;
			router[ r_id ]->receive_packet() ;
			//std::cout << "At Router " << r_id << std::endl ;
		}
	}

	long min_dropped = INFINITY , max_dropped = 0 , total_dropped = 0 ;

	std::ofstream outfile ( "results.txt" ) ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		if ( router[ i ]->get_number_received() > 0 || router[ i ]->get_number_transmitted() > 0 )
			++ num_routers_routing ;
		if ( ( router[ i ]->get_number_received() > 0 || router[ i ]->get_number_transmitted() > 0 ) && router[ i ]->get_number_dropped() < min_dropped )
			min_dropped = router[ i ]->get_number_dropped() ;
		if ( router[ i ]->get_number_dropped() > max_dropped )
			max_dropped = router[ i ]->get_number_dropped() ;
		total_dropped += router[ i ]->get_number_dropped() ;

		num_created += router[ i ]->get_number_created() ;
		num_received += router[ i ]->get_number_received() ;
		num_dropped += router[ i ]->get_number_dropped() ;
		num_transmitted += router[ i ]->get_number_transmitted() ;
		outfile << "Router " << i << ":" <<std::endl << router[ i ]->get_number_created() << " packets created." << std::endl ;
		outfile << router[ i ]->get_number_received() << " packets received." << std::endl ;
		outfile << router[ i ]->get_number_dropped() << " packets dropped." << std::endl ;
		outfile << router[ i ]->get_number_transmitted() << " packets transmitted." << std::endl ;
		outfile << router[ i ]->get_drop_in() << " packets dropped from in-queue." << std::endl ;
		outfile << router[ i ]->get_drop_out() << " packets dropped from out-queue." << std::endl ;
		outfile << std::endl ;
	}
	outfile.close() ;

	num_out = num_created - num_received - num_dropped ;

	std::cout << (((double)num_dropped) / num_created) * 100 << "% packets dropped" << std::endl ;
	std::cout << (((double)num_received) / num_created) * 100 << "% packets received" << std::endl ;
	std::cout << num_out << " packets out" << std::endl ;

	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		if ( sourceRouter[ i ] != -1 )
		{
			std::cout << "Connection (" << i << "," << sourceRouter[ i ] << "):" << std::endl ;
			std::cout << "Generated " << router[ i ]->get_number_created() << " packets." << std::endl ;
			std::cout << router[ sourceRouter[ i ] ]->get_number_received() << " packets arrived." << std::endl ;
			std::cout <<  ((double)router[ sourceRouter[ i ] ]->get_number_received()) / ((double)router[ i ]->get_number_created()) * 100 << "% packets arrived." << std::endl ;
			std::cout << "The mininmum transmission time for a packet is " << router[ sourceRouter[ i ] ]->get_min_time_in_transit() << " seconds." << std::endl ;
			std::cout << "The maximum transmission time for a packet is " << router[ sourceRouter[ i ] ]->get_max_time_in_transit() << " seconds." << std::endl ;
			std::cout << "The average transmission time per packet is " << router[ sourceRouter[ i ] ]->get_total_time_in_transit() / ((double)router[ sourceRouter[ i ] ]->get_number_received()) << " seconds." << std::endl ;
			std::cout << std::endl ;
		}
	}

	std::cout << "Min dropped packets at a router: " << min_dropped << std::endl ;
	std::cout << "Max dropped packets at a router: " << max_dropped << std::endl ;
	std::cout << "Average dropped packets per router: " << ((((double)total_dropped) / num_created ) / num_routers_routing) * 100 << "%" << std::endl ;

	/*
	//output source destination pairs
	for(int i = 0; i < NUMBER_OF_NODES; i++){
			std::cout << i << " -> " << sourceRouter[i] << " \n";
	}
	*/

	/*
	//practice with priority queue of structs
	thing a;
	thing b;
	thing c;

	a.a = 1;
	b.a = 3;
	c.a = 2;

	wirePQ[0].push(a);
	wirePQ[1].push(b);
	wirePQ[0].push(c);
	int output = wirePQ[0].top().a;
	std::cout << std::endl << std::endl << output;
	output = wirePQ[1].top().a;
	std::cout << std::endl << std::endl << output;
	*/
	return 0 ;
}

void
Path ( int i , int j , int iFinal , int jFinal )
{
	++ counter ;
	int intermediate = next[ i ][ j ] ;
	if ( intermediate != -1 )
	{
		Path( intermediate , j , iFinal , jFinal ) ;
		//std::cout << intermediate << "(" << counter << ")";
		next[ iFinal ][ jFinal ] = intermediate ;
		Path( i , intermediate , iFinal , jFinal ) ;
	}
}

void
dfs ( int u )
{
	//std::cout << u << std::endl;
	seen[u] = true;
	for ( int v = 0 ; v < numberOfNodes ; ++ v )
	{
		if( !seen[ v ] && links[ u ][ v ] != INFINITY && links[ u ][ v ] != 0 )
			dfs ( v ) ;
	}
}
