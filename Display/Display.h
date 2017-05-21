#pragma once
//////////////////////////////////////////////////////////////////////////
// Display.h - Displays Analyzer's AST and TypeTable                    //
// Ver 1.0                                                              //
// Platform:    Apple Mac Mini Late 2014, Win 10 Education,             //
//              Visual Studio 2015 Enterprise                           //
// Author:      Nagaprasad Natarajaurs, 418910728                       //
//              nnataraj@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Uses information in the Analyzer's AST and TypeTable to build an
* effective display of the dependency relationships between all files
* in the selected file set. 
*
* Required Files:
* ---------------
*   - Display.h, Display.cpp,
*     StrongComponents.h, StrongComponents.cpp,
*     TypeAnalysis.h, TypeAnalysis.cpp
*     DependencyAnalysis.h, DependencyAnalysis.cpp
*
* Build Process:
* --------------
*   devenv Display.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 1.0 : 13 Mar 2017
* - first release
*
*/

#include "../StrongComponents/StrongComponents.h"

namespace CodeAnalysis
{
	class Display
	{
	public:
		void showTypeTable(TypeAnal&);
		void showDependencyTable(DepAnal&);
		void showStrongComponents(StrongComponents&);
		Display();
	};

	//Empty constructor just to show requirement page
	Display::Display()
	{
		std::cout << "\n-----------------Requirement 3: Created Display package to effectively display of the dependency relationships-------------------\n\n";
	}

	//Display Type Table
	void Display::showTypeTable(TypeAnal & typTable)
	{
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
		std::cout << "\n****************************************Type Table*******************************************\n";
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
		typTable.showTypeTable();
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
		std::cout << "\n\n";
		std::cout << "-----------------------------GlobalFunctionTable------------------------------------------------\n";
		std::cout << std::setw(20) << "TypeName" << std::setw(17) << "Type" << std::setw(25) << "Namespace" << "Filename";
		std::cout << "\n-----------------------------------------------------------------------------------------------\n";
		std::unordered_map<std::string, std::vector<std::string>>& globFuncMap = typTable.getGlobFuncMap();
		for (auto globalFunc : globFuncMap)
		{
			std::cout << std::setw(20) << globalFunc.first << std::setw(20) << "GlobalFunction" << std::setw(20) << "GlobalNamespace";
			for (std::string fname : globalFunc.second)
			{
				std::cout << fname << "  ";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}

	//Display Dependency Table
	void Display::showDependencyTable(DepAnal & depTable)
	{
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
		std::cout << "\n***************************************Dependency Table**************************************\n";
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
		depTable.showDependencies();
		std::cout << "\n---------------------------------------------------------------------------------------------\n";
	}

	//Display Strongly Connected Components
	void Display::showStrongComponents(StrongComponents & SCCInst)
	{
		std::cout << "\n-----------------Requirement 6: SCC created from the dependency graph-------------------\n\n";
		std::cout << "\n-------------------------------------------------------------------------------------\n";
		std::cout << "\n*****************************Strongly Connected Components***************************\n";
		std::cout << "\n-------------------------------------------------------------------------------------\n";
		SCCInst.showStrongComp();
		std::cout << "\n-------------------------------------------------------------------------------------\n";
	}

}