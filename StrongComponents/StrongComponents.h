#pragma once
//////////////////////////////////////////////////////////////////////////
// StrongComponents.h - Find is the largest set of files that are all   //
//                      mutually dependent                              //
// Ver 1.0                                                              //
// Platform:    Apple Mac Mini Late 2014, Win 10 Education,             //
//              Visual Studio 2015 Enterprise                           //
// Author:      Nagaprasad Natarajaurs, 418910728                       //
//              nnataraj@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* A strong component is the largest set of files that are all mutually
* dependent. That is, all the files which can be reached from any other
* file in the set by following direct or transitive dependency links.
* The term "Strong Component" comes from the theory of directed graphs.
* There are a number of algorithms for finding strong components in
* graphs. Favorite is the Tarjan Algorithm, nicely described here:
* "en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm"
*
* Required Files:
* ---------------
*   - StrongComponents.h, StrongComponents.cpp
*     DependencyAnalysis.h, DependencyAnalysis.cpp
*     XmlDocument.h, XmlDocument.cpp
*
* Build Process:
* --------------
*   devenv StrongComponents.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 1.0 : 13 Mar 2017
* - first release
*
*/

#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"

namespace CodeAnalysis
{
	class Graph
	{
	public:
		std::unordered_map<std::string, int> createGraph(DepAnal &, std::vector<std::string>);
		int createMap();
		void createAdjacencyMatrix(DepAnal &);
		std::list<int>* getAdjacencyMatrix();
		int getCount();
	private:
		int noOfVertices;
		std::vector<std::string> fileVect;
		std::unordered_map<std::string, int> mapOfFiles;
		std::list<int> *adjacency_matrix;
	};

	//Retrieves content from dependency analysis for graph creation
	std::unordered_map<std::string, int> Graph::createGraph(DepAnal &dp, std::vector<std::string> filesInLoc)
	{
		fileVect = filesInLoc;
		noOfVertices = createMap();
		adjacency_matrix = new std::list<int>[noOfVertices];
		createAdjacencyMatrix(dp);
		return mapOfFiles;
	}

	//Pairs each file in the dependancy table to a unique integer value
	int Graph::createMap()
	{
		int i = 0;
		for (std::string x : fileVect)
		{
			mapOfFiles[x] = i;
			i++;
		}
		return i;
	}

	//Make adjacency matrix to associate each node in the graph to its connected node
	void Graph::createAdjacencyMatrix(DepAnal &dp)
	{

		std::vector<std::string> files = dp.getDb().keys();
		for (std::string file : files)
		{
			std::vector<std::string> listFiles = dp.getDb().value(file).children.getValue();
			int index = mapOfFiles[file];
			for (std::string x : listFiles)
			{
				adjacency_matrix[index].push_back(mapOfFiles[x]);
			}
		}
	}

	//Retrieve adjacency matrix
	std::list<int>* Graph::getAdjacencyMatrix()
	{
		return adjacency_matrix;
	}

	//Retrieve the number of vertices in graph
	int Graph::getCount()
	{
		return noOfVertices;
	}

	class StrongComponents
	{
	public:
		using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		StrongComponents(std::string );
		void createGraph(DepAnal &, std::vector<std::string> );
		void strongCompRecursive(int x, int [], int [], std::stack<int> *, bool []);
		void findStrongComp();
		std::string fetchVal(int k);
		void showStrongComp();
		void  writeToXmLFile(std::string , std::string );
		std::string& getXmlFileContent();

	private:
		Graph graph;
		int noOfVertices;
		std::list<int>* adj_matrix;
		std::unordered_map<int, std::vector<std::string>> scc_matrix;
		std::unordered_map<std::string, int> nodeToIntMap;
		std::vector<std::string> tempVect;
		std::string writeSCCToXml();
		int tempVal = 0;
		std::string xmlPath;
		std::string xmlFileContent;
	};

	//SC constructor, also initializes xml path for writing strongly connected components to an XML file
	StrongComponents::StrongComponents(std::string xmlFilePath) :xmlPath(xmlFilePath)
	{
		std::cout << "\n-----------------Requirements 3 & 6: Created StrongComponents package for Analysis of StrongComponents-------------------\n\n";
	}

	//entry point to SC creation
	void StrongComponents::createGraph(DepAnal &dp, std::vector<std::string> filesInLoc)
	{
		nodeToIntMap = graph.createGraph(dp, filesInLoc);
		noOfVertices = graph.getCount();
		adj_matrix = graph.getAdjacencyMatrix();
		findStrongComp();
		std::string toFile = writeSCCToXml();
		writeToXmLFile(toFile, xmlPath);
	}

	//Iteraate through adjacency matrix & generate strongly connected component info
	void StrongComponents::findStrongComp()
	{
		try {
			int *nodeArr = new int[noOfVertices];
			int *strongCompLow = new int[noOfVertices];
			bool *strongCompFlag = new bool[noOfVertices];
			std::stack<int> *strongCompStack = new std::stack<int>();

			for (int i = 0; i < noOfVertices; i++)
			{
				nodeArr[i] = -1;
				strongCompLow[i] = -1;
				strongCompFlag[i] = false;
			}

			for (int i = 0; i < noOfVertices; i++)
				if (nodeArr[i] == -1)
					strongCompRecursive(i, nodeArr, strongCompLow, strongCompStack, strongCompFlag);
		}
		catch (std::exception &e) {
			std::cout << e.what() << "\n";
		}
	}

	//Store SCC to XML file
	void  StrongComponents::writeToXmLFile(std::string xmlData, std::string nameOfFile) {
		std::cout << "\n-----------------Requirement 7: Storing StrongComponents Info to file at: "<< nameOfFile <<"-------------------\n\n";
		try {
			std::ofstream ost(nameOfFile);
			if (ost.is_open()) {
				ost << xmlData;
				ost.close();
			}
			else
			{
				std::cout << "Unable to access file \n";
			}
		}
		catch (std::exception &e) {
			std::cout << e.what();
		}
	}

	//Returns XML content in string
	std::string & StrongComponents::getXmlFileContent()
	{
		return xmlFileContent;
	}

	//Recursively iterates through each node and identifies node to node associations (algorithm to find SC components)
	void StrongComponents::strongCompRecursive(int x, int strongCompArr[], int strongCompLow[], std::stack<int>* strongcCompStack, bool strongBool[])
	{
		try {
			static int time = 0;

			strongCompArr[x] = strongCompLow[x] = ++time;
			strongcCompStack->push(x);
			strongBool[x] = true;

			std::list<int>::iterator i;
			for (i = adj_matrix[x].begin(); i != adj_matrix[x].end(); ++i)
			{
				int v = *i;
				if (strongCompArr[v] == -1)
				{
					strongCompRecursive(v, strongCompArr, strongCompLow, strongcCompStack, strongBool);
					strongCompLow[x] = min(strongCompLow[x], strongCompLow[v]);
				}

				else if (strongBool[v] == true)
					strongCompLow[x] = min(strongCompLow[x], strongCompArr[v]);
			}

			int w = 0;
			if (strongCompLow[x] == strongCompArr[x])
			{
				while (strongcCompStack->top() != x)
				{
					w = (int)strongcCompStack->top();
					tempVect.push_back(fetchVal(w));
					//std::cout << w << " ";
					strongBool[w] = false;
					strongcCompStack->pop();
				}
				w = (int)strongcCompStack->top();
				tempVect.push_back(fetchVal(w));
				scc_matrix[tempVal] = tempVect;
				tempVal++;
				tempVect.clear();
				//std::cout << w << "\n";
				strongBool[w] = false;
				strongcCompStack->pop();
			}
		}
		catch (std::exception &e) {
			std::cout << e.what();
		}
	}

	//Retrieves the integer value associated with each file in the pair
	std::string StrongComponents::fetchVal(int k)
	{
		for (auto i : nodeToIntMap)
		{
			if (i.second == k)
				return i.first;
		}
		return  std::string();
	}

	//Shows SC components
	void StrongComponents::showStrongComp()
	{
		int i = 1;
		for (auto toD : scc_matrix)
		{
			for (auto xx : toD.second)
			{
				std::cout << "\n";
				std::cout << "Strongly Connected Component :" << i << "\n";
				std::cout << "---------------------------------\n";
				std::cout << xx << "\n";
				i++;
			}
			std::cout << "\n";
		}
	}

	//Convert scc_matrix which contains strongly connected components to XML File
	std::string StrongComponents::writeSCCToXml()
	{
		try {
			int i = 1;
			XmlProcessing::XmlDocument doc;
			SPtr pRoot = XmlProcessing::makeTaggedElement("StronglyConnectedComponents");
			doc.docElement() = pRoot;

			for (auto sc : scc_matrix)
			{
				SPtr subRoot = XmlProcessing::makeTaggedElement("StronglyConnectedComponent");
				SPtr name = XmlProcessing::makeTaggedElement("Name");
				SPtr nameText = XmlProcessing::makeTextElement("Component" + std::to_string(i));
				i++;
				name->addChild(nameText);
				subRoot->addChild(name);
				for (std::string file : sc.second)
				{
					SPtr fileElem = XmlProcessing::makeTaggedElement("FileName");
					SPtr pfileTextElem = XmlProcessing::makeTextElement(file);
					fileElem->addChild(pfileTextElem);
					//adding element to subroot
					subRoot->addChild(fileElem);
				}
				//add subroot tag to root tag
				pRoot->addChild(subRoot);
			}
			xmlFileContent = doc.toString();
		}
		catch (std::exception &e) {
			std::cout << e.what();
		}
		return xmlFileContent;
	}
}