#include "jfyserial.h"

#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "jfyexception.h"

using namespace std;
using namespace Jfy;

Serial::Serial()
:	_handle( -1 ),
	_readTimeout( 1 )
{
}

Serial::Serial( const string& device )
:	_device( device ),
	_handle( -1 ),
	_readTimeout( 1 )
{
}

bool Serial::setDevice( const string& device )
{
	if ( isOpen() ) {
		cerr << "Device is open." << endl;
		return false;
	}

	_device = device;
	return true;
}

string Serial::device() const
{
	return _device;
}

void Serial::setReadTimeout( int seconds )
{
	_readTimeout = seconds;
}

int Serial::readTimeout() const
{
	return _readTimeout;
}

bool Serial::open()
{
	if ( _device.empty() ) {
		cerr << "Device is not set." << endl;
		return false;
	}

	if ( isOpen() ) {
		cerr << "Device is already open." << endl;
		return true;
	}
	
	struct termios options;

	// Open the device
	_handle = ::open( _device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );

	if ( _handle == -1 ) {
		cerr << "Cannot open " << _device << "." << endl;
		return false;
	}

	fcntl( _handle, F_SETFL, 0 );
	
	// Set a bunch of options on the serial device
	tcgetattr( _handle, &options );

	cfsetispeed( &options, B9600 );
	cfsetospeed( &options, B9600 );
	cfmakeraw( &options );
    options.c_cc[ VMIN ] = 0;
    options.c_cc[ VTIME ] = 10;
	tcsetattr( _handle, TCSANOW, &options );

	return true;
}

void Serial::close()
{
	if ( _handle == -1 )
		return;
	
	::close( _handle );
	_handle = -1;
}

bool Serial::isOpen() const
{
	return _handle != -1;
}

bool Serial::sendRequest( const Data& request )
{
	if ( !isOpen() ) {
		cerr << "Device is not open." << endl;
		return false;
	}

	PacketData packetData = request.packetData();

	size_t bytesWritten = write( _handle, packetData.data(), packetData.size() );

	if ( bytesWritten == -1 ) {
		cerr << "Could not write to the device." << endl;
		return false;
	} else if ( bytesWritten < packetData.size() ) {
		cerr << "Short write to device." << endl;
		return false;
	}
	
	return true;
}

Data Serial::readResponse()
{
	Data response;

	try {
		if ( !isOpen() )
			throw Exception( "Device is not open." );

		// Read the header
		if ( !waitForBytesReady( 7 ) )
			throw Exception( "Device timed out." );

		unsigned char header[ 7 ];
		readData( header, sizeof( header ) );

		if ( header[ 0 ] != 0xa5 || header[ 1 ] != 0x0a5 )
			throw Exception( "Response header is invalid." );
		
		response.setSourceAddress( header[ 2 ] );
		response.setDestinationAddress( header[ 3 ] );
		response.setControlCode( header[ 4 ] );
		response.setFunctionCode( header[ 5 ] );

		int dataSize = static_cast< int >( header[ 6 ] );

		// Read the data (if any)
		if ( dataSize > 0 ) {
			if ( !waitForBytesReady( dataSize ) )
				throw Exception( "Device timed out." );

			char* responseData = new char[ dataSize ];
			readData( responseData, dataSize );

			response.setData( responseData, dataSize );
			delete [] responseData;
		}

		unsigned short checksum = readUnsignedShort();

		unsigned short footer;
		readData( &footer, 2 );

		if ( checksum != response.checksum() )
			throw Exception( "Checksum mismatch" );
	}
	catch ( Exception e ) {
		cerr << e.what() << endl;
		return Data();
	}

	return response;
}

Data Serial::sendRequestReadResponse( const Data& request )
{
	Data response;
	
	for ( int retry = 0; retry < 3; ++retry ) {
		if ( !sendRequest( request ) )
			continue;

		response = readResponse();

		if ( response.isValid() )
			break;
	}

	return response;
}

bool Serial::waitForBytesReady( int numBytes ) const
{
	int bytes = 0;
	time_t startTime = time( 0 );

	while ( time( 0 ) - startTime < _readTimeout ) {
		if ( ioctl( _handle, FIONREAD, &bytes ) == -1 ) {
			cerr << "Cannot determine number of bytes waiting on the device." << endl;
			return false;
		}

		if ( bytes >= numBytes )
			break;
	}

	return bytes >= numBytes;
}

void Serial::readData( void* buffer, size_t size )
{
	if ( !waitForBytesReady( size ) )
		throw Exception( "Timed out waiting for data to become available." );
	
	char* buf = (char*)buffer;
	time_t startTime = time( 0 );
	
	while ( size > 0 && time( 0 ) - startTime < _readTimeout ) {
		int bytesRead = read( _handle, buf, size );

		if ( bytesRead == -1 )
			throw Exception( "Could not read from the device." );

		buf += bytesRead;
		size -= bytesRead;
	}

	if ( size > 0 )
		throw Exception( "Read timed out." );
}

short Serial::readShort()
{
	short s;
	
	try {
		if ( !waitForBytesReady( 2 ) )
			throw Exception( "Device timed out." );

		readData( &s, sizeof( s ) );
		s = ( ( s & 0xff00 ) >> 8 ) | ( ( s & 0x00ff ) << 8 );
	}
	catch ( Exception e ) {
		cerr << e.what() << endl;
		return 0;
	}

	return s;
}

unsigned short Serial::readUnsignedShort()
{
	unsigned short s;

	try {
		if ( !waitForBytesReady( 2 ) )
			throw Exception( "Device timed out." );

		readData( &s, sizeof( s ) );
		s = ( ( s & 0xff00 ) >> 8 ) | ( ( s & 0x00ff ) << 8 );
	}
	catch ( Exception e ) {
		cerr << e.what() << endl;
		return 0;
	}

	return s;
}
