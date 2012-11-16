#ifndef JFY_H
#define JFY_H

namespace Jfy
{
	enum RequestCode {
		InvalidRequestCode = 0x0000,
		// Registration codes
		OfflineQuery = 0x3040,
		SendRegisterAddress = 0x3041,
		RemoveRegister = 0x3042,
		ReconnectRemovedInverter = 0x3043,
		ReRegister = 0x3044,
		// Read codes
		ReadDescription = 0x3140,
		ReadWriteDescription = 0x3141,
		QueryNormalInfo = 0x3142,
		QueryInverterIdInfo = 0x3143,
		ReadSetInfo = 0x3144,
		ReadRtcTime = 0x3145,
		ReadModelInfo = 0x3146,
		RielloFixSize = 0x3147,
		Pv33SlaveAInfo = 0x3148,
		Pv33SlaveBInfo = 0x3149,
		ReadDcCurrentInjection = 0x314a,
		ReadMasterSlaveLoggerVersion = 0x314b
	};

	enum ResponseCode {
		InvalidResponseCode = 0x00,
		AckResponseCode = 0x06,
		// Registration codes
		OfflineQueryResponseCode = 0x30bf,
		SendRegisterAddressResponseCode = 0x30be,
		RemoveRegisterResponseCode = 0x30bd,
		ReconnectRemovedInverterResponseCode = 0x30bc,
		ReRegisterResponseCode = 0x30bb,
		// Read codes
		ReadDescriptionResponseCode = 0x31bf,
		ReadWriteDescriptionResponseCode = 0x31be,
		QueryNormalInfoResponseCode = 0x31bd,
		QueryInverterIdInfoResponseCode = 0x31bc,
		ReadSetInfoResponseCode = 0x31bb,
		ReadRtcTimeResponseCode = 0x31ba,
		ReadModelInfoResponseCode = 0x31b9,
		RielloFixSizeResponseCode = 0x31b8,
		Pv33SlaveAInfoResponseCode = 0x31b7,
		Pv33SlaveBInfoResponseCode = 0x31b6,
		ReadDcCurrentInjectionResponseCode = 0x31b5,
		ReadMasterSlaveLoggerVersionResponseCode = 0x31b4
	};

	template< typename T > T bitReverse( T value ) {
		T result = value;
		int s = sizeof( value ) * 8 - 1;

		for ( value >>= 1; value; value >>= 1 ) {
			result <<= 1;
			result |= value & 1;
			s--;
		}

		result <<= s;

		return result;
	}

	class Common
	{
	public:
		static short buildShort( int b1, int b2 );
		static long buildLong( int b1, int b2, int b3, int b4 );
	};
}

#endif // JFY_H