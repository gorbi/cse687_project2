#pragma once
//////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - Finds, for each file in a specified file      //
//                        collection, all other files from the file     //
//                        collection on which they depend               //
// Ver 1.0                                                              //
// Platform:    Apple Mac Mini Late 2014, Win 10 Education,             //
//              Visual Studio 2015 Enterprise                           //
// Author:      Nagaprasad Natarajaurs, 418910728                       //
//              nnataraj@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* It is used to find, for each file in a specified file collection,
* all other files from the file collection on which they depend. File A
* depends on file B, if and only if, it uses the name of any type or
* global function or global data defined in file B. It might do that by
* calling a function or method of a type or by inheriting the type.
*
* Note that this intentionally does not record dependencies of a file
* on files outside the file set, e.g., language and platform libraries. 
*
* Required Files:
* ---------------
*   - TypeAnalysis.h, TypeAnalysis.cpp,
*     NoSqlDb.h, NoSqlDb.cpp,
*     Tokenizer.h, Tokenizer.cpp,
*     DependencyAnalysis.h, DependencyAnalysis.cpp,
*
* Build Process:
* --------------
*   devenv DependencyAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 1.0 : 13 Mar 2017
* - first release
*
*/
#pragma warning (disable: 4503)

#include "../NoSqlDb/NoSqlDb.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../Tokenizer/Tokenizer.h"
using namespace Scanner;


namespace CodeAnalysis
{
	class DepAnal
	{
	public:
		//DependencyAnalysis constructor & initialises NoSQLDB to given path & writeCount
		DepAnal(std::string fpsth, int persistcount) : db(fpsth, persistcount) {
			std::cout << "\n-----------------Requirements 3 & 5: Created DependencyAnalysis package for Dependency Analysis-------------------\n\n";
		}
		void doDepAnal(TypeAnal& typ, std::string tokFile);
		void checkAgainstTypeTable(std::string fName, std::string token, TypeAnal& typ);
		NoSqlDb<std::string>& getDb();
		void showDependencies();
	private:
		NoSqlDb<std::string> db;
	};

	//Starts dependency analysis & uses tokenizer on each input file
	void DepAnal::doDepAnal(TypeAnal& typ, std::string tokFile) {
		try {
			std::string fileName = FileSystem::Path::getName(tokFile);
			std::ifstream in(tokFile);
			if (!in.good())
			{
				std::cout << "\n  can't open " << tokFile << "\n\n";
			}
			{
				Toker toker;
				toker.returnComments();
				toker.attach(&in);
				do
				{
					std::string tok = toker.getTok();
					if (tok == "\n")
						tok = "newline";
					checkAgainstTypeTable(tokFile, tok, typ);
				} while (in.good());
			}
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
	}

	//Returns the NoSQLDb which has dependency analysis data
	NoSqlDb<std::string>& DepAnal::getDb() {
		return db;
	}

	//Checks whether token is present in type table
	//and returns list of all files that the token is found in
	//Stores dependency info in the database
	void DepAnal::checkAgainstTypeTable(std::string fileName, std::string token, TypeAnal& typ) {
		std::vector<std::string> depFileVect = typ.returnDependentFiles(token);
		if (depFileVect.size() != 0) {
			Element<std::string> elem;
			elem.name = fileName;
			elem.children = depFileVect;
			bool flag = db.save(fileName, elem);
			if (!flag) {
				db.updateChildVal(fileName, depFileVect);
			}
		}
	}

	//display dependencies
	void DepAnal::showDependencies() {
		std::cout << "------------------------------------" << std::endl;
		std::cout << "Displaying Database contents: " << std::endl;
		std::cout << "------------------------------------" << std::endl;
		std::cout << "\n";
		std::cout << "\n  size of db = " << db.count() << "\n";
		std::vector<std::string> keys = db.keys();
		for (std::string key : keys)
		{
			std::cout << db.value(key).show();
		}
	}

}