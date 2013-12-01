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

	class Outgoing_Connection
	{
		public:
			Outgoing_Connection ( double , int , int ) ;
			~Outgoing_Connection ( void ) ;
			void enqueue_packet ( Packet * ) ;
			void drop_packet ( Packet * ) ;
			Packet * switch_packet ( void ) ;
			int get_dropped_packets ( void ) ;
			int get_transmitted_packets ( void ) ;
		private:
			int maximum_queue_size ;
			int packets_dropped ;
			int packets_transmitted ;
			double propogation_delay ;
			std::queue<Packet*> queue ;
	} ;

#endif
