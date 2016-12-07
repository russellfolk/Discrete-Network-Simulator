/**
	* Outgoing Connection Definition File
	*
	* Used to track outgoing connections for a router. Handles the enqueueing and
	* switching of a packet from the router to the wire. Tracks the number of
	* packets that are processed by the router, switched to the wire, and dropped
	* from the router.
	*
	* Created: 30 November 2013
	* Revised:
	*/

#ifndef __OUT_CONNECTION__
	#define __OUT_CONNECTION__

	#include <cstddef>
	#include <queue>

	#include "dns_constants.h"
	#include "packet.h"

	class Outgoing_Connection
	{
		public:
			Outgoing_Connection ( double ) ;
			~Outgoing_Connection ( void ) ;
			void enqueue_packet ( Packet * ) ;
			void drop_packet ( Packet * ) ;
			Packet * switch_packet ( void ) ;
			int get_dropped_packets ( void ) ;
			int get_transmitted_packets ( void ) ;
		private:
			unsigned int maximum_queue_size ;
			int packets_dropped ;
			int packets_transmitted ;
			double bandwidth ;
			std::queue<Packet*> packet_queue ;
	} ;

#endif
