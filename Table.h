
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
	
    //getters
    string getTableName()const { return _tableName; }
    vector<Attribute> getTblAttributes() const { return _attributes; }
    vector<Record> getTblRecords() const { return _records; }
	bool getQuery() const { return _query; }

	//setters
	void changeAttributeName(int index, string newAttribute);
	void changeTableName(string newTableName) { _tableName = newTableName; }
	void eraseRecord(vector<int> &deleteIndeces);
	void updateRecord(vector<int> &updateIndeces, vector<string> &attrNames, vector<string> &literals);
	void updateSpecific(int recIndex, int attIndex, string literal) {_records[recIndex].updateRecord(attIndex, literal);}
	void copyRecord(int recIndex, Record rec) {_records[recIndex] = rec;}
	void setQuery(bool query) {_query = query;}
	
    //helper funcions
	string addNewRecord(Record newRecord);
	
	//Overload operator
	Table & operator=(const Table &rhs);
};


#endif