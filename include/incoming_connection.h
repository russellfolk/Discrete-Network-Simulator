/**
	* Incoming Connection Definition File
	*
	* Used to track incoming connections for a router. Handles the enqueueing and
	* switching for a packet. Tracks the number of packets that are received,
	* switched to the outgoing queue, and dropped from the router.
	*
	* Created: 29 November 2013
	* Revised:
	*/

#ifndef __CONNECTION__
	#define __CONNECTION__

	#ifndef __QUEUE__
		#include <queue>
	#endif

	#ifndef __CONSTANTS__
		#define __CONSTANTS__
		#include "dns_constants.h"
	#endif

	#ifndef __PACKET__
		#define __PACKET__
		#include "packet.h"
	#endif

	class Incoming_Connection
	{
		public:
			Incoming_Connection ( double , int , int ) ;
			~Incoming_Connection ( void ) ;
			double receive_packet ( Packet * ) ;
			void enqueue_packet ( Packet * ) ;
			void drop_packet ( Packet * ) ;
			Packet * switch_packet ( void ) ;
			int get_dropped_packets ( void ) ;
			int get_received_packets ( void ) ;
			int get_switched_packets ( void ) ;
		private:
			int maximum_queue_size ;
			int packets_dropped ;
			int packets_received ;
			int packets_switched ;
			int this_location ;
			double service_time ;
			std::queue<Packet*> queue ;
	} ;

#endif
