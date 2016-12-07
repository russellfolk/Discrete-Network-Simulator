


typedef struct
{
	int last_stop ;       // last location visitied
	int next_stop ;       // next location in transit
	double time_on_wire ; // when to arrive at the next location
	Packet * packet ;     // the packet itself
} packet_on_wire ;