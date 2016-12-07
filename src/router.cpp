/**
	* Network Router
	*
	* Serves as an individual node on the network. A router is able to generate
	* packages, forward packages, and receive packages. A router will store a
	* routing table that holds the location of where to go next for each packet. A
	* router will receive a packet_on_wire and unpack the packet and send it to
	* its Incoming_Connection for queueing and processing. Next it will take the
	* packet and process it to the appropriate Outgoing_Connection queue for
	* shipping on its way. Before sending the packet back onto the wire the packet
	* will be prepped by calculating the time_on_wire by multiplying the bandwidth
	* and the size of the packet then wrapped in a new packet_on_wire then placed
	* on the wire.
	*
	* Created: 30 November 2013
	* Revised:
	*/

#include "router.h"

/**
	* Constructor
	*
	* This will initialize a router with a routing table that provides the next
	* destination in the shortest path to any point in the network. This will also
	* create an incoming and outgoing queue for all connections this router can
	* have.
	*
	* @param address the network address of the router
	*	@param table   int array that contains the location of the next stop for
	*                destination table[i]
	*/
Router::Router ( int address , int * table , double * delay )
{
	int i ;
	this->created_packets = 0 ;
	this->min_time_in_transit = INFINITY ;
	this->max_time_in_transit = 0 ;
	this->total_time_in_transit = 0 ;
	this->router_address = address ;
	//routing_table = table ;
	//propogation_delay = delay ;
	//in_queue = ( Incoming_Connection * ) malloc (
	//             sizeof ( Incoming_Connection * ) * NUMBER_OF_NODES ) ;
	//out_queue = ( Outgoing_Connection * ) malloc (
	//              sizeof ( Outgoing_Connection * ) * NUMBER_OF_NODES ) ;
	for ( i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		this->routing_table[ i ] = table[ i ] ;
		this->propogation_delay[ i ] = delay[ i ] ;
		this->in_queue[ i ] = new Incoming_Connection( poisson_random_variable( 1.0f ) , router_address ) ;
		this->out_queue[ i ] = new Outgoing_Connection( standard_uniform_random_variable() ) ;	// initialize with prop delay
	}
}

/**
	* Create a packet
	*
	* The router will receive the command to create a packet and send it on its
	* path to a given destination. It will generate a random size and provide the
	* next stop, time next step, and last stop.
	*
	* @param dest end destination of the packet
	*/
void
Router::create_packet ( int dest )
{
	++ this->created_packets ;
	Packet * p = new Packet ;
	p->destination = dest ;
	p->size = standard_uniform_random_variable() ;
	p->time_next_step = global_time ;
	p->last_stop = this->router_address ;
	p->next_stop = this->routing_table[ p->destination ] ;
	//std::cout << "Packet: going to " << p->destination << " size " << p->size << " time " << p->time_next_step << " last " << p->last_stop << " next " << p->next_stop << " q " << this->routing_table[ p->destination ] << std::endl ;
	this->out_queue[ this->routing_table[ p->destination ] ]->enqueue_packet( p ) ;
}

/**
	* Receive a packet
	*
	* The router will check the wire for any packet that is arriving and receive
	* it into the appropriate incoming connection.
	*
	* @return true if packets were received, else false
	*/
bool
Router::receive_packet ( void )
{
	bool change = false ;
	//std::cout << "This router is " << this->router_address << std::endl ;
	while ( !WIRE[ this->router_address ].empty() && WIRE[ this->router_address ].top()->time_next_step <= global_time )
	{
		Packet * p = WIRE[ this->router_address ].top() ;
		WIRE[ this->router_address ].pop() ;
		double temp = this->in_queue[ p->last_stop ]->receive_packet( p ) ;
		if ( temp != -1 )
		{
			total_time_in_transit += temp ;
			if ( temp > max_time_in_transit )
				max_time_in_transit = temp ;
			if ( temp < min_time_in_transit )
				min_time_in_transit = temp ;
		}
		//delete p ;
		change = true ;
	}
	return change ;
}

/**
	* Switch Packet
	*
	* For each connection, switch any processed packets from the incoming queue to
	* the outgoing queue so that they may be transmitted to the next node in the
	* network on the way to its destination.
	*
	* @return true if packets were switched, else false
	*/
bool
Router::switch_packet ( void )
{
	bool change = false ;
	int i ;
	for ( i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		Packet * p ;
		while ( ( p = this->in_queue[ i ]->switch_packet() ) != NULL )
		{
			//std::cout << "switching packet" << std::endl ;
			p->last_stop = this->router_address ;
			p->next_stop = this->routing_table[ p->destination ] ;
			this->out_queue[ this->routing_table[ p->destination ] ]->enqueue_packet( p ) ;
			change = true ;
		}
	}
	return change ;
}

/**
	* Transmit Packet
	*
	*	For each connection, transmit each ready packet to the wire.
	*
	* @return true if packets were transmitted, else false
	*/
bool
Router::transmit_packet ( void )
{
	bool change = false ;
	int i ;
	for ( i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		Packet * p ;
		while ( ( p = this->out_queue[ i ]->switch_packet() ) != NULL )
		{
			p->time_next_step = this->propogation_delay[ p->destination ] ;
			WIRE[ this->routing_table[ p->destination ] ].push( p ) ;
			change = true ;
		}
	}
	return change ;
}

double
Router::calculate_time_on_wire ( Packet * )
{
	return 0.0f ;
}

int
Router::get_number_transmitted ( void )
{
	long transmitted_packets = 0 ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
		transmitted_packets += this->out_queue[ i ]->get_transmitted_packets() ;
	return transmitted_packets ;
}

int
Router::get_number_dropped ( void )
{
	long dropped_packets = 0 ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
	{
		dropped_packets += this->in_queue[ i ]->get_dropped_packets() ;
		dropped_packets += this->out_queue[ i ]->get_dropped_packets() ;
	}
	return dropped_packets ;
}

int
Router::get_number_received ( void )
{
	int received_packets = 0 ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
		received_packets += this->in_queue[ i ]->get_received_packets() ;
	return received_packets ;
}

int
Router::get_number_created ( void )
{
	return this->created_packets ;
}

int
Router::get_drop_in ( void )
{
	int dropped_packets = 0 ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
		dropped_packets += this->in_queue[ i ]->get_dropped_packets() ;
	return dropped_packets ;
}

int
Router::get_drop_out ( void )
{
	int dropped_packets = 0 ;
	for ( int i = 0 ; i < NUMBER_OF_NODES ; ++ i )
		dropped_packets += this->out_queue[ i ]->get_dropped_packets() ;
	return dropped_packets ;
}

double
Router::get_total_time_in_transit ( void )
{
	return this->total_time_in_transit ;
}

double
Router::get_min_time_in_transit ( void )
{
	return this->min_time_in_transit ;
}

double
Router::get_max_time_in_transit ( void )
{
	return this->max_time_in_transit ;
}