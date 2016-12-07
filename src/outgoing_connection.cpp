/**
	* Outgoing Connection
	*
	* Used to track outgoing connections for a router. Handles the enqueueing and
	* switching of a packet from the router to the wire. Tracks the number of
	* packets that are processed by the router, switched to the wire, and dropped
	* from the router.
	*
	* Created: 30 November 2013
	* Revised:
	*/

#include "outgoing_connection.h"

/**
	* Constructor
	*
	* This initializes an outgoing connection on the router. It sets up the
	* outgoing queue and sets the maximum size of it (based on a global
	* constant), and
	* the service delay time needed to switch packets from the input queue to
	* the output queue.
	*
	* @param double delay the service delay to switch a packet, service_time
	* @param int    loc   the address of this router
	*/
Outgoing_Connection::Outgoing_Connection ( double delay )
{
	this->maximum_queue_size = QUEUE_SIZE ;
	this->packets_dropped = 0 ;
	this->packets_transmitted = 0 ;
	this->bandwidth = delay ;
	//packet_queue = new std::queue<Packet*> ;
}

/**
	* Deconstructor
	*
	* Delete the queue and reset the variables to unusable times in order to
	* free up memory and make this connection invalid.
	*/
Outgoing_Connection::~Outgoing_Connection ( void )
{
	this->maximum_queue_size = -1 ;
	this->packets_dropped = -1 ;
	this->packets_transmitted = -1 ;
	this->bandwidth = -1 ;
	//delete packet_queue ;
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
void Outgoing_Connection::enqueue_packet ( Packet * packet )
{
	if ( packet_queue.size() < maximum_queue_size )
	{
		packet->time_next_step += packet->size / this->bandwidth ;
		this->packet_queue.push( packet ) ;
	}
	else
		Outgoing_Connection::drop_packet ( packet ) ;
}

/**
	* Drop Packet
	*
	* There was no room for the packet on the queue so the package is dropped and
	* lost. The number of dropped packets must be incremented.
	*
	* @param Packet * the packet to be dropped.
	*/
void Outgoing_Connection::drop_packet ( Packet * packet )
{
	++ this->packets_dropped ;
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
Packet * Outgoing_Connection::switch_packet ( void )
{
	if ( this->packet_queue.empty() )
		return NULL ;
	if ( this->packet_queue.front()->time_next_step > global_time )
		return NULL ;
	Packet * p = this->packet_queue.front() ;
	this->packet_queue.pop() ;
	++ this->packets_transmitted ;
	return p ;
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
int Outgoing_Connection::get_dropped_packets ( void )
{
	return this->packets_dropped ;
}
/**
	* Get Packets Transmitted
	*
	* This will return the total number of packets switched across the router and
	* transmitted to the wire. This number alone will define the number of
	* successfully switched and transmitted packets on this router.
	*
	* @return int tally of transmitted packets
	*/
int Outgoing_Connection::get_transmitted_packets ( void )
{
	return this->packets_transmitted ;
}
