/*
Team project 1 - Database.h

*/
#ifndef _DATABASE_H_
#define _DATABASE_H_


#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <stdlib.h>
#include "Table.h"
#include "Record.h"
#include "Attribute.h"

using namespace std;

const string DBFILEPATH = "DatabaseFiles/";
const bool relationalAlgebraBool = true;
extern int selectionTblCount;
extern int unionTblCount;
extern int intersectTblCount;
extern int projectTblCount;
extern int differenceTblCount;
extern int crossProductTblCount;
extern int NaturalJoinCounter;
extern int setDifferenceCounter;

class Database
{
	// members of the database
	vector<Table> _dbTables;
	
	//helper functions
	void getAttributesInFile(ifstream &inputFile, vector<Attribute> &attributes);
	void getRecordsInFile(ifstream &inputFile, vector<Record> &records);
	vector<string> readFromFile(stringstream& sstream);
	string addRecord(int index, Record &newRecord);
	bool isUnionCompatible(Table R, Table S);
	
public:
    
	Database();
	~Database();
	
	// functions
	void openTable(string tableName);
	void closeTable(string tableName);
	void writeTable(string tableName);
	void exit();
	void show(string tableName);
	void createTable(string tableName, vector<string> attributeList, vector<string> keyAttributeList);
	void insertToTable(string tableName, vector<string> literals);
	void updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals);
	void deleteFromTable(string tableName, string attrName, string op, vector<string> &comp);
	string conditionParser(string tableName, string condition);
	void conditionParser(string condition);
	string recursiveDescent(string tableName, string condition);
	
	// relational alebgra functions
	string selection(string tableName, string attName, string ops, vector<string> &constList);
	string renaming(string tableName, vector<string> names);
	string projection(string tableName, vector<string> projAttr);
	string setUnion(string tableRName, string tableSName);
	string setDifference(string tableRName, string tableSName);
	string crossProduct(string tableRName, string tableQName);
	string naturalJoin(string tableRName, string tableQName);
	string setIntersect(string tableRName, string tableSName);
	
	//getters
    int getTableIndex(vector<Table>& _dbTables, string tableName);
    Table getDBtable(int index) { return _dbTables[index]; }
};

#endif
