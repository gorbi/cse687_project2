/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "NoSqlDb.h"
#include <iostream>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
	try {
		std::cout << "\n  Demonstrating Project 1 - KeyValueDatabase";
		std::cout << "\n =================================\n";

		std::cout << "\n  Creating and saving NoSqlDb elements with string data - requirement 2";
		std::cout << "\n -------------------------------------------------------\n";

		NoSqlDb<StrData> db;
		db.setNumOfWriteToSave(3);
		db.setSaveLoc("..\\mainstringDB.xml");
		std::cout << "\n  Set the database to save in mainstringDB.xml when write count limit of 3 is reached - requirement 6\n";

		Element<StrData> elem1;
		elem1.name = "elem1";
		elem1.category = "test";
		elem1.description = "desc";
		elem1.data = "StrDataOfelem1";

		db.save(elem1.name, elem1);
		std::cout << "\n  Saved data with key = elem1, name = elem1, category = test, description = desc, children = none and data = StrDataOfelem1 - requirement 2 & 3";

		Element<StrData> elem2;
		elem2.name = "elem2";
		elem2.category = "test";
		elem2.description = "desc";
		elem2.data = "StrDataOfelem2";

		db.save(elem2.name, elem2);

		Element<StrData> elem4;
		elem4.name = "elem4";
		elem4.category = "test";
		elem4.description = "desc";
		elem4.data = "StrDataOfelem4";

		db.save(elem4.name, elem4);

		Element<StrData> elem3;
		elem3.name = "elem3";
		elem3.category = "test";
		elem3.description = "desc";
		elem3.children = { elem1.name, elem2.name };
		elem3.data = "StrDataOfelem3";

		db.save(elem3.name, elem3);
		std::cout << "\n  Saved data with key = elem3, name = elem3, category = test, description = desc, children = elem1.name & elem2.name and data = StrDataOfelem1 - requirement 2\n";

		db.erase(elem2.name);
		std::cout << "\n  Deleted entry where key = elem2. Since elem3 had elem2 as one of its child, elem3 has been updated and it has no elem2 as its child - requirement 3\n";


		Element<StrData> elem4_new;
		elem4_new.name = "elem4_new";
		elem4_new.category = "test_new";
		elem4_new.data = "StrDataOfelem4_new";

		db.updateData(elem4.name, elem4_new.data); std::cout << "\n  Updated data of elem4 to StrDataOfelem4_new from StrDataOfelem4 - requirement 4\n";
		db.updateMetaData(elem4.name, "name", elem4_new.name, false); std::cout << "\n  Updated name metadata of elem4 to elem4_new from elem4 - requirement 4\n";
		db.updateMetaData(elem4.name, "category", elem4_new.category, false); std::cout << "\n  Updated category metadata of elem4 to test_new from test - requirement 4\n";
		db.updateMetaData(elem4.name, "description", "desc_new", false); std::cout << "\n  Updated description metadata of elem4 to desc_new from desc using literal string - requirement 4\n";
		db.updateMetaData(elem4.name, "timeDate", "1970-01-01T00:01:00", false); std::cout << "\n  Updated timeDate metadata of elem4 to 1970-01-01T00:01:00 using literal string - requirement 4\n";

		db.updateMetaData(elem4.name, "children", elem1.name, true); std::cout << "\n  Updated children metadata of elem4 i.e added elem1 as its child - requirement 4\n";
		db.updateMetaData(elem3.name, "children", elem4.name, true); std::cout << "\n  Updated children metadata of elem3 i.e added elem4 as its child - requirement 4\n";
		db.updateMetaData(elem3.name, "children", elem1.name, false); std::cout << "\n  Updated children metadata of elem3 i.e removed elem1 as its child - requirement 4\n";

		Element<StrData> result = db.value(elem4.name); std::cout << "\n  Queried for elem4's value using its key - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		std::cout << result.show() << "\n";

		std::vector<std::string> k; //empty string vector
		k = db.query(k, elem4.name, "keys"); std::cout << "\n  Queried for keys which has pattern = elem4.name as key (answer should be just elem4) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : k)
			std::cout << std::setw(8) << v << "\n";

		std::vector<std::string> r1; //empty string vector
		r1 = db.query(r1, "test", "category"); std::cout << "\n  Queried for keys which has pattern = test as category (answer should be all elems) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : r1)
			std::cout << std::setw(8) << v << "\n";

		std::cout << "\n  both results from the previous two query has elem4 in their results, lets do union (answer should have only one instance of elem4 key) - requirement 9\n";
		r1 = db.unionOfKeys(r1, k);
		std::cout << "\n  results from the union - requirement 9\n";
		for (std::string v : r1)
			std::cout << std::setw(8) << v << "\n";

		db.updateMetaData(elem4.name, "name", "none", false); std::cout << "\n  Updated name metadata of elem4 to none from elem4_new using literal string - requirement 4\n";

		std::vector<std::string> r2; //empty string vector
		r2 = db.query(r2, "elem", "name"); std::cout << "\n  Queried for keys which has pattern = elem as name (answer should be elem1 & elem3) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : r2)
			std::cout << std::setw(8) << v << "\n";

		//std::vector<std::string> r3; //empty string vector
		r2 = db.query(r2, "test", "category"); ; std::cout << "\n  Queried for keys which has pattern = test as category only the results set returned by previous query(answer should be elem1 & elem3, wrong if query run on all keys in DB answer should be all keys in DB) - requirement 8\n";
		std::cout << "\n  results from the query - requirement 8\n";
		for (std::string v : r2)
			std::cout << std::setw(8) << v << "\n";

		std::vector<std::string> r4; //empty string vector
		r4 = db.queryRegEx(r4, ".*_.*", "category"); std::cout << "\n  Queried for keys which matches regular expression pattern = .*_.* in category (answer should be elem4) - requirement 12\n";
		std::cout << "\n  results from the query - requirement 12\n";
		for (std::string v : r4)
			std::cout << std::setw(8) << v << "\n";

		std::vector<std::string> r5; //empty string vector
		r5 = db.query(r5, "StrDataOfelem5_new", "data"); std::cout << "\n  Queried for keys which has pattern = StrDataOfelem5_new in data (answer should be none or empty) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : r5)
			std::cout << std::setw(8) << v << "\n";

		std::vector<std::string> r6; //empty string vector
		r6 = db.queryTimeInterval(r6, "2017-02-09T19:53:32"); std::cout << "\n  Queried for keys which contain values written after 2017-02-09T19:53:32 (answer should be elem1 & elem3) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : r6)
			std::cout << std::setw(8) << v << "\n";

		std::vector<std::string> r7; //empty string vector
		r7 = db.queryTimeInterval(r7, "1970-01-01T00:00:00", "2017-02-09T19:53:32"); std::cout << "\n  Queried for keys which contain values written between 1970-01-01T00:00:00 & 2017-02-09T19:53:32 (answer should be elem4) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : r7)
			std::cout << std::setw(8) << v << "\n";

		db.fromXml("..\\contents.xml"); std::cout << "\n  loaded the entries to it from contents.xml file - requirement 10\n";

		db.toXml("..\\database1.xml");
		std::cout << "\n  Saved the entire database to database1.xml file - requirement 5\n";

		{ // wrote in a block so that db1 goes out of scope & memory is cleared when block exits
			NoSqlDb<StrData> db1;
			db1.fromXml("..\\database1.xml"); std::cout << "\n  Created new database & loaded the entries to it from database1.xml file - requirement 5\n";
			db1.toXml("..\\database2.xml"); std::cout << "\n  Saved the new database to database2.xml, so database1.xml and database2.xml should be identical\n";
		}

		std::cout << "\n  Retrieving elements from NoSqlDb<string>";
		std::cout << "\n ------------------------------------------\n";

		std::cout << "\n  size of db = " << db.count() << "\n";
		Keys keys = db.keys();
		for (Key key : keys)
		{
			std::cout << "\n  " << key << ":";
			std::cout << db.value(key).show();
		}
		std::cout << "\n\n";

		std::cout << "\n  Creating and saving NoSqlDb elements with int data - requirement 2";
		std::cout << "\n ----------------------------------------------------\n";

		NoSqlDb<intData> idb;

		Element<intData> ielem1;
		ielem1.name = "elem1";
		ielem1.category = "test";
		ielem1.description = "desc";
		ielem1.data = 1;

		idb.save(ielem1.name, ielem1);

		Element<intData> ielem2;
		ielem2.name = "elem2";
		ielem2.category = "test";
		ielem2.description = "desc";
		ielem2.children = { ielem1.name };
		ielem2.data = 2;

		idb.save(ielem2.name, ielem2);

		Element<intData> ielem3;
		ielem3.name = "elem3";
		ielem3.category = "test";
		ielem3.description = "desc";
		ielem3.data = 3;

		idb.save(ielem3.name, ielem3);

		Element<intData> ielem4;
		ielem4.name = "elem4";
		ielem4.category = "test";
		ielem4.description = "desc";
		ielem4.children = { ielem2.name, ielem3.name };
		ielem4.data = 4;

		idb.save(ielem4.name, ielem4);

		idb.updateData(ielem4.name, 5); std::cout << "\n  Updated data of ielem4 to 5 from 4 using literal int - requirement 4\n";
		idb.updateData(ielem3.name, ielem4.data); std::cout << "\n  Updated data of ielem3 to 4 from 3 ielem4's data - requirement 4\n";

		Keys children = idb.getAllChildren(ielem4.name); std::cout << "\n  Queried for all ielem4's children using its key - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (Key child : children) {
			std::cout << "\n    " << std::setw(8) << "child" << " : " << child;
		}
		std::cout << "\n\n";

		std::vector<std::string> rn; //empty string vector
		rn = idb.query(rn, "2", "data"); std::cout << "\n  Queried for keys which has pattern = 2 in data (answer should be elem2) - requirement 7\n";
		std::cout << "\n  results from the query - requirement 7\n";
		for (std::string v : rn)
			std::cout << std::setw(8) << v << "\n";

		idb.toXml("..\\database3.xml");
		std::cout << "\n  Saved the entire int data database to database3.xml file - requirement 5\n";

		{ // wrote in a block so that idb1 goes out of scope & memory is cleared when block exits
			NoSqlDb<intData> idb1;
			idb1.fromXml("..\\database3.xml"); std::cout << "\n  Created new int data database & loaded the entries to it from database3.xml file - requirement 5\n";
			idb1.toXml("..\\database4.xml"); std::cout << "\n  Saved the new int data database to database4.xml, so database3.xml and database4.xml should be identical\n";
		}

		std::cout << "\n  Retrieving elements from NoSqlDb<int>";
		std::cout << "\n ---------------------------------------\n";

		std::cout << "\n  size of idb = " << idb.count() << "\n";
		Keys ikeys = idb.keys();
		for (Key key : ikeys)
		{
			std::cout << "\n  " << key << ":";
			std::cout << idb.value(key).show();
		}
		std::cout << "\n\n";
	}
	catch (const std::exception& ex) {
		std::cout << "\n\n  Caught Exception: " << ex.what() << "\n\n";
	}
	catch (...) {
		std::cout << "\n\n  Caught Nasty Exception\n\n";
	}
}