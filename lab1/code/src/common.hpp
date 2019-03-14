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

namespace traceroute
{
	random_device randomDevice;
	default_random_engine randomEngine
		= default_random_engine( randomDevice( ) );
	uniform_int_distribution<uint16_t> randomDistro(0,0xFFFF);

	auto RandomInt16 = std::bind( randomDistro, randomEngine );

	void StringAppend( string &data, const uint8_t &value );
	void StringAppend( string &data, const uint16_t &value );
	void StringAppend( string &data, const uint32_t &value );
}

#endif
