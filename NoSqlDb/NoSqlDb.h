#pragma once
 //////////////////////////////////////////////////////////////////////////
 // Project #1 - Key/Value Database - a container of NoSqlDB nodes       //
 // Ver 1.0                                                              //
 // Application: Key/Value Database Pr#1, Spring 2017                    //
 // Platform:    Apple Mac Mini Late 2014, Win 10 Education,             //
 //              Visual Studio 2015 Enterprise                           //
 // Author:      Jim Fawcett, CST 4-187, 443-3948                        //
 //              jfawcett@twcny.rr.com                                   //
 //////////////////////////////////////////////////////////////////////////
 /*
 * Package Operations:
 * -------------------
 * Key of DB is string
 * Value of DB is Data(template type either string or int or  some other
 * collection of data captured in some, perhaps custom, data structure)
 * +Metdata(name, category, description, dateTime, children (related key of other entries))
 * 
 * support addition and deletion of key/value pairs
 *
 * support editing of values including the addition and/or deletion of relationships,
 * editing text metadata, and replacing an existing value's instance with a new instance
 * 
 * on command, persist database contents to an XML file & this process be reversible, e.g.,
 * the database can be restored or augmented3 from an existing XML file as well as write its
 * contents out to an XML file. 
 *
 * accept a positive number of writes (default =5 if location is specified) after which
 * the database contents are persisted
 * 
 * support queries for:
 * The value of a specified key.
 * The children of a specified key.
 * The set of all keys matching a specified pattern which defaults to all keys.
 * All keys that contain a specified string in their item name
 * All keys that contain a specified string in their category name
 * All keys that contain a specified string in their template data section when that makes sense.
 * All keys that contain values written within a specified time-date interval. If only one end
 * of the interval is provided shall take the present as the other end of the interval.
 * Each query accepts a list of keys and returns a collection of keys from the list that
 * match the query.
 *
 * Support regular expression matching for the query types described above
 *
 * support making the same kinds of queries, cited in the requirement above, on the set of keys
 * returned by an earlier query.
 *
 * support forming a union of the keys from two or more earlier queries.
 * 
 * Required Files:
 * ---------------
 *   - Convert.h, Convert.cpp,
 *     CppProperties.h, CppProperties.cpp
 *     XmlDocument.h, XmlDocument.cpp,
 *     XmlElement.h, XmlElement.cpp
 *
 * Build Process:
 * --------------
 *   devenv KeyValueDB.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * Ver 1.0 : 9 Feb 2017
 * - first release
 *
 */

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <iostream>
#include <memory>
#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>
#include "../Convert/Convert.h"
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

// trim on both ends
static inline std::string& trim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

static std::string dateTimeToString(time_t dateTime) {
	struct tm buf;
	char cStr[100];
	localtime_s(&buf, &dateTime);
	strftime(cStr, sizeof(cStr), "%Y-%m-%dT%X", &buf);
	std::string str(cStr);
	return str;
}

static time_t dateTimeFromString(std::string dateTime) {
	time_t time;
	struct tm buf;
	sscanf_s(dateTime.c_str(), "%d-%d-%dT%d:%d:%d", &buf.tm_year, &buf.tm_mon, &buf.tm_mday, &buf.tm_hour, &buf.tm_min, &buf.tm_sec);
	buf.tm_year -= 1900;
	buf.tm_mon -= 1;
	time = mktime(&buf);
	return time;
}


/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it needs to store child data, something for you to implement
//
template<typename Data>
class Element
{
public:
  using Name = std::string;
  using Category = std::string;
  using Description = std::string; //added
  using TimeDate = std::string;
  using Key = std::string;
  using Children = std::vector<Key>; //added

  Property<Name> name;            // metadata
  Property<Category> category;    // metadata
  Property<Description> description;    // metadata
  Property<TimeDate> timeDate;    // metadata
  Property<Children> children;    // metadata
  Property<Data> data;            // data

  std::string show();
};

template<typename Data>
std::string Element<Data>::show()
{
  // show children when you've implemented them

  std::ostringstream out;
  out.setf(std::ios::adjustfield, std::ios::left);
  out << "\n    " << std::setw(8) << "name"     << " : " << name;
  out << "\n    " << std::setw(8) << "category" << " : " << category;
  out << "\n    " << std::setw(8) << "description" << " : " << description;
  out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;

  std::vector<Key> _children = children;
  
  for (Key child : _children) {
	  out << "\n    " << std::setw(8) << "child" << " : " << child;
  }

  out << "\n    " << std::setw(8) << "data"     << " : " << data;
  out << "\n";
  return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
template<typename Data>
class NoSqlDb
{
public:
  using Key = std::string;
  using Keys = std::vector<Key>;
  using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;

  Keys keys();
  bool save(Key key, Element<Data> elem);
  Element<Data> value(Key key);
  size_t count();
  size_t erase(Key key);
  bool updateData(Key key, Data data);
  bool updateMetaData(Key key, std::string paramToUpdate, std::string newValue, bool isAddNotRemove);
  bool toXml(const std::string&);
  void fromXml(const std::string&);
  unsigned getNumOfWriteToSave() { return numOfWriteToSave; };
  void setNumOfWriteToSave(unsigned value) { numOfWriteToSave = value; };
  std::string getSaveLoc() { return saveLoc; };
  void setSaveLoc(std::string value) { saveLoc = value; };

  //Queries
  Keys unionOfKeys(Keys k1, Keys k2);
  Keys getAllChildren(Key key);
  Keys query(Keys keys,std::string queryPattern, std::string paramName);
  Keys queryRegEx(Keys _keys, std::string regExQueryPattern, std::string paramName);
  Keys queryTimeInterval(Keys _keys, std::string time1, std::string time2);
  Keys queryTimeInterval(Keys _keys, std::string time_t);

  //Default constructor
  NoSqlDb() {
  }

  //Constructor to overload the save location and number of writes before auto saving
  NoSqlDb(std::string fname, int writeNum) {
	  saveLoc = fname;
	  writeCount = writeNum;
  }

  // Uodate child values from the the given vector
  void updateChildVal(Key key, std::vector<std::string> vect) {
	  bool flag = false;
	  Element<Data> elem = value(key);
	  std::vector<std::string> childVect = elem.children;
	  for (std::string childVal : vect) {
		  for (std::string existChild : childVect) {
			  if (childVal == existChild)
				  flag = true;
		  }
		  if (flag == false)
			  childVect.push_back(childVal);
	  }
	  elem.children = childVect;
	  store[key] = elem;
  }

  ~NoSqlDb() {
	  if (saveLoc != "") {
		  toXml(saveLoc);
		  std::cout << "\n  Saved database to " << saveLoc << " as DB object went out of scope (destructor invoked) - requirement 6\n";
	  }
  }
private:
  using Item = std::pair<Key, Element<Data>>;
  unsigned numOfWriteToSave = 5; //default 5, reduce IO and reduce wear&tear
  void removeRelationships(Key key);
  int writeCount = 0;
  std::string saveLoc = "";
  void updateWriteCount() {
	  writeCount++;
	  if (writeCount == numOfWriteToSave) {
		  if (saveLoc != "") {
			  toXml(saveLoc);
			  std::cout << "\n  Saved database to "<< saveLoc <<" as reached write count limit of "<< numOfWriteToSave <<" - requirement 6\n";
		  }
		  writeCount = 0;
	  }
  }
  std::unordered_map<Key,Element<Data>> store;
};

// return all keys present in DB
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
  Keys keys;
  for (Item item : store)
  {
    keys.push_back(item.first);
  }
  return keys;
}

// save key & value i.e metadata+data
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
  if(store.find(key) != store.end())
    return false;

  elem.timeDate = dateTimeToString(time(NULL));

  store[key] = elem;
  updateWriteCount();
  return true;
}

// returns the entire value i.e metadata+data
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
  if (store.find(key) != store.end())
    return store[key];
  return Element<Data>();
}

// return size of DB
template<typename Data>
size_t NoSqlDb<Data>::count()
{
  return store.size();
}

// remove child relationship of the given key if present in any other
// key's metadata
template<typename Data>
void NoSqlDb<Data>::removeRelationships(Key key)
{
	for (auto i = store.begin(); i != store.end(); ++i) {
		Element<Data> currentData = i->second;
		std::vector<std::string> children = currentData.children;
		children.erase(std::remove(children.begin(), children.end(), key), children.end());
		currentData.children = children;
		i->second = currentData;
	}
}

// remove key
template<typename Data>
size_t NoSqlDb<Data>::erase(Key key)
{
	size_t deletedNum = store.erase(key);
	if (deletedNum > 0) {
		removeRelationships(key);
		updateWriteCount();
	}
	return deletedNum;
}

// update key with provided data
template<typename Data>
bool NoSqlDb<Data>::updateData(Key key, Data data) {
	if (store.find(key) != store.end()) {
		Element<Data> element = store[key];
		element.data = data;
		store[key] = element;
		updateWriteCount();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////
// Persistance class
// - you will need to implement that using the XmlDocument class
//   from Handouts/Repository

// serialise and store the DB by creating a corresponding XML
template<typename Data>
bool NoSqlDb<Data>::toXml(const std::string& xml) {
	sPtr pRoot = XmlProcessing::makeTaggedElement("Database");
	XmlProcessing::XmlDocument doc(XmlProcessing::makeDocElement(pRoot));

	for (auto i = store.begin(); i != store.end(); ++i) {
		sPtr pEntry = XmlProcessing::makeTaggedElement("Entry");
		Key currentKey = i->first;
		Element<Data> currentData = i->second;

		sPtr pKeyElement = XmlProcessing::makeTaggedElementNText("Key", currentKey);
		sPtr pNameElement = XmlProcessing::makeTaggedElementNText("Name", currentData.name);
		sPtr pCategoryElement = XmlProcessing::makeTaggedElementNText("Category", currentData.category);
		sPtr pDescriptionElement = XmlProcessing::makeTaggedElementNText("Description", currentData.description);
		sPtr pDateTimeElement = XmlProcessing::makeTaggedElementNText("DateTime", currentData.timeDate);

		std::vector<Key> children = currentData.children;
		sPtr pChildrenElement = XmlProcessing::makeTaggedElement("Children");
		for (Key child:children) {
			sPtr pChildElement = XmlProcessing::makeTaggedElementNText("Child", child);
			pChildrenElement->addChild(pChildElement);
		}

		sPtr pDataElement = XmlProcessing::makeTaggedElementNText("Data", Convert<Data>::toString(currentData.data));

		pEntry->addChild(pKeyElement);
		pEntry->addChild(pNameElement);
		pEntry->addChild(pCategoryElement);
		pEntry->addChild(pDescriptionElement);
		pEntry->addChild(pDateTimeElement);
		pEntry->addChild(pChildrenElement);
		pEntry->addChild(pDataElement);

		pRoot->addChild(pEntry);
	}
	std::ofstream xmlFile(xml);
	if (xmlFile.is_open())
		xmlFile << doc.toString();
	xmlFile.close();
	return true;
}

//Loads entries to DB from the given path of the XML file
template<typename Data>
void NoSqlDb<Data>::fromXml(const std::string& xml) {
	try
	{
		XmlProcessing::XmlDocument doc(xml, XmlProcessing::XmlDocument::file);
		//std::cout << doc.toString();

		std::vector<sPtr> entries = doc.descendents("Entry").select();
		for (sPtr entry : entries) {
			Element<Data> data;
			std::string key = trim(entry->children()[0]->children()[0]->value());
			data.name = trim(entry->children()[1]->children()[0]->value());
			data.category = trim(entry->children()[2]->children()[0]->value());
			data.description = trim(entry->children()[3]->children()[0]->value());
			data.timeDate = trim(entry->children()[4]->children()[0]->value());

			std::vector<sPtr> children = entry->children()[5]->children();
			std::vector<Key> _children;
			for (sPtr child:children) {
				_children.push_back(trim(child->children()[0]->value()));
			}
			data.children = _children;
			//data.data = entry->children()[6]->children()[0]->value();
			data.data = Convert<Data>::fromString(trim(entry->children()[6]->children()[0]->value()));

			if (store.find(key) != store.end())
				continue;

			store[key] = data;
		}
		updateWriteCount();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
	}

}
/*
Used to update matedata values including the addition and/or deletion of relationships
and replacing an existing value's instance with a new instance
Use paramToUpdate value as
 name to update  name metadata's value
 category to update  category metadata's value
 description to update  description metadata's value
 timeDate to update  timeDate metadata's value
 children to update  children metadata's value

 parameter isAddNotRemove is applicable when paramToUpdate is children
	if true add the newvalue to child
	if false remove the newvalue from child if present

 returns true if an update was made

*/
template<typename Data>
bool NoSqlDb<Data>::updateMetaData(Key key, std::string paramToUpdate, std::string newValue, bool isAddNotRemove) {
	if (store.find(key) != store.end()) {
		Element<Data> element = store[key];
		if (paramToUpdate == "name")
			element.name = newValue;
		else if (paramToUpdate == "category")
			element.category = newValue;
		else if (paramToUpdate == "description")
			element.description = newValue;
		else if (paramToUpdate == "timeDate")
			element.timeDate = newValue;
		else if (paramToUpdate == "children") {
			std::vector<Key> children = element.children;
			if (isAddNotRemove)
				children.push_back(newValue);
			else
				children.erase(std::remove(children.begin(), children.end(), newValue), children.end());
			element.children = children;
		} else
			return false;

		store[key] = element;
		updateWriteCount();
		return true;
	}
	return false;
}

// Returns the distinct and unique union of the provided keys k1,k2
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::unionOfKeys(Keys k1, Keys k2)
{
	std::unordered_set<Key> keys;
	Keys finalKeys;

	for (Key k : k1)
		keys.insert(k);
	for (Key k : k2)
		keys.insert(k);

	for (auto i : keys)
		finalKeys.push_back(i);

	return finalKeys;
}

// Returns the all children held the provided key's value
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::getAllChildren(Key key)
{
	return value(key).children;
}

/*
Supports querying on keys, value's name metadata, value's category metadata and on value's data based on the given pattern

Keys can provided to the functions to limit query on the provided keys only, if keys arn't provided
query for the pattern will be done on all keys present in DB

paramName specifics on whose value the pattern should be searched for

*/
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::query(Keys _keys, std::string queryPattern, std::string paramName)
{
	//if keys not provided query on all keys present in the database if else query on the provided keys only
	if (_keys.empty())
		_keys = keys();

	Keys results;

	if (paramName == "keys") {
		for (Key key : _keys)
			if (key.find(queryPattern) != std::string::npos)
				results.push_back(key);
	}
	else if (paramName == "name") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = currEle.name;
			if (data.find(queryPattern) != std::string::npos)
				results.push_back(key);
		}
	}
	else if (paramName == "category") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = currEle.category;
			if (data.find(queryPattern) != std::string::npos)
				results.push_back(key);
		}
	}
	else if (paramName == "data") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = Convert<Data>::toString(currEle.data);
			if (data.find(queryPattern) != std::string::npos)
				results.push_back(key);
		}
	}

	return results;
}

/*
Same as above function but pattern matching occurs using the regEx string provided
*/
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryRegEx(Keys _keys, std::string regExQueryPattern, std::string paramName)
{
	//if keys not provided query on all keys present in the database if else query on the provided keys only
	if (_keys.empty())
		_keys = keys();

	Keys results;
	std::regex pattern(regExQueryPattern);

	if (paramName == "keys") {
		for (Key key : _keys)
			if (std::regex_match(key, pattern))
				results.push_back(key);
	}
	else if (paramName == "name") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = currEle.name;
			if (std::regex_match(data, pattern))
				results.push_back(key);
		}
	}
	else if (paramName == "category") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = currEle.category;
			if (std::regex_match(data, pattern))
				results.push_back(key);
		}
	}
	else if (paramName == "data") {
		for (Key key : _keys) {
			Element<Data> currEle = value(key);
			std::string data = Convert<Data>::toString(currEle.data);
			if (std::regex_match(data, pattern))
				results.push_back(key);
		}
	}

	return results;
}

/*
 Returns all keys  that contain values written within a specified date time interval
 interval such that time2 > time1
*/
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryTimeInterval(Keys _keys, std::string time1, std::string time2)
{
	//if keys not provided query on all keys present in the database if else query on the provided keys only
	if (_keys.empty())
		_keys = keys();

	Keys results;

	for (Key key : _keys) {
		Element<Data> currEle = value(key);
		time_t data = dateTimeFromString(currEle.timeDate);
		if (data>=dateTimeFromString(time1) && data<= dateTimeFromString(time2))
			results.push_back(key);
	}

	return results;
}

/*
Returns all keys  that contain values written after specified date time till now
*/
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::queryTimeInterval(Keys _keys, std::string time_t)
{
	return queryTimeInterval(_keys,time_t, dateTimeToString(time(NULL)));
}