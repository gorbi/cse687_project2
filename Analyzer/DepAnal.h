#pragma once
/////////////////////////////////////////////////////////////////////////
// DepAnal.h - Demonstrate how to start developing dependency analyzer //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////
/*
 * You need to provide all the manual and maintenance informnation
 */

#include "../Parser/ActionsAndRules.h"
#include <iostream>
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
  private:
    void DFS(ASTNode* pNode);
    AbstrSynTree& ASTref_;
    ScopeStack<ASTNode*> scopeStack_;
    Scanner::Toker& toker_;
  };

  inline TypeAnal::TypeAnal() : 
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))
  {
    std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
  }                                              // It doesn't do anything useful for dep anal.

  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }
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
      std::cout << "\n  " << pNode->name_;
      std::cout << ", " << pNode->type_;
    }
    for (auto pChild : pNode->children_)
      DFS(pChild);
  }

  inline void TypeAnal::doTypeAnal()
  {
    std::cout << "\n  starting type analysis:\n";
    std::cout << "\n  scanning AST and displaying important things:";
    std::cout << "\n -----------------------------------------------";
    ASTNode* pRoot = ASTref_.root();
    DFS(pRoot);
    std::cout << "\n\n  the rest is for you to complete\n";
  }
}