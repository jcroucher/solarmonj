#include "jfycommon.h"

using namespace Jfy;

short Common::buildShort( int b1, int b2 )
{
	return ( ( (short)b1 & 0x0ff ) << 8 | ( (short)b2 & 0x0ff ) );
}

long Common::buildLong( int b1, int b2, int b3, int b4 )
{
	return ( ( (long)b1 & 0x0ff ) << 24 ) | ( ( (long)b2 & 0x0ff ) << 16 ) | ( ( (long)b3 & 0x0ff ) << 8 ) | ( (long)b4 & 0x0ff );
}
