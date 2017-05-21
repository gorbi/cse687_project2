#pragma once
/////////////////////////////////////////////////////////////////////
// Convert.h - Implement serialization to and from strings         //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include <sstream>
#include <string>

/////////////////////////////////////////////////////////////////////
// Convert<T> class
// - (de)serializes objects of type T (from)to strings
// - generates a type identifier using Run-Time Type Identification
//
template<typename T>
class Convert
{
public:
	virtual ~Convert<T>() {}
	static std::string toString(const T& t);
	static T fromString(const std::string& str);
	static std::string id(const T& t);
};
/*----< serialize t to a std::string >-----------------------------*/
/*
* Assumes that T defines an insertion operator.
*/
template<typename T>
std::string Convert<T>::toString(const T& t)
{
	std::ostringstream out;
	out << *const_cast<T*>(&t);
	return out.str();
}
/*----< create an instance of T from a std::string >---------------*/
/*
* Assumes that T defines an extraction operator.
* To succeed str must be created from Convert<T>::toString(const T& t).
*/
template<typename T>
T Convert<T>::fromString(const std::string& str)
{
	std::istringstream in(str);
	T value;
	in >> value;    // istringstream extraction provides the conversion
	return value;
}
/*----< create an id string from an instance of T using RTTI >----*/

template<typename T>
std::string Convert<T>::id(const T& t)
{
	return typeid(t).name();
}
