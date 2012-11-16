#include "jfypacketdata.h"

#include "jfyexception.h"

using namespace Jfy;

PacketData::PacketData( size_t size )
:	_size( size ),
	_data( new unsigned char[ size ] )
{
}

PacketData::~PacketData()
{
	delete [] _data;
}

unsigned char& PacketData::operator[]( int index )
{
	if ( index < 0 || index >= _size )
		throw Exception( "Index out of range" );

	return _data[ index ];
}

const unsigned char* PacketData::data() const
{
	return _data;
}

size_t PacketData::size() const
{
	return _size;
}
