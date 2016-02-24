#include "Table.h"

Table::Table()
{
	_attributes.clear();
	_records.clear();
	
}
Table::Table(string tableName, vector<Attribute> &attributes, vector<Record> &records)
{
	_tableName = tableName;
	_attributes = attributes;
	_records = records;
	_query = false;
	
}
Table::Table(string tableName, vector<Attribute> &attributes, vector<Record> &records, bool query)
{
	_tableName = tableName;
	_attributes = attributes;
	_records = records;
	_query = true;
	
}
Table::~Table()
{
	_tableName = "";
	_attributes.clear();
	_records.clear();
}
string Table::addNewRecord(Record newRecord) 
{ 
	/****
		The addNewRecord() function adds a record to an existing table. We begin by checking if the values are added 
		are unique and pushing them back into a the vector if they are. In order to do this, we have to find which 
		attributes are keys then we use this index to check the record's vectors elements for redundancies. We then 
		have to check if the record being entered is larger than the max size allowed for an attribute. If the size 
		is appropriate, the record is added to the database.
	****/
// HERE IS WHERE WE CHECK FOR THE UNIQUE VALUES IF ALL IS O.K. WE PUSH_BACK
	bool flag = false;
	string error = "";
	//find out which Attributes are KEYS
	// funciton returns which columns are keys
	vector<int> keyIndex;
	vector<int> varcharIndex;
	for(int i = 0; i < _attributes.size(); i++)
	{
		bool attrKey = _attributes[i].getKey();
		if (attrKey)
			keyIndex.push_back(i);

		string attrType = _attributes[i].getType();
		size_t varcharFound = attrType.find("varchar");
		if(varcharFound != string::npos)
			varcharIndex.push_back(i);
	}

	for(int i = 0; i < _records.size(); i++)
	{
		vector<string> currRecord = _records[i].getRecord();
		// checking for unique values
		for(int j = 0; j < keyIndex.size(); j++)
		{
			int index = keyIndex[j];
			//compare record[index] to currRecord[index]
			vector<string> newRecordVec = newRecord.getRecord();
			if(currRecord[index] == newRecordVec[index])
			{
				flag = true;
				error = "\nERROR: Must enter a UNIQUE value for the Keys.\n";
			}
		}
	}

	for(int i = 0; i < varcharIndex.size(); i++){
		//cout << "varcharIndex: " << varcharIndex[i];
		int index = varcharIndex[i];
		//cout << "  maxsize: " << _attributes[index].getMaxSize() << endl;
		int currMaxSize = _attributes[index].getMaxSize();
		
		vector<string> newRecordVec = newRecord.getRecord();
		int newRecordSize = newRecordVec[index].size();

		//cout << "maxsize: " << currMaxSize << ", newRecordSize: " << newRecordSize << endl;
		if (currMaxSize < newRecordSize)
		{
			flag = true;
			error = "\nERROR: Cannot input a value larger the max size allowed for attribute.\n";
		}

	}



	if(flag)
	{
		return error;
	}
	else
	{
		_records.push_back(newRecord); 
		return "";
	}	
}

void Table::changeAttributeName(int index, string newAttribute)
{
	/****
		The changeAttributeName() function traverses the attribute vector to the specified index passed into the 
		argument. The value at the index is replaced with the new attribute passed in.
	****/
	//cout << "index:" << index << endl;
	_attributes[index].changeAttributeName(newAttribute);
}

void Table::eraseRecord(vector<int> &deleteIndeces){
	/****
		The eraseRecord() function deletes the record at the index that is passed into the argument.
	****/
	reverse(deleteIndeces.begin(), deleteIndeces.end());
	
	for(int i = 0; i < deleteIndeces.size(); i++)
	{
		int index = deleteIndeces[i];
		//cout << "index: " << index << endl;
		_records.erase(_records.begin()+index);
	}
}

void Table::updateRecord(vector<int> &updateIndeces, vector<string> &attrNames, vector<string> &literals)
{
	/****
		The updateRecord() function updates the index inside of our existing _records vector.
	****/
	vector<Attribute> attributes = _attributes;
	for(int k = 0; k < attributes.size(); k++)
	{
		for(int j = 0; j < attrNames.size(); j++)
		{
			if(attributes[k].getName() == attrNames[j])
			{
				for(int i = 0; i < updateIndeces.size(); i++)
				{
					//update::  updateIndeces[i] with literals[j]
					int index = updateIndeces[i];
					_records[index].updateRecord(k, literals[j]);
				}
				
			}
		}
	}
}