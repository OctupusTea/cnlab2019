#include "common.hpp"

#include <iostream>
#include <string>
#include <random>

#include <cstdint>

using std::string;

namespace traceroute
{
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
}
