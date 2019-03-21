#include "icmp.hpp"
#include "common.hpp"
#include "socket.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

using std::vector;
using std::string;
using std::stringstream;
using std::cin;
using std::cout;
using std::clog;
using std::endl;

#ifdef DEBUG

// #define DEBUG_PACKET

#endif

namespace traceroute
{
	string targetName;
	Ip targetIp;

	extern IcmpSocket icmpSocket;
	uint16_t id;
	vector<string> output( 4, "" );

	void Usage( )
	{
		clog << "Usage: ./traceroute ip | name" << endl;
	}

	bool ParseArg( int argc, char **argv )
	{
		if( argc != 2 )
		{
			return false;
		}

		targetName = argv[ 1 ];
		targetIp = Dns( targetName );

		return true;
	}

	Ip IpFromICMP( const ICMP &icmp )
	{
		uint8_t ip[ 4 ] = { static_cast<uint8_t>( icmp.Data( )[ 4 ] ),
							static_cast<uint8_t>( icmp.Data( )[ 5 ] ),
							static_cast<uint8_t>( icmp.Data( )[ 6 ] ),
							static_cast<uint8_t>( icmp.Data( )[ 7 ] ) };
		return Ip( ip );
	}

	bool RegularPacketHandler( ICMP &icmp, string &ipString,
			string &timeString )
	{
		
	}

	bool TimeoutPacketHandler( ICMP &icmp, string &ipString,
			string &timeString )
	{
		uint16_t temp = static_cast<uint16_t>( icmp.Data( )[ 44 ] ) << 8;
		temp += static_cast<uint16_t>( icmp.Data( )[ 45 ] );
		icmp.Id( temp );

		temp = static_cast<uint16_t>( icmp.Data( )[ 45 ] ) << 8;
		temp += static_cast<uint16_t>( icmp.Data( )[ 47 ] );
		icmp.Sequence( temp );

		return RegularPacketHandler( icmp, ipString, timeString );
	}

	bool SendIcmp( int ttl, int seq, string &ipString, string &timeString )
	{
		ICMP icmp_send( id, seq );
		icmpSocket.Send( icmp_send, ttl );

		ICMP icmp_recv = ICMP( );
		if( icmpSocket.Recv( icmp_recv ) )
		{
			switch( icmp_recv.Type( ) )
			{
				case ICMP_TIME_EXCEEDED:
					return TimeoutPacketHandler( icmp, ipString, timeString );
				case ICMP_ECHO_REPLY:
					return RegularPacketHandler( icmp, ipString, timeString );
				default:
					clog << "Error / Request Type = "
						<< int( icmp_recv.Type( ) ) << endl;
					return false;
			}
		}
		else
		{
			ipString = "0.0.0.0";
			timeString = "*";
			return false;
		}
	}

	int Main( int argc, char **argv )
	{
		if( not ParseArg( argc, argv ) )
		{
			Usage( );
			return 1;
		}

		icmpSocket = IcmpSocket( targetIp );
		id = RandomInt16( );

		for( int ttl = 1; ttl <= 30; ++ttl )
		{
			string ipString, timeString;

			for( int i = 0; i < 3; ++i )
			{
				while( not SendIcmp( ttl, ttl * 3 + i, ipString, timeString ) )
				{
					//	empty
				}

				if( ipString != "0.0.0.0" )
				{
					output[ 0 ] = ipString;
				}

				output[ i + 1 ] = timeString;
			}

			cout << ttl;
			for( int i = 0; i < 4; ++i )
			{
				cout << ' ' << output[ i ];
			}
			cout << endl;
		}
		return 0;
	}
}

int main( int argc, char **argv )
{
	return traceroute::Main( argc, argv );
}
