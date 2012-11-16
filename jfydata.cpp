#include "jfydata.h"

#include <fstream>
#include <iostream>

using namespace Jfy;
using namespace std;

Data::Data()
:	_requestCode( InvalidRequestCode ),
	_sourceAddress( 0 ),
	_destinationAddress( 0 ),
	_dataSize( 0 ),
	_bufferSize( 0 ),
	_data( 0 )
{
}

Data::Data( RequestCode code, unsigned char source, unsigned char destination )
:	_requestCode( code ),
	_sourceAddress( source ),
	_destinationAddress( destination ),
	_dataSize( 0 ),
	_bufferSize( 0 ),
	_data( 0 )
{
}

Data::Data( const Data& other )
:	_requestCode( other._requestCode ),
	_sourceAddress( other._sourceAddress ),
	_destinationAddress( other._destinationAddress ),
	_dataSize( other._dataSize ),
	_bufferSize( 0 ),
	_data( 0 )
{
	allocateData( _dataSize );
	_dataSize = other._dataSize;
	memcpy( _data, other._data, _dataSize );
}

Data::~Data()
{
	clear();
}

Data& Data::operator=( const Data& other )
{
	if ( this != &other ) {
		_requestCode = other._requestCode;
		_sourceAddress = other._sourceAddress;
		_destinationAddress = other._destinationAddress;

		allocateData( other._dataSize );
		_dataSize = other._dataSize;
		memcpy( _data, other._data, _dataSize );
	}

	return *this;
}

bool Data::isValid() const
{
	return _requestCode != InvalidRequestCode;
}

void Data::setRequestCode( RequestCode code )
{
	_requestCode = code;
}

void Data::setControlCode( unsigned char code )
{
	_requestCode = static_cast< RequestCode >( ( _requestCode & 0x00ff ) | ( (unsigned short)code ) << 8 );
}

void Data::setFunctionCode( unsigned char code )
{
	_requestCode = static_cast< RequestCode >( ( _requestCode & 0xff00 ) | code );
}

RequestCode Data::requestCode() const
{
	return _requestCode;
}

ResponseCode Data::responseCode() const
{
	return static_cast< ResponseCode >( _requestCode );
}

void Data::setSourceAddress( unsigned char source )
{
	_sourceAddress = source;
}

unsigned char Data::sourceAddress() const
{
	return _sourceAddress;
}

void Data::setDestinationAddress( unsigned char destination )
{
	_destinationAddress = destination;
}

unsigned char Data::destinationAddress() const
{
	return _destinationAddress;
}

void Data::addData( char data )
{
	addData( &data, 1 );
}

void Data::addData( const string& data )
{
	addData( data.c_str(), data.length() );
}

void Data::addData( const char* data, size_t size )
{
	reallocateData( size );

	memcpy( _data + _dataSize, data, size );
	_dataSize += size;
}

void Data::setData( char* data, int size )
{
	clearData();
	addData( data, size );
}

void Data::setData( const string& data )
{
	clearData();
	addData( data );
}

size_t Data::size() const
{
	return _dataSize;
}

const unsigned char* Data::data() const
{
	return _data;
}

string Data::toString() const
{
	string s;
	
	for ( int i = 0; i < _dataSize; ++i )
		s += _data[ i ];

	return s;
}

unsigned char Data::controlCode() const
{
	return ( _requestCode & 0xff00 ) >> 8;
}

unsigned char Data::functionCode() const
{
	return _requestCode & 0x00ff;
}

unsigned short Data::checksum() const
{
	// Add the heder, source, destination, control code, function code and data size
	unsigned short value = 0xa5 + 0xa5 + _sourceAddress + _destinationAddress + controlCode() + functionCode() + _dataSize;

	// Add the data
	for ( int i = 0; i < _dataSize; ++i )
		value += _data[ i ];

	// Flip the bits and subtract 1 (per documentation)
	value ^= 0xffff;
	++value;

	return value;
}

PacketData Data::packetData() const
{
	// Packets are always 11 bytes plus the data size
	size_t size = 11 + _dataSize;
	PacketData d( size );

	// Start at the first byte and go from there
	int index = 0;
	
	// Add the header
	d[ index++ ] = 0xa5;
	d[ index++ ] = 0xa5;

	// Add the source address
	d[ index++ ] = _sourceAddress;

	// Add the destination address
	d[ index++ ] = _destinationAddress;

	// Add the control code
	d[ index++ ] = controlCode();

	// Add the function code
	d[ index++ ] = functionCode();

	// Add the data length
	d[ index++ ] = static_cast< unsigned char >( _dataSize );

	// Add the data
	for ( int i = 0; i < _dataSize; ++i )
		d[ index++ ] = _data[ i ];

	// Add the checksum
	unsigned short c = checksum();
	d[ index++ ] = ( c & 0xff00 ) >> 8;
	d[ index++ ] = c & 0x00ff;

	// Add the trailing bytes
	d[ index++ ] = 0x0a;
	d[ index++ ] = 0x0d;

	return d;
}

void Data::allocateData( size_t size )
{
	delete _data;

	_bufferSize = size + ( 16 - size % 16 );
	_data = new unsigned char[ _bufferSize ];
	_dataSize = 0;
}

void Data::reallocateData( size_t extraBytes )
{
	size_t newSize = _dataSize + extraBytes;
	
	if ( newSize <= _bufferSize )
		return;

	newSize = newSize + ( 16 - newSize % 16 );
	unsigned char* newData = new unsigned char[ newSize ];

	if ( _data ) {
		memcpy( newData, _data, _bufferSize );
		delete _data;
	}

	_bufferSize = newSize;
	_data = newData;
}

void Data::clear()
{
	_requestCode = InvalidRequestCode;
	_sourceAddress = 0;
	_destinationAddress = 0;

	clearData();
}

void Data::clearData()
{
	delete _data;
	_data = 0;
	_dataSize = 0;
	_bufferSize = 0;
}
