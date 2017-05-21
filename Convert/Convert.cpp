/////////////////////////////////////////////////////////////////////
// Convert.cpp - Implement serialization to and from strings       //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "Convert.h"
#include <iostream>
#include <functional>

/////////////////////////////////////////////////////////////////////
// Widget class - shows user defined types can be converted
// - must have operator<< and operator>>

class Widget
{
public:
	Widget(const std::string& str = "") : state(str) {}
	std::string& value() { return state; }
private:
	std::string state;
};

std::ostream& operator<<(std::ostream& out, Widget& widget)
{
	out << widget.value();
	return out;
}

std::istream& operator >> (std::istream& in, Widget& widget)
{
	std::string temp;
	while (in.good())  // extract all the words from widget's string state
	{
		in >> temp;
		widget.value() += temp + " ";
	}
	return in;
}

/////////////////////////////////////////////////////////////////////
// Demo code

int main()
{
	std::cout << "Demonstrating Converting\n";

	std::cout << "Demonstrating Conversion of numerical types";
	std::string intString = Convert<int>::toString(42);
	std::cout << "\n  conversion of integer: " << intString;
	std::cout << "\n  retrieving integer:    " << Convert<int>::fromString(intString);
	std::cout << "\n";
	std::string dblString = Convert<double>::toString(3.1415927);
	std::cout << "\n  conversion of double:  " << dblString;
	std::cout << "\n  retrieving double:     " << Convert<double>::fromString(dblString);

	std::cout << "\nDemonstrating Conversion of instances of Widget classe";
	Widget widget("Joe Widget");
	std::string wdgString = Convert<Widget>::toString(widget);
	std::cout << "\n  conversion of Widget:  " << wdgString;
	Widget newWdg = Convert<Widget>::fromString(wdgString);
	std::cout << "\n  retrieved instance of: " << Convert<Widget>::id(newWdg);
	std::cout << "\n  Widget's value is:     " << newWdg.value();
	std::cout << "\n\n";
}