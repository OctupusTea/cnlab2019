#include "common.hpp"
#include "icmp.hpp"
#include "socket.hpp"

#include <string>

#include <cstdint>

extern "C"
{

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

}

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
		socketFd = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
	}

	bool IcmpSocket::Send( const string &content, const uint32_t &ttl )
	{
		sockaddr_in sockAddr = BuildSockaddr( hostIp );
		setsockopt( socketFd, IPPROTO_IP, IP_TTL, &ttl, sizeof( ttl ) );

		size_t bytes_sent = sendto( socketFd, content.c_str( ),
				content.length( ), 0, reinterpret_cast<sockaddr*>( &sockAddr ),
				sizeof( sockAddr ) );

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
		size_t bytes_recv = recvfrom( socketFd, &( content[ 0 ] ), 65536, 0,
				reinterpret_cast<sockaddr*>( &sockAddr ), &addrLength );

		return ( bytes_recv > 0 );
	}

	bool IcmpSocket::Recv( ICMP &icmp )
	{
		string content;
		if( not Recv( content ) )
		{
			return false;
		}
		else
		{
			icmp = ICMP( content );
			return true;
		}
	}
}
