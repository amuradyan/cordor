#ifndef EXCEPTION_H
#define EXCEPTION_H

#define __STRICT_ANSI__

#include <string>

class ExceptionError {
	std::string exceptionMessage;
public:
	ExceptionError(std::string message) : exceptionMessage(message) {};
	std::string message() { return exceptionMessage; };
};

#endif