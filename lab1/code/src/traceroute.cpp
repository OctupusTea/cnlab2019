#include "icmp.hpp"
#include "common.hpp"
#include "socket.hpp"

#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;

using std::string;
using std::cin;
using std::cout;
using std::clog;
using std::endl;

namespace traceroute
{
	string targetName;
	Ip targetIp;

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

	int Main( int argc, char **argv )
	{
		if( not ParseArg( argc, argv ) )
		{
			Usage( );
			return 1;
		}

		IcmpSocket icmpSocket( targetIp );

		ICMP icmp( 1 );
		cout << icmp << endl;
		icmpSocket.Send( icmp, 1 );

		if( icmpSocket.Recv( icmp ) )
		{
			cout << icmp << endl;
		}
		else
		{
			cout << "icmp failed." << endl;
		}

		return 0;
	}
}

int main( int argc, char **argv )
{
	return traceroute::Main( argc, argv );
}
