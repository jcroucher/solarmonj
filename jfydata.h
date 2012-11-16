#ifndef JFYDATA_H
#define JFYDATA_H

#include <string>
#include <vector>
#include "jfypacketdata.h"
#include "jfycommon.h"

using namespace std;

namespace Jfy
{
	class Data
	{
	public:
		Data();
		Data( RequestCode code, unsigned char source, unsigned char destination );
		Data( const Data& other );
		virtual ~Data();

		Data& operator=( const Data& other );

		bool isValid() const;
		void setRequestCode( RequestCode code );
		void setControlCode( unsigned char code );
		void setFunctionCode( unsigned char code );
		RequestCode requestCode() const;
		ResponseCode responseCode() const;
		void setSourceAddress( unsigned char source );
		unsigned char sourceAddress() const;
		void setDestinationAddress( unsigned char destination );
		unsigned char destinationAddress() const;
		void addData( char data );
		void addData( const string& data );
		void addData( const char* data, size_t size );
		void setData( char* data, int size );
		void setData( const string& data );
		size_t size() const;
		const unsigned char* data() const;
		string toString() const;
		unsigned char controlCode() const;
		unsigned char functionCode() const;
		unsigned short checksum() const;
		PacketData packetData() const;

	private:
		void allocateData( size_t size );
		void reallocateData( size_t extraBytes );
		void clear();
		void clearData();
		
	private:
		RequestCode _requestCode;
		unsigned char _sourceAddress;
		unsigned char _destinationAddress;
		size_t _dataSize;
		size_t _bufferSize;
		unsigned char* _data;
	};
}

#endif // JFYDATA_H
