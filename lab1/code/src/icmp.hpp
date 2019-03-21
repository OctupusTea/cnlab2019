#ifndef ICMP_HPP
#define ICMP_HPP

#include <iostream>
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

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_REQUEST 8

using std::ostream;
using std::string;

namespace traceroute
{
	class ICMP
	{
		private:
			uint8_t type;
			uint8_t code;
			uint16_t checksum;
			uint16_t id;
			uint16_t sequence;

			string data;

			void DoChecksum( );
			uint16_t ComputeChecksum( );

		public:
			ICMP( const uint16_t &sequence = 1, const string &data = "",
					const uint8_t &type = ICMP_ECHO_REQUEST );
			ICMP( const string &content );

			const uint8_t& Type( ) const { return type; }
			void Type( const uint8_t &type ) { this -> type = type; }

			const uint8_t& Code( ) const { return code; }

			const uint16_t& Checksum( ) const { return checksum; }
			bool CheckChecksum( );

			const uint16_t& Id( ) const { return id; }
			void GenerateId( );

			const uint16_t& Sequence( ) const { return sequence; }
			void Sequence( const uint16_t &sequence )
				{ this -> sequence = sequence; }

			const string& Data( ) const { return data; }
			void Data( const string &data ) { this -> data = data; }
			
			const string Content( ) const;

			friend ostream& operator<<( ostream &output, const ICMP &icmp );
	};
}
#endif
