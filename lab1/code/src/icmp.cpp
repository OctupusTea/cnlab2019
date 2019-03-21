#include <iostream>
#include <string>

#include "icmp.hpp"
#include "common.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

using std::cout;
using std::clog;
using std::endl;

using std::ostream;
using std::string;

#ifdef DEBUG

// #define DEBUG_PACKET

#endif

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

	ICMP::ICMP( const string &buf )
	{
// 		size_t pos = 0;
// 
// 		pos = StringGet( content, type, pos );
// 		pos = StringGet( content, code, pos );
// 		pos = StringGet( content, checksum, pos );
// 		pos = StringGet( content, id, pos );
// 		pos = StringGet( content, sequence, pos );
// 
// 		data = content.substr( pos );
		struct iphdr *iphdrptr = (struct iphdr*)buf.data();
		struct icmphdr *icmphdrptr =
			(struct icmphdr*)(buf.data() + (iphdrptr->ihl) * 4);
		
		type = icmphdrptr -> type;
		code = icmphdrptr -> code;
		checksum = icmphdrptr -> checksum;
		id = icmphdrptr -> un.echo.id;
		sequence = icmphdrptr -> un.echo.sequence;
		data = buf.substr( 8 );

#ifdef DEBUG_PACKET
		cout << "data============";
		cout << data.substr( 0, 256 );
#endif
	}

	void ICMP::DoChecksum( )
	{
		checksum = ComputeChecksum( );
	}

	uint16_t ICMP::ComputeChecksum( )
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

		return ~( ( sum & 0xFFFF ) + ( ( sum & 0xFFFF0000 ) >> 16 ) );
	}

	const string ICMP::Content( ) const
	{
		string content = "";

		StringAppend( content, type );
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif
		StringAppend( content, code );
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif
		StringAppend( content, checksum );
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif
		StringAppend( content, id );
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif
		StringAppend( content, sequence );
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif
		content += data;
#ifdef DEBUG_PACKET
		clog << content.length( ) << endl;
#endif

		return content;
	}

	ostream& operator<<( ostream &output, const ICMP &icmp )
	{
		output << "( " << int(icmp.type) << " , " << int(icmp.code) << " , "
			<< icmp.checksum << " , " << icmp.id << " , " << icmp.sequence
			<< " , " << icmp.data << " )";

		return output;
	}
}
