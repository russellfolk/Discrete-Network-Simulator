/**
	* Network Router Definition File
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

#ifndef __ROUTER__
	#define __ROUTER__

	#include <iostream>

	#include <cstdlib> // needed for malloc

	#include "dns_constants.h"

	#include "incoming_connection.h"

	#include "outgoing_connection.h"

	class Router
	{
		public:
			Router ( int , int * , double * ) ;
			~Router ( void ) ;
			void create_packet ( int ) ;
			bool receive_packet ( void ) ;
			bool switch_packet ( void ) ;
			bool transmit_packet ( void ) ;
			double calculate_time_on_wire ( Packet * ) ;
			int get_number_transmitted ( void ) ;
			int get_number_dropped ( void ) ;
			int get_number_received ( void ) ;
			int get_number_created ( void ) ;
			int get_drop_out ( void ) ;
			int get_drop_in ( void ) ;
			double get_total_time_in_transit ( void ) ;
			double get_min_time_in_transit ( void ) ;
			double get_max_time_in_transit ( void ) ;
		private:
			void allocate_connections ( void ) ;
			int created_packets ;
			int router_address ;
			int routing_table[ NUMBER_OF_NODES ] ;
			double total_time_in_transit ;
			double min_time_in_transit ;
			double max_time_in_transit ;
			double propogation_delay[ NUMBER_OF_NODES ] ;
			Outgoing_Connection * out_queue[ NUMBER_OF_NODES ] ;
			Incoming_Connection * in_queue[ NUMBER_OF_NODES ] ;
	} ;

#endif
