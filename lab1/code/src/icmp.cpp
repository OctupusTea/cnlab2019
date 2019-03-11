#include <string>

#include "icmp.hpp"
#include "common.hpp"

using std::string;

namespace traceroute
{
	ICMP::ICMP( const uint16_t &sequence, const string &data, const uint8_t &type ) :
		type( type ), code( 0 ),
		sequence( sequence )
	{
		id = RandomInt16( );
		this -> data = data;

		DoChecksum( );
	}

	void ICMP::DoChecksum( )
	{
		uint32_t sum = 0;

		sum += ( static_cast<uint16_t>( type ) << 8 ) + code;
		sum += id;
		sum += sequence;

		for( auto &i : data ) 
		{
			sum += 
		}
	}
}
