#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "common.hpp"
#include "icmp.hpp"

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
	class IcmpSocket
	{
		private:
			int socketFd;

			string hostName;
			bool hostNameValid;
			Ip hostIp;

			void SocketCreate( );

			bool Send( const string &content, const uint32_t &ttl );
			bool Recv( string &content );

		public:
			IcmpSocket( const Ip &hostIp = Ip( static_cast<uint32_t>( 0 ) ) );
			IcmpSocket( const string &hostName );

			bool Send( const ICMP &icmp, const uint32_t &ttl );
			
			bool Recv( ICMP &icmp );
	};
}

#endif
