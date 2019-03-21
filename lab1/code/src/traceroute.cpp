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
using std::cerr;
using std::clog;
using std::endl;

#ifdef DEBUG

// #define DEBUG_PACKET

#endif

namespace traceroute
{
	enum Icmp_Status{ ICMP_AGAIN = -1, ICMP_SUCC = 0, ICMP_FINAL = 1 };

	string targetName;
	Ip targetIp;

	IcmpSocket icmpSocket;
	uint16_t id;
	uint16_t seq;
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

	Icmp_Status RegularPacketHandler( ICMP &icmp, string &ipString,
			string &timeString, system_clock::time_point timeSent )
	{
#ifdef DEBUG
		clog << "origin: ( " << id << " , " << seq << " )" << endl;
		clog << "icmp: ( " << icmp.Id( ) << " , " << icmp.Sequence( ) << " )" << endl;
#endif

		if( icmp.Id( ) != id or icmp.Sequence( ) != seq )
		{
			ipString = "255.255.255.255";
			timeString = "NaN";
			return ICMP_AGAIN;
		}

		ipString = to_string( IpFromICMP( icmp ) );

		auto rtt = duration_cast<microseconds>
			( system_clock::now( ) - timeSent );
		stringstream temp;
		temp << std::setprecision( 3 ) << double( rtt.count( ) ) / 1000.0;
		timeString = temp.str( );

		return ICMP_SUCC;
	}

	Icmp_Status TimeoutPacketHandler( ICMP &icmp, string &ipString,
			string &timeString, system_clock::time_point timeSent )
	{
#ifdef DEBUG_PACKET
		cout << "data============";
		cout << icmp.Data( ).substr( 0, 128 );
		cout << "===============" << endl;
#endif
		uint32_t temp = static_cast<uint32_t>( icmp.Data( )[ 44 ] );
		temp <<= 8;
		temp += static_cast<uint32_t>( icmp.Data( )[ 45 ] );
		icmp.Id( temp );

		temp = static_cast<uint32_t>( icmp.Data( )[ 46 ] );
		temp <<= 8;
		temp += static_cast<uint32_t>( icmp.Data( )[ 47 ] );
		icmp.Sequence( temp );

		return RegularPacketHandler( icmp, ipString, timeString, timeSent );
	}

	Icmp_Status SendIcmp( int ttl, string &ipString, string &timeString )
	{
		system_clock::time_point start = system_clock::now( );

		ICMP icmp_send( id, seq );
		icmpSocket.Send( icmp_send, ttl );

		ICMP icmp_recv = ICMP( );
		if( icmpSocket.Recv( icmp_recv ) )
		{
			switch( icmp_recv.Type( ) )
			{
				case ICMP_TIME_EXCEEDED:
					return TimeoutPacketHandler( icmp_recv, ipString,
								timeString, start );

				case ICMP_ECHO_REPLY:
					if( RegularPacketHandler ( icmp_recv, ipString,
								timeString, start ) == ICMP_SUCC )
					{
						return ICMP_FINAL;
					}
					else
					{
						return ICMP_AGAIN;
					}

				default:
					clog << "Error / Request Type = "
						<< int( icmp_recv.Type( ) ) << endl;
					return ICMP_SUCC;
			}
		}
		else
		{
			ipString = "0.0.0.0";
			timeString = "*";
			return ICMP_SUCC;
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
			Icmp_Status stat = ICMP_AGAIN;

			for( int i = 0; i < 3; ++i )
			{
#ifdef DEBUG
				clog << "Hop # " << ttl << " , i = " << i << endl;
#endif
				seq = ttl * 3 + i;

				while( stat == ICMP_AGAIN )
				{
					stat = SendIcmp( ttl, ipString, timeString );
#ifdef DEBUG
					clog << "stat = " << stat << endl;
#endif
				}

				if( ipString != "0.0.0.0" and ipString != "255.255.255.255" )
				{
					output[ 0 ] = ipString;
				}

				output[ i + 1 ] = timeString + " ms";

				if( stat == ICMP_FINAL )
				{
					break;
				}
			}

			cout << ttl;
			for( int i = 0; i < 4; ++i )
			{
				cout << ' ' << output[ i ];
			}
			cout << endl;

			if( stat == ICMP_FINAL )
			{
				break;
			}
		}
		return 0;
	}
}

int main( int argc, char **argv )
{
	return traceroute::Main( argc, argv );
}
