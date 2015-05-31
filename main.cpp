#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdlib.h>

#include "clientsocket.h"
#include "socketexception.h"

#include "jfyconnection.h"

using namespace std;

int main( int argc, char** argv )
{
	Jfy::Connection conn( "/dev/ttyUSB0" );
	string logPathRoot = "/var/log/solarmonj/";
	
	if ( !conn.init() ) {
		cerr << "Cannot initialise the connection." << endl;
		return 1;
	}
	
	// Get the current time
	time_t now = time(0);
	
	// Get the data
	Jfy::InverterData data;
	conn.readNormalInfo( &data );
	
	// Write to logstash

	try
	{
		ClientSocket client_socket ( "localhost", 7022 );

		try
		{
			stringstream ss;

			ss << now << "," <<   data.temperature << "," << data.energyCurrent << "," << data.energyToday << "," << data.pvoltageAc << "," << data.voltageDc << "," << data.voltageAc << "," << data.frequency << "," << data.current << "\n";

			client_socket << ss.str();
			std::cout << "Sent\n";
		}
		catch (SocketException& e)
		{
      			std::cout << "Error writing to logging server:\n\"" << e.description() << "\"\n";	
		}

	} 
	catch (SocketException& e)
	{
		std::cout << "Error connecting to logging server:\n\"" << e.description() << "\"\n";
	}

	conn.close();

	return 0;
}

