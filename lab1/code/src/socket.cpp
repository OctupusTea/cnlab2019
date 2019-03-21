#include "common.hpp"
#include "icmp.hpp"
#include "socket.hpp"

#include <iostream>
#include <string>

#include <cstdint>
#include <cstring>

extern "C"
{

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

}

using std::clog;
using std::endl;

using std::string;

namespace traceroute
{
	IcmpSocket::IcmpSocket( const Ip &hostIp ) :
		hostName( ), hostNameValid( false ), hostIp( hostIp )
	{
		SocketCreate( );
	}

	IcmpSocket::IcmpSocket( const string &hostName ) :
		hostName( hostName ), hostNameValid( true ), hostIp( Dns( hostName ) )
	{
		SocketCreate( );
	}

	void IcmpSocket::SocketCreate( )
	{
		do
		{
			socketFd = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
		}while( socketFd == -1 );
	}

	bool IcmpSocket::Send( const string &content, const uint32_t &ttl )
	{
		sockaddr_in sockAddr = BuildSockaddr( hostIp );
		setsockopt( socketFd, IPPROTO_IP, IP_TTL, &ttl, sizeof( ttl ) );
		timeval timeout = { 2, 0 };
		setsockopt( socketFd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof( timeout ) );

		auto bytes_sent = sendto( socketFd, content.c_str( ),
				content.length( ), 0, reinterpret_cast<sockaddr*>( &sockAddr ),
				sizeof( sockAddr ) );
#ifdef DEBUG
		clog << "content length = " << content.length( ) << endl;
		clog << "bytes_sent = " << bytes_sent << endl;
#endif
		return ( bytes_sent == content.length( ) );
	}

	bool IcmpSocket::Send( const ICMP &icmp, const uint32_t &ttl )
	{
		return Send( icmp.Content( ), ttl );
	}

	bool IcmpSocket::Recv( string &content )
	{
		content.resize( 65536 );

		sockaddr_in sockAddr = BuildSockaddr( hostIp );
		socklen_t addrLength = sizeof( sockAddr );
		auto bytes_recv = recvfrom( socketFd, &( content[ 0 ] ), 65536, 0,
				reinterpret_cast<sockaddr*>( &sockAddr ), &addrLength );
#ifdef DEBUG
		clog << "bytes_recv = " << bytes_recv << endl;

		if( bytes_recv == -1 )
		{
			clog << "errno = " << errno << " : " << strerror( errno ) << endl;
		}
#endif
		return ( bytes_recv > 0 );
	}

	bool IcmpSocket::Recv( ICMP &icmp )
	{
		string content;
		if( Recv( content ) )
		{
			icmp = ICMP( content );
			return true;
		}
		else
		{
			return false;
		}
	}
}
