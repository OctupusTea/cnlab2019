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
			uint16_t hostPort;
			uint32_t ttl;

			void SocketCreate( );

		public:
			IcmpSocket( const Ip &hostIp, const uint16_t &hostPort = -1,
					const uint32_t &ttl = 30 );
			IcmpSocket( const string &hostName, const uint16_t &hostPort
					= -1, const uint32_t &ttl = 30 );

			bool Send( const string &content );
			bool Send( const ICMP &icmp );
			
			bool Recv( string &content );
			bool Recv( ICMP &icmp );
	};
}

#endif
