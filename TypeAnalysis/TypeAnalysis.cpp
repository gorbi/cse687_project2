// TypeAnal

#include "TypeAnalysis.h"

#ifdef TEST_TYPEANAL

int main()
{

	CodeAnalysis::TypeAnal typAnal;
	typAnal.doTypeAnal();
	std::cout << "-------------------------------\n";
	std::cout << "Displaying Type Table Content\n";
	typAnal.showTypeTable();
	std::cout << "-------------------------------\n";

}
#endif
