#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdlib.h>

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
	
	struct stat sb;
	std::stringstream sstm;
	
	// Get the current time, used for logs
	time_t now 			= time(0);
	struct tm *tmData = localtime(&now);
	
	int year 	= tmData->tm_year + 1900;
	int month 	= tmData->tm_mon + 1;
	int day 		= tmData->tm_mday;
	
	// Create the log path using the year and month
	sstm << logPathRoot << year << "/" << month;
	string logPath = sstm.str();
	
	// Add the file name to the path
	sstm << "/" << day << ".csv";
	string logFile = sstm.str();
	
	if(stat(&logPath[0], &sb) != 0 )
	{
		string command = "mkdir -p " + logPath;
		system(&command[0]);
	}
		
	// Get the data
	Jfy::InverterData data;
	conn.readNormalInfo( &data );
	
	// Log the data
	ofstream csvFile;
	csvFile.open (&logFile[0], ios::app);	
	csvFile << now  << "," <<   data.temperature << "," << data.energyCurrent << "," << data.energyToday << "," << data.energyTotal << "," << data.voltageDc << "," << data.voltageAc << "," << data.frequency << "," << data.current << "\n";
  	csvFile.close();

	// Close the connection to the inverter. 
	conn.close();

	return 0;
}

