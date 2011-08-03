#ifndef CTWEXCEPTION_H_
#define CTWEXCEPTION_H_

#include <exception>
#include <string>

namespace ctw {

class CtwException:
	public std::exception
{
public:
	CtwException(std::string info)
		:what_(info)
	{}

	virtual const char *what() const throw()
	{
		return what_.c_str();
	}

	~CtwException() throw() { }

private:
	std::string what_;
};

}

#endif /* CTWEXCEPTION_H_ */
