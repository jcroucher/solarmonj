#ifndef JFYEXCEPTION_H
#define JFYEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

namespace Jfy
{
	class Exception
	:	public std::exception
	{
	public:
		Exception( const char* what ) throw();
		Exception( const string& what ) throw();
		Exception( const Exception& other ) throw();
		virtual ~Exception() throw();

		Exception& operator=( const Exception& other ) throw();

		virtual const char* what() const throw();

	private:
		string _what;
	};
}

#endif // JFYEXCEPTION_H
