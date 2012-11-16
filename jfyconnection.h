#ifndef JFYCONNECTION_H
#define JFYCONNECTION_H

#include <string>

using namespace std;

namespace Jfy
{
	typedef struct {
		float temperature;
		float energyToday;
		float voltageDc;
		float current;
		float voltageAc;
		float frequency;
		float energyCurrent;
		float energyTotal;
	} InverterData;
	
	class Serial;
	
	class Connection
	{
	public:
		Connection();
		Connection( const string& device );
		virtual ~Connection();

		bool init();
		void close();

		bool isRegistered() const;
		string serialNumber() const;
		unsigned char registrationAddress() const;

		bool readNormalInfo( InverterData* data );
		
	private:
		Serial* _conn;
		bool _registered;
		string _serialNumber;
		unsigned char _sourceAddress;
		unsigned char _destinationAddress;
	};
}

#endif // JFYCONNECTION_H
