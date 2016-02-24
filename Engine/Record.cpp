#include "Record.h"
#include <iostream>
Record::Record()
{
	_recordValues.clear();
}
Record::Record(vector<string> &newRecords)
{
	_recordValues = newRecords;
}
Record::~Record()
{
	_recordValues.clear();
}
bool Record::operator==(Record& rhsRecord)
{
	/****
		The operator==() is used in the conditionalParser() and recursiveDescent(). This function compares each 
		value inside the 2 records to each other and once it finds a mismatch, the function will return false, 
		otherwise return true.
	****/
	// this == lhsRecord
	vector<string> lhsVec = this->getRecord();
	vector<string> rhsVec = rhsRecord.getRecord();

	for(int i = 0; i < rhsVec.size(); i++)
	{
		string lhsValue = lhsVec[i];
		string rhsValue = rhsVec[i];
		if(lhsValue != rhsValue)
			return false;
	}
	return true;
}
void Record::setVector(vector<string> &newRecords)
{
	_recordValues = newRecords;
}
void Record::updateRecord(int vecStringIndex, string newValue)
{
	/****
		The updateRecord() is used with the relational algebra function updateTable(). 
		This function will simply overwrite the current value at the desired index.
	****/
	_recordValues[vecStringIndex] = newValue;
}