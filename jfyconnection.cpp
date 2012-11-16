#include "jfyconnection.h"

#include <stdlib.h>
#include <iostream>
#include "jfycommon.h"
#include "jfyexception.h"
#include "jfyserial.h"

using namespace std;
using namespace Jfy;

Connection::Connection()
:	_conn( new Serial ),
	_registered( false ),
	_sourceAddress( 1 ),
	_destinationAddress( 1 )
{
}

Connection::Connection( const string& device )
:	_conn( new Serial( device ) ),
	_registered( false ),
	_sourceAddress( 1 ),
	_destinationAddress( 1 )
{
}

Connection::~Connection()
{
	close();
	delete _conn;
}

bool Connection::init()
{
	srand( time( 0 ) );
	
	if ( !_conn->open() )
		return false;

	try {
		// Re-register wtih the inverter
		_conn->sendRequest( Data( Jfy::ReRegister, _sourceAddress, 0 ) );
		
		// Get the serial number
		Data response = _conn->sendRequestReadResponse( Data( Jfy::OfflineQuery, _sourceAddress, 0 ) );

		if ( !response.isValid() )
			throw Exception( "Cannot read the serial number from the device." );

		_serialNumber = response.toString();

		sleep( 1 );

		Data request( SendRegisterAddress, _sourceAddress, 0 );
		request.addData( _serialNumber );
		request.addData( _destinationAddress );

		response = _conn->sendRequestReadResponse( request );

		if ( !response.isValid() || response.size() == 0 || response.data()[ 0 ] != AckResponseCode )
			throw Exception( "Cannot read the registration response." );

		_registered = true;
	}
	catch ( Exception e ) {
		cerr << e.what() << endl;
		close();
		return false;
	}

	return true;
}

void Connection::close()
{
	if ( _registered ) {
		Data response = _conn->sendRequestReadResponse( Data( RemoveRegister, _sourceAddress, _destinationAddress ) );

		if ( !response.isValid() || response.size() == 0 || response.data()[ 0 ] != AckResponseCode )
			cerr << "Could not send the remove registration request to the device." << endl;
	}

	_conn->close();
	_sourceAddress = 0;
	_destinationAddress = 0;
	_registered = false;
	_serialNumber = "";
}

bool Connection::isRegistered() const
{
	return _registered;
}

string Connection::serialNumber() const
{
	return _serialNumber;
}

unsigned char Connection::registrationAddress() const
{
	return _destinationAddress;
}

bool Connection::readNormalInfo( InverterData* data )
{
	if ( !isRegistered() ) {
		cerr << "Device is not registered." << endl;
		return false;
	}

	Data response = _conn->sendRequestReadResponse( Data( QueryNormalInfo, _sourceAddress, _destinationAddress ) );

	if ( !response.isValid() ) {
		cerr << "Could not read the information response." << endl;
		return false;
	}

	const unsigned char* buf = response.data();
	int size = response.size();

	data->temperature = Common::buildShort( buf[ 0 ], buf[ 1 ] ) / 10.0;
	data->energyToday = Common::buildShort( buf[ 2 ], buf[ 3 ] ) / 100.0;
	data->voltageDc = Common::buildShort( buf[ 4 ], buf[ 5 ] ) / 10.0;
	data->current = Common::buildShort( buf[ 6 ], buf[ 7 ] ) / 10.0;
	data->voltageAc = Common::buildShort( buf[ 8 ], buf[ 9 ] ) / 10.0;
	data->frequency = Common::buildShort( buf[ 10 ], buf[ 11 ] ) / 100.0;
	data->energyCurrent = Common::buildShort( buf[ 12 ], buf[ 13 ] );
	//
	data->energyTotal = Common::buildShort( buf[ 18 ], buf[ 19 ] ) / 10.0;
	
	return true;
}
