#include <string>

#include "icmp.hpp"
#include "common.hpp"

using std::string;

namespace traceroute
{
	ICMP::ICMP( const uint16_t &sequence, const string &data,
			const uint8_t &type ) : type( type ), code( 0 ),
			sequence( sequence )
	{
		id = RandomInt16( );
		this -> data = data;

		DoChecksum( );
	}

	ICMP::ICMP( const string &content )
	{
		size_t pos = 0;

		pos = StringGet( content, type, pos );
		pos = StringGet( content, code, pos );
		pos = StringGet( content, checksum, pos );
		pos = StringGet( content, id, pos );
		pos = StringGet( content, sequence, pos );

		data = content.substr( pos );
	}

	void ICMP::DoChecksum( )
	{
		uint32_t sum = 0;

		sum += ( static_cast<uint16_t>( type ) << 8 ) + code;
		sum += id;
		sum += sequence;

		for( size_t i = 0; i < data.length( ); i += 2 ) 
		{
			sum += static_cast<uint16_t>( data[ i ] ) << 8;
			sum += data[ i + 1 ];
		}

		if( data.length( ) % 2 == 1 )
		{
			sum += data.back( );
		}

		checksum = ~( ( sum & 0xFFFF ) + ( ( sum & 0xFFFF0000 ) >> 16 ) );
	}

	const string ICMP::Content( ) const
	{
		string content = "";

		StringAppend( content, type );
		StringAppend( content, code );
		StringAppend( content, checksum );
		StringAppend( content, id );
		StringAppend( content, sequence );
		content += data;

		return content;
	}
}
