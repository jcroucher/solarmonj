#include "jfyexception.h"

using namespace Jfy;

Exception::Exception( const char* what ) throw()
:	_what( what )
{
}

Exception::Exception( const string& what ) throw()
:	_what( what )
{
}

Exception::Exception( const Exception& other ) throw()
:	exception( other ),
	_what( other._what )
{
}

Exception::~Exception() throw()
{
}

Exception& Exception::operator=( const Exception& other ) throw()
{
	if ( this != &other )
		_what = other._what;

	return *this;
}

const char* Exception::what() const throw()
{
	return _what.c_str();
}





