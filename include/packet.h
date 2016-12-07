

#ifndef __PACKET__
	#define __PACKET__

	typedef struct P
	{
		int destination ;
		double size ;
		double time_next_step ;
		int last_stop ;       // last location visitied
		int next_stop ;       // next location in transit

		bool operator < ( const P &o ) const
		{
			return time_next_step > o.time_next_step;
		}

	} Packet ;

#endif
