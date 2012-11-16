#ifndef JFYARRAY_H
#define JFYARRAY_H

#include <cstring>

namespace Jfy
{
	class PacketData
	{
	public:
		PacketData( size_t size );
		~PacketData();

		unsigned char& operator[]( int index );
		const unsigned char* data() const;
		size_t size() const;

	private:
		size_t _size;
		unsigned char* _data;
	};
}

#endif // JFYARRAY_H
