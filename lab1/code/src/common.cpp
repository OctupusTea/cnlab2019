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
}
