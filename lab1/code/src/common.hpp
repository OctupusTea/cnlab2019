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
using std::to_string;

namespace traceroute
{
	union Ip
	{
		uint32_t numFormat;
		uint8_t dotFormat[ 4 ];

		Ip( const uint32_t ip = 0 )
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

		Ip( const string &ipString )
		{
			size_t pos = 0;

			for( int i = 0; i < 4; ++i )
			{
				size_t next;
				dotFormat[ i ] = stoi( ipString.substr( pos ), &next );
				pos += next + 1;
			}
		}

		void SetDotFormat( const uint8_t ip[ 4 ] )
		{
			for( int i = 0; i < 4; ++i )
			{
				dotFormat[ i ] = ip[ i ];
			}
		}

		const uint8_t& operator[] ( int i ) const
		{
			return dotFormat[ i ];
		}

		friend string to_string( const Ip &ip )
		{
			return to_string( ip[ 0 ] ) + ' ' + to_string( ip[ 1 ] ) + ' ' +
				to_string( ip[ 2 ] ) + ' ' + to_string( ip[ 3 ] );
		}
	};

	extern random_device randomDevice;
	extern default_random_engine randomEngine;
	extern uniform_int_distribution<uint16_t> randomDistro;

	uint16_t RandomInt16( );

	void StringAppend( string &data, const uint8_t &value );
	void StringAppend( string &data, const uint16_t &value );
	void StringAppend( string &data, const uint32_t &value );

	size_t StringGet( const string &data, uint8_t &value,
			const size_t &pos = 0 );
	size_t StringGet( const string &data, uint16_t &value,
			const size_t &pos = 0 );
	size_t StringGet( const string &data, uint32_t &value,
			const size_t &pos = 0 );

	const Ip Dns( const string &hostName );
	const string Dns( const Ip &hostIp );

	const sockaddr_in BuildSockaddr( const string &hostName );
	const sockaddr_in BuildSockaddr( const Ip &ip );
}

#endif
