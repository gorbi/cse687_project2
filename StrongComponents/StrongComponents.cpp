// StrongComponents

#include "StrongComponents.h"

#ifdef TEST_STRONGCOMPONENTS

int main()
{

	Executive exec;
	StronglyConnectedComponents scc(exec.sccXmlPath);
	scc.createGraph(dp, allsubfiles);
	std::cout << "---------------------------------------\n";
	std::cout << "Strongly Connected Components\n";
	std::cout << "---------------------------------------\n";
	scc.showStrongComp();

}
#endif
