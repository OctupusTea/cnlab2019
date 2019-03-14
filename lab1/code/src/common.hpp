#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>
#include <random>
#include <functional>

#include <cstdint>

extern "C"
{

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

}

using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;

using std::string;

namespace traceroute
{
	union Ip
	{
		uint32_t numFormat;
		uint8_t dotFormat[ 4 ];

		Ip( const uint32_t ip )
		{
			numFormat = ip;
		}

		Ip( const uint8_t ip[ 4 ] )
		{
			for( int i = 0; i < 4; ++i )
			{
				dotFormat[ i ] = ip[ i ];
			}
		}

		void SetDotFormat( const uint8_t ip[ 4 ] )
		{
			for( int i = 0; i < 4; ++i )
			{
				dotFormat[ i ] = ip[ i ];
			}
		}
	};

	random_device randomDevice;
	default_random_engine randomEngine
		= default_random_engine( randomDevice( ) );
	uniform_int_distribution<uint16_t> randomDistro(0,0xFFFF);

	auto RandomInt16 = std::bind( randomDistro, randomEngine );

	void StringAppend( string &data, const uint8_t &value );
	void StringAppend( string &data, const uint16_t &value );
	void StringAppend( string &data, const uint32_t &value );

	size_t StringGet( const string &data, uint8_t &value,
			const size_t &pos = 0 );
	size_t StringGet( const string &data, uint16_t &value,
			const size_t &pos = 0 );
	size_t StringGet( const string &data, uint32_t &value,
			const size_t &pos = 0 );

	const Ip DnsIp( const string &hostName );
	const string DnsName( const Ip &hostIp );
}

#endif
