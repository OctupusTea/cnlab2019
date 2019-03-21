#include "common.hpp"

#include <iostream>
#include <string>
#include <random>

#include <cstdint>
#include <cstdio>

#ifdef DEBUG
using std::clog;
using std::endl;
#endif

using std::string;

namespace traceroute
{
	random_device randomDevice;
	default_random_engine randomEngine( randomDevice( ) );
	uniform_int_distribution<uint16_t> randomDistro(0,0xFFFF);

	uint16_t RandomInt16( )
	{
		return randomDistro( randomEngine );
	}

	void StringAppend( string &data, const uint8_t &value )
	{
		data += static_cast<char>( value );
	}

	void StringAppend( string &data, const uint16_t &value )
	{
		data += static_cast<char>( value >> 8 );
		data += static_cast<char>( value & 0xFFFF );
	}

	void StringAppend( string &data, const uint32_t &value )
	{
		data += static_cast<char>( value >> 24 );
		data += static_cast<char>( ( value << 8 ) >> 24 );
		data += static_cast<char>( ( value << 16 ) >> 24 );
		data += static_cast<char>( value & 0xFFFF );
	}

	size_t StringGet( const string &data, uint8_t &value, const size_t &pos )
	{
		if( pos >= data.length( ) )
		{
			value = -1;
			return pos;
		}
		else
		{
			value = static_cast<uint8_t>( data[ pos ] );

			return pos + 1;
		}
	}

	size_t StringGet( const string &data, uint16_t &value, const size_t &pos )
	{
		if( pos + 1 >= data.length( ) )
		{
			value = -1;
			return pos;
		}
		else
		{
			value = static_cast<uint8_t>( data[ pos ] ) << 8;
			value += static_cast<uint8_t>( data[ pos ] );

			return pos + 2;
		}
	}

	size_t StringGet( const string &data, uint32_t &value, const size_t &pos )
	{
		if( pos + 3 >= data.length( ) )
		{
			value = -1;
			return pos;
		}
		else
		{
			value = static_cast<uint8_t>( data[ pos ] ) << 24;
			value = static_cast<uint8_t>( data[ pos ] ) << 16;
			value = static_cast<uint8_t>( data[ pos ] ) << 8;
			value = static_cast<uint8_t>( data[ pos ] );

			return pos + 4;
		}
	}

	const Ip Dns( const string &hostName )
	{
		FILE *dnsPipe = popen( ( "dnsip " + hostName ).c_str( ), "r" );
		string ipString = string( 16, '\0' );
		fread( &( ipString[ 0 ] ), 1, 16, dnsPipe );
#ifdef DEBUG
		clog << "ipString = " << ipString << endl;
#endif
		return Ip( ipString );
	}

	const string Dns( const Ip &ip )
	{
		FILE *dnsPipe = popen( ( "dnsname" + to_string( ip ) ).c_str( ), "r" );
		string hostName = string ( 65536, '\0' );
		fread( &( hostName[ 0 ] ), 1, 65535, dnsPipe );
#ifdef DEBUG
		clog << "hostName = " << hostName << endl;
#endif
		return hostName;
	}

	const sockaddr_in BuildSockaddr( const string &hostName,
			const uint16_t &port )
	{
		sockaddr_in sockAddr;

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr = { Dns( hostName ).numFormat };
		sockAddr.sin_port = port;

		return sockAddr;
	}

	const sockaddr_in BuildSockaddr( const Ip &ip, const uint16_t &port )
	{
		sockaddr_in sockAddr;

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr = { ip.numFormat };
		sockAddr.sin_port = port;

		return sockAddr;
	}
}
