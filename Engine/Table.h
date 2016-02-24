
#ifndef _TABLE_H
#define _TABLE_H



#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "Record.h"
#include "Attribute.h"

using namespace std;

class Table
{
	string _tableName;
	bool _query;
	vector<Attribute> _attributes;
	vector<Record> _records;
	
public:
    
	Table();
	Table(string tableName, vector<Attribute> &attributes, vector<Record> &records);
	Table(string tableName, vector<Attribute> &attributes, vector<Record> &records, bool query);
	~Table();

	//void addRecord(Record &newRecord);
    //getters
    string getTableName() { return _tableName; }
    vector<Attribute> getTblAttributes() { return _attributes; }
    vector<Record> getTblRecords() { return _records; }
	bool getQuery() { return _query; }

	//setters
	void changeAttributeName(int index, string newAttribute);
	void changeTableName(string newTableName) { _tableName = newTableName; }
	void eraseRecord(vector<int> &deleteIndeces);
	void updateRecord(vector<int> &updateIndeces, vector<string> &attrNames, vector<string> &literals);
	
    //helper funcions
	string addNewRecord(Record newRecord);
};


#endif