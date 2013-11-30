/**
	* Incoming Connection
	*
	* Used to track incoming connections for a router. Handles the enqueueing and
	* switching for a packet. Tracks the number of packets that are received,
	* switched to the outgoing queue, and dropped from the router.
	*
	* Created: 29 November 2013
	* Revised:
	*/

#include "incoming_connection.h"

/**
	* Constructor
	*
	* This initializes an incomming connection on the router. It sets up the
	* incoming queue and sets the maximum size of it (based on a global
	* constant), the location that this connection receives its data from, and
	* the service delay time needed to switch packets from the input queue to
	* the output queue.
	*
	* @param double delay the service delay to switch a packet, service_time
	* @param int    loc   the address of this router
	*/
Incoming_Connection::Incoming_Connection ( double delay , int from , int loc )
{
	maximum_queue_size = QUEUE_SIZE ;
	packets_dropped = 0 ;
	packets_received = 0 ;
	packets_switched = 0 ;
	service_time = delay ;
	this_location = loc ;
	queue = new std::queue<Packet*> ;
}

/**
	* Deconstructor
	*
	* Delete the queue and reset the variables to unusable times in order to
	* free up memory and make this connection invalid.
	*/
Incoming_Connection::~Incoming_Connection ( void )
{
	maximum_queue_size = -1 ;
	packets_dropped = -1 ;
	packets_received = -1 ;
	packets_switched = -1 ;
	propogation_delay = -1 ;
	delete queue ;
}

/**
	* Receive Packet
	*
	*	Receive a packet from the wire. Check to see if it has arrived at its
	* destination and if so receive it, else send to enqueue for further
	* processing.
	*
	* @param  Packet * a pointer to the packet being transmitted.
	* @return double   the total time in transit
	*/
double Incoming_Connection::receive_packet ( Packet * packet )
{
	if ( packet->destination == this_location )
	{
		++ packets_received ;
		double time_in_transit = packet->time_next_step ;
		delete packet ; // The packet arrived safely and memory must be freed.
		return time_in_transit ;
	}
	else
	{
		Incoming_Connection::enqueue_packet ( packet ) ;
		return -1 ;
	}
}

/**
	*	Enqueue Packet
	*
	* Check to see if there is room on the queue to receive the packet else drop
	* it. If it can be enqueued, calculate the time to switch and store in the
	* packet's time until next step variable.
	*
	* @param Packet * a pointer to the packet being enqueued.
	*/
void Incoming_Connection::enqueue_packet ( Packet * packet )
{
	if ( queue.size() < maximum_queue_size )
	{
		packet->time_next_step += service_time ;
		queue.push( packet ) ;
	}
	else
		Incoming_Connection::drop_packet ( packet ) ;
}

/**
	* Drop Packet
	*
	* There was no room for the packet on the queue so the package is dropped and
	* lost. The number of dropped packets must be incremented.
	*
	* @param Packet * the packet to be dropped.
	*/
void Incoming_Connection::drop_packet ( Packet * packet )
{
	++ packets_dropped ;
	delete packet ;
}

/**
	* Switch Packet
	*
	* Check to see if the leading packet in the queue is ready to switch. If so,
	* it is returned else nothing it returns NULL.
	*
	* @return Packet * a pointer to the packet to be switched to the out queue
	*/
Packet * Incoming_Connection::switch_packet ( void )
{
	if ( queue.empty() )
		return NULL ;
	if ( queue.front()->time_next_step < global_time )
		return NULL ;
	++ packets_switched ;
	return queue.pop() ;
}

/**
	* Get Packets Dropped
	*
	* This will return the total number of packets dropped from the router's
	* incoming connection queue. This number will need to be combined with the
	* outgoing connection queue to get the total number of dropped packets.
	*
	* @return int tally of dropped packets
	*/
int Incoming_Connection::get_dropped_packets ( void )
{
	return packets_dropped ;
}

/**
	* Get Packets Received
	*
	* This will return the total number of packets received by the router's
	* incoming connection queue. This number represents the total number of
	* packets successfully transmitted to this router.
	*
	* @return int tally of received packets
	*/
int Incoming_Connection::get_received_packets ( void )
{
	return packets_received ;
}

/**
	* Get Packets Switched
	*
	* This will return the total number of packets switched from the router's
	* incoming connection queue to the router's outgoing connection queue. This
	* number alone will not represent the number of transmitted packets but can be
	* used to track when a packet is being dropped.
	*
	* @return int tally of switched packets
	*/
int Incoming_Connection::get_switched_packets ( void )
{
	return packets_switched ;
}
