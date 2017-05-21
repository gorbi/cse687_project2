// Display

#include "Display.h"

#ifdef TEST_DISPLAY

int main()
{

	DisplayItems disp;
	TypeTable typTab;
	typTab.doTypeAnal();
	DependencyTable depTab("../../xml1");
	depTab.doDepAnal(fileNames, typTab);
	StrongComponents sccComp("../../xml2.xml");
	sccComp.createGraph(objDepTable);

	disp.showTypeTable(typTab);
	disp.showDependancyTable(depTab);
	disp.showStrongComponents(sccComp);

	return 0;

}
#endif
