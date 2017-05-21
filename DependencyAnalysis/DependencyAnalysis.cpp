// DepAnal

#include "DependencyAnalysis.h"

#ifdef TEST_DEPANAL

int main()
{

	Executive exec;
	std::vector<std::string> fileVect = exec.getAllSubFiles();
	DependancyAnalysis dpAnal("..\..\depInfo.xml", 20);
	for (std::string file : fileVect) {
		dpAnal.startDependancyAnalysis(file);
	}
	dpAnal.displayDependancies();

}
#endif
