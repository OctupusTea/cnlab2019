#include "icmp.hpp"
#include "common.hpp"

#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

namespace traceroute
{
	int Main( int argc, char **argv )
	{
		for( int i = 0; i < argc; ++i )
		{
			cout << argv[ i ] << endl;
		}

		return 0;
	}
}

int main( int argc, char **argv )
{
	return traceroute::Main( argc, argv );
}
