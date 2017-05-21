#pragma once
//////////////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Finds all the types and global functions defined in //
//                  each of a collection of C++ source files            //
// Ver 1.0                                                              //
// Platform:    Apple Mac Mini Late 2014, Win 10 Education,             //
//              Visual Studio 2015 Enterprise                           //
// Author:      Nagaprasad Natarajaurs, 418910728                       //
//              nnataraj@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* It is used to find all the types and global functions defined in
* each of a collection of C++ source files. It does this by building
* rules to detect: 
*       type definitions - classes, structs, enums, typedefs, & aliases. 
*       global function definitions
*       global data definitions
* and capture their fully qualified names and the files where they are
* defined. It uses the TypeTable (unordered map std c++ library) to
* store that information. 
*
* Required Files:
* ---------------
*   - ActionsAndRules.h, ActionsAndRules.cpp,
*     TypeAnalysis.h, TypeAnalysis.cpp
*
* Build Process:
* --------------
*   devenv TypeAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 1.0 : 13 Mar 2017
* - first release
*
*/

#pragma warning (disable: 4503)
#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <stack>
#include <functional>

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;

		TypeAnal();
		void doTypeAnal();
		void showTypeTable();
		std::unordered_map<std::string, std::vector<std::string>>& getGlobFuncMap();
		std::vector<std::string> returnDependentFiles(std::string token);
	private:
		void DFS(ASTNode* pNode);
		void getGlobalFunctionInfo(ASTNode * globalNode);
		void storeGlobalFunction(ASTNode* pNode);
		std::unordered_map<std::string, std::vector<std::unordered_map <std::string, std::string>>> typeTable;
		std::unordered_map<std::string, std::vector<std::string>> funcMap;
		std::stack<std::string> namespaceStack;
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
	};

	// Default constructor also initialises AST, ScopeStack and Toker
	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
		std::cout << "\n-----------------Requirements 3 & 4: Created TypeAnalysis package for Type Analysis-------------------\n\n";
		std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
	}                                              // It doesn't do anything useful for dep anal.

	//Checks if node is either of class,struct or enum
	inline bool doDisplay(ASTNode* pNode)
	{
		static std::string toDisplay[] = {
			"class","struct", "enum"
		};
		for (std::string type : toDisplay)
		{
			if (pNode->type_ == type)
				return true;
		}
		return false;
	}

	//Depth first search over the abstract syntax tree & builds type table
	inline void TypeAnal::DFS(ASTNode* pNode)
	{
		static std::string path = "";

		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}
		if (doDisplay(pNode))
		{
			std::unordered_map<std::string, std::string> mapElem;
			mapElem["type"] = pNode->type_;
			mapElem["filename"] = pNode->path_;
			mapElem["namespace"] = namespaceStack.top();
			std::vector<std::unordered_map<std::string, std::string>> initVect;
			if (typeTable.find(pNode->name_) == typeTable.end()) {
				initVect.push_back(mapElem);
			}
			else {
				initVect = typeTable[pNode->name_];
				initVect.push_back(mapElem);

			}
			typeTable[pNode->name_] = initVect;
		}
		if (pNode->type_ == "namespace") {
			namespaceStack.push(pNode->name_);
		}
		for (auto pChild : pNode->children_) {
			DFS(pChild);
		}
		if (pNode->type_ == "namespace") {
			namespaceStack.pop();
		}
	}

	//Start type analysis
	inline void TypeAnal::doTypeAnal()
	{
		std::cout << "\n  starting type analysis:\n";
		std::cout << "\n  scanning AST and displaying important things:";
		std::cout << "\n -----------------------------------------------";
		ASTNode* pRoot = ASTref_.root();
		DFS(pRoot);
		getGlobalFunctionInfo(Repository::getInstance()->getGlobalScope());
	}

	//Display the contents of the type table
	void TypeAnal::showTypeTable() {
		for (auto it : typeTable) {
			std::cout << std::endl;
			std::cout << it.first;
			std::cout << std::endl;
			std::cout << "----------------------------------" << std::endl;
			std::vector<std::unordered_map<std::string, std::string>> valueVect = it.second;
			for (std::vector<std::unordered_map <std::string, std::string>>::iterator it = valueVect.begin(); it != valueVect.end(); it++) {
				std::unordered_map <std::string, std::string> inst = *it;
				for (auto item : inst) {
					std::cout << std::setw(20) << item.first << std::setw(20) << item.second << std::endl;
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "-------------------------------------" << std::endl;
		}
	}

	//Returns list of all files from  type table
	//by checking to match the token against each entry in type table
	std::vector<std::string> TypeAnal::returnDependentFiles(std::string token) {
		std::vector<std::string> depFileVect;
		std::vector<std::string> emptyVect;
		if (typeTable.find(token) != typeTable.end()) {
			std::vector<std::unordered_map<std::string, std::string>> mapVect = typeTable[token];
			for (std::vector<std::unordered_map<std::string, std::string>>::iterator it = mapVect.begin(); it != mapVect.end(); it++) {
				std::unordered_map<std::string, std::string> mapForTok = *it;
				depFileVect.push_back(mapForTok["filename"]);
			}
		}
		else if (funcMap.find(token) != funcMap.end() && token != "main") {
			depFileVect = funcMap[token];
		}
		return depFileVect;
	}

	//Saves global function info
	void TypeAnal::storeGlobalFunction(ASTNode* pNode)
	{
		std::vector<std::string> filenames;
		if (funcMap.find(pNode->name_) == funcMap.end())
		{
			filenames.push_back(pNode->path_);
		}
		else
		{
			filenames = funcMap[pNode->name_];
			filenames.push_back(pNode->path_);
		}
		funcMap[pNode->name_] = filenames;
	}

	//Retrieve global function info
	void TypeAnal::getGlobalFunctionInfo(ASTNode * globalNode) {
		std::cout << "\n---------------------------------------\n";
		for (auto node : globalNode->children_) {
			if (node->type_ == "function") {
				if ((node->name_ != "main") && (node->name_ != "void"))
					storeGlobalFunction(node);
			}
		}
		std::cout << "\n---------------------------------------\n";
	}

	//Retrieve the function mappings
	std::unordered_map<std::string, std::vector<std::string>>& TypeAnal::getGlobFuncMap()
	{
		return funcMap;
	}


}