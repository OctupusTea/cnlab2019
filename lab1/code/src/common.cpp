#include "common.hpp"

#include <iostream>
#include <string>
#include <random>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
        struct addrinfo hints, *res, *p;
        int status;
        char ipstr[INET6_ADDRSTRLEN];


        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(hostName.c_str(), NULL, &hints, &res)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        }

        string ip;
        for(p = res;p != NULL; p = p->ai_next) {
            void *addr;

            if (p->ai_family == AF_INET) { // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
            } else { // IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
            }

            // convert the IP to a string and print it:
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            ip = ipstr;
        }

        freeaddrinfo(res);
		return Ip( ip );
// 		FILE *dnsPipe = popen( ( "dnsip " + hostName ).c_str( ), "r" );
// 		string ipString = string( 16, '\0' );
// 		fread( &( ipString[ 0 ] ), 1, 16, dnsPipe );
// #ifdef DEBUG
// 		clog << "ipString = " << ipString << endl;
// #endif
// 		return Ip( ipString );
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

	const sockaddr_in BuildSockaddr( const string &hostName )
	{
		sockaddr_in sockAddr;

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr = { Dns( hostName ).numFormat };

		return sockAddr;
	}

	const sockaddr_in BuildSockaddr( const Ip &ip )
	{
		sockaddr_in sockAddr;

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr = { ip.numFormat };

		return sockAddr;
	}
}
