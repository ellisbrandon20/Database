/*
	Database.cpp
	ABV- Database Project
	CSCE 315
	This file contains the methods that operate the database functionality
*/
#include <cstdlib>
#include "Database.h"
int selectionTblCount = 0;
int unionTblCount = 0;
int intersectTblCount = 0;
int projectTblCount = 0;
int differenceTblCount = 0;
int crossProductTblCount = 0;
int NaturalJoinCounter = 0;
int setDifferenceCounter = 0;

Database::Database()
{
    _dbTables.clear();
	// create a folder to store Database files
	// -p = wont create folder if it already exist
	system("mkdir -p DatabaseFiles");
}
Database::~Database()
{
    _dbTables.clear();
}


//functions
/****
		The openTable() function accepts a name of a table as an argument to open a .db file that has 
		the information stored in it. From the file, the attributes and records are copied into separate 
		vectors associated with their class type. The table is then added into a vector of type Table 
		that stores every Table that is opened through the openTable function.
	****/
void Database::openTable(string tableName)
{   
	ifstream inputFile;
	string file = DBFILEPATH + tableName + ".db";
	inputFile.open(file.c_str());
	
    // check that the file exists in the database
	if(!inputFile)
    {
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
    }
    else
    {
		cout<< "Opened file " << file << endl;
    	// load information from file into vectors
    	vector<Attribute> attributes;
    	getAttributesInFile(inputFile, attributes);
    	vector<Record> records;
        getRecordsInFile(inputFile, records);
    	Table tmpTable(tableName, attributes, records);
    	_dbTables.push_back(tmpTable);        
    }
	inputFile.close();
}
/****
		The closeTable() function accepts a name of a table as an argument and begins by calling the 
		getTableIndex function which returns the position of the table that is passed within the 
		vector _dbTables. For error checking purposes, getTableIndex returns -1 if a table index 
		is not found so through if statements, we check if the index is less than 0 in order to 
		conclude if the table exists or not. If the index does exist, we then check if the table 
		is a query and proceed to delete the table from _dbTables because query results should not 
		be saved into the file. However if the table is not a query result, we proceed to save all 
		changes in the table's associated .db file.
	****/
void Database::closeTable(string tableName)
{
    int tblIndex = getTableIndex(_dbTables, tableName);
    if(tblIndex < 0)
    {
        cerr << "ERROR: Couldn't open file. Table does not exist." << tableName << ".db" << endl;
    }
    else
    { 
		Table tempTable = _dbTables[tblIndex];	
		if(tempTable.getQuery() == true)
		{
			// delete table if query == true			
			_dbTables.erase(_dbTables.begin()+tblIndex);
		}
		else
		{	
			// if not a query result, save all changes to relations
			writeTable(tableName);
			_dbTables.erase(_dbTables.begin()+tblIndex);
		}
    }   
}
/****
		The writeTable() function also checks if the table exists similar to the functions 
		mentioned above. This function traverses the user's input that is held within a vector 
		that holds the individual records. Since our output format is separated by a '|' symbol 
		to differentiate the records, we check that the user has not accidentally added a '|' 
		symbol in their input that could cause the program to count one record as two separate 
		records. Our solution to this problem was to add the '\' character whenever the input
		contained the '|' character that should not have been there. This lets our program know 
		that having "|" in an input is user error and should not be separated at that spot.
****/
void Database::writeTable(string tableName)
{
	ofstream outputFile(DBFILEPATH+tableName + ".db", ofstream::trunc);
	
    int tblIndex = getTableIndex(_dbTables, tableName);   
    if(tblIndex < 0)
    {
        cerr << "ERROR: Couldn't find file " << tableName << ".db in the database" << endl;
    }
    else
    {
		string file = DBFILEPATH + tableName + ".db";
        if(!outputFile)
		{

			cerr<<"Problem creating file"<<endl;
			
		}
		// temporary table object
        Table tempTable = _dbTables[tblIndex];
        vector<Attribute> tblAttributes = tempTable.getTblAttributes();
        // Checking for "|" 
        for(int i = 0; i < tblAttributes.size(); i++)
        {
            string attrName  = tblAttributes[i].getName();
            size_t pos = attrName.find('|');
            if(pos != string::npos)
                attrName.insert(pos, "\\");
			
			if(tblAttributes[i].getKey())
				attrName.insert(0, "*");
			
            string printVal = attrName + "[" + tblAttributes[i].getType() + "]";
            outputFile << "|" << printVal << "|";
        } 
        vector<Record> tblRecords = tempTable.getTblRecords();
        for(int i = 0; i < tblRecords.size(); i++)
        {
            outputFile << endl;
            vector<string> currRecord = tblRecords[i].getRecord();
            for(int j = 0; j < currRecord.size(); j++)
            {   
                string recordStr  = currRecord[j];
                size_t pos = recordStr.find('|');
                if(pos != string::npos)
                    recordStr.insert(pos, "\\");
                    
                outputFile << "|" << recordStr << "|";
            } 
        }
    } 
    outputFile.close();
}
/****
		The show() function prints out a formatted table. Through for loops, 
		the table's attributes and records are printed as they are iterated 
		through. We call setw() to make the output neat and presentable.
****/
/****
		BONUSSSSSSS:
        
		right now this doesn't print information in PRETTY format but if 
		it prints if we can find pretty way to print could get bonus points
****/
void Database::show(string tableName)
{
    int tblIndex = getTableIndex(_dbTables, tableName);
    
    if (tblIndex < 0)
    {
        cerr << "\nERROR: Can't show table, " << tableName << ", does not exist in database" 
             << endl; 
    }
    else
    {
        cout << "TABLE: " << tableName << endl;
        // print header/Attributes (columns for the table)
        vector<Attribute> tblAttributes = _dbTables[tblIndex].getTblAttributes();  
        for(int i = 0; i < tblAttributes.size(); i++)
        {
            string attributeName = tblAttributes[i].getName();
            string attributeType = tblAttributes[i].getType();
			// check if attribute is a key
			if(tblAttributes[i].getKey())
				attributeName.insert(0,"*");
            string attribute = attributeName + "[" + attributeType + "]";
            cout << setw(25) << left << attribute;
        }
        cout << '\n';     
        // print the records values
        vector<Record> tblRecords = _dbTables[tblIndex].getTblRecords();
        for(int i = 0; i < tblRecords.size(); i++)
        {
            vector<string> currRecord = tblRecords[i].getRecord(); 
            for(int j = 0; j < currRecord.size(); j++)
                cout << setw(25) << left << currRecord[j];
            cout << '\n';
        }
    }
     
}
/****
	The createTable() function needs the name of the table, the vector of attributes, and the keys associated with 
	it as arguments. The program iterates through the attributes vector looking for the '*' symbol. This indicates 
	that the attribute is a key. We then check the type of the attribute to see if its a varchar. If it is, find the 
	int value associated with it and then push that into a new attribute vector that only contains keys. This attribute 
	vector containing keys is the one used to create the table.
****/
void Database::createTable(string tableName, vector<string> attributeList, vector<string> keyAttributeList)
{
    int tblIndex = getTableIndex(_dbTables, tableName);
    if(tblIndex < 0)
    {
        string attributeName = "";
        string attributeType = "";
        int maxSize = -1;
        bool key = false;
        vector<Attribute> attributes;
        for(int i = 0; i < attributeList.size(); i++)
        {
			key = false;
            stringstream sstream(attributeList[i]);
			getline(sstream, attributeName, '[');
            getline(sstream, attributeType, ']');
			for(int x = 0; x < keyAttributeList.size(); x++)
			{
				string keyName = keyAttributeList[x];
				if(keyName == attributeName)
				{
					key = true;
					break;
				}
			}
            // If the Type is a VARCHAR need to find the value of the INT 
            if(attributeType.at(0) == 'v' || attributeType.at(0) == 'V' )
            {
                unsigned first = attributeType.find("("); 
                unsigned last = attributeType.find(")");
                string num = attributeType.substr(first+1,last-(first+1));
                string::size_type sz; // alias of size_t (allows me to convert to decimal int)
                maxSize = stoi(num,&sz);
            }
            Attribute tmpAttribute(attributeName, attributeType, maxSize, key);
            attributes.push_back(tmpAttribute);
        }
        // create the table
        vector<Record> empty; 
        Table newTable(tableName, attributes, empty);
		//cout<<"Inserting new table tableName"<<endl;
        _dbTables.push_back(newTable);
    }
    else{
        cerr << "\nERROR: The table " << tableName << " already exists." << endl;
    }
}
void Database::exit()
{
	for(int x = 0; x<_dbTables.size(); x++)
	{
		if(!_dbTables[x].getQuery())
			writeTable(_dbTables[x].getTableName());
	}
}
/****
	The insertToTable() function checks if the table passed exists and also checks if the size of 
	the Tuple being inserted is the same size as existing Tuples. We then call the addRecord() function
	which inserts the new record into the table specified by the index given.
****/
/**** 
 - literals is the values to be added.
 - we will not be accepting null values for our DB so we check that there are enough values to be storing in the DB.
 - the parser will be doing the type checking. 
 - may need to put the type checking in here but not sure quiet yet.
****/
void Database::insertToTable(string tableName, vector<string> literals)
{
    int tblIndex = getTableIndex(_dbTables, tableName);
    if(tblIndex < 0)
    {
        cerr << "\nERROR: Can't insert records becuase the table, " << tableName 
             << ", does not exist in database" << endl; 
    }
    int tblAttributesSize = _dbTables[tblIndex].getTblAttributes().size();
    if(literals.size() < tblAttributesSize)
    {
        cerr << "\nERROR: Missing information in Tuple cannot insert Record." << endl;
    }
    else if(literals.size() > tblAttributesSize)
    {
        cerr << "\nERROR: Extra information in Tuple cannot insert Record." << endl;
    }
    else
    {
        Record tmpRecord(literals);
        // this function calls addNewRecord for the desired table indicated by tblIndex
        string flag = addRecordToTable(tblIndex, tmpRecord);

        if (flag != "")
            cerr << flag;
    }
}
/****
	The updateTable() function has been optimized for use of data passed by the parser. 
	It now receives a table of unselected records, a table of selected records, and a 
	vector of changes. It then applies the changes to the selected records table and unions
	it with the unselected records. It then drops the original table and renames the final 
	table to the name of the original table.
****/
string Database::updateTable(string masterTable,string selectedTable, vector<string> &attrNames, vector<string> &literals)
{
	int masterTblIndex = getTableIndex(_dbTables, masterTable);
	if (masterTblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << masterTable << ".db, because it does not exist" << endl;
	}
	else
	{
		Table* master = getDBtable(masterTblIndex);
		int selectedTblIndex = getTableIndex(_dbTables, selectedTable);
		if (selectedTblIndex < 0)
		{
			cerr << "\nERROR: Can't open " << selectedTable << ".db, because it does not exist" << endl;
		}
		else
		{
			Table* selected = getDBtable(selectedTblIndex);
			for(int x = 0; x<attrNames.size(); x++)
			{
				for(int y = 0; y<selected->getTblAttributes().size(); y++)
				{
					if(selected->getTblAttributes()[y].getName() == attrNames[x])
					{
						for(int z = 0; z<selected->getTblRecords().size(); z++)
						{
							selected->updateSpecific(z, y, literals[x]);
						}
					}
				}
			}
			selectedTable = setUnion(selectedTable, masterTable);
			return selectedTable;
		}
	}
}
/****
	The updateTable() function modifies a table based on given criteria. If a row matches the arguments passed along with 
	the operations applied to it, updateTable() allows records of that row to be changed. The function first searches for 
	the column that matches the attribute name of the record. If that attribute name exists, the name of the record is 
	searched for and, if found, checks if the comparison on the test conditions evaluate to true. If true, the records 
	that were searched for are then updated to their new values.
****/
void Database::updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals)
{
	int tblIndex = getTableIndex( _dbTables, tableName);
	
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
	}
	else
	{
		vector<Attribute> currAttributes = _dbTables[tblIndex].getTblAttributes();
		int col = -1;
		bool stoiFLAG = false;
		//find out what column has the attrName
		for(int i = 0; i < currAttributes.size(); i++)			
		{
			string vecAttrName = currAttributes[i].getName();
			string vecAttrType = currAttributes[i].getType();
			if(vecAttrName == attrName)
			{
				col = i;
				if(vecAttrType == "integer")
					stoiFLAG = true;
			}
		}
		if(col == -1)
		{
			cerr << "\nERROR: attribute name does not exist:: Union" << endl;
		}
		else
		{
			vector<int> updateIndeces;
			vector<Record> currRecords = _dbTables[tblIndex].getTblRecords();
			for(int i = 0; i < currRecords.size(); i++) //iterate through tbl of Records
			{
				vector<string> currRecordsString = currRecords[i].getRecord();
				string toCompare = currRecordsString[col];
				bool skip = false;
				bool UpdateRecord = false;
				
				if(op == "==")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare == intComp)
								UpdateRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							if(toCompare == comp[k]){
								UpdateRecord = true;
								cout << "true"<<endl;
							}
						}
					}
					if(UpdateRecord )
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
				else if(op == "!=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare != intComp)
								UpdateRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							if(toCompare != comp[k])
								UpdateRecord = true;
							else
								skip = true;
						}
					}
					if(UpdateRecord && !skip)
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
				else if(op == "<")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare < intComp)
							{
								UpdateRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!UpdateRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '<' when comparing strings" << endl;
						}
					}
					if(UpdateRecord && !skip)
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
				else if(op == ">")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							if(intToCompare > intComp)
							{
								UpdateRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!UpdateRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '>' when comparing strings" << endl;
						}
					}
					if(UpdateRecord && !skip)
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
				else if(op == "<=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare <= intComp)
							{
								UpdateRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!UpdateRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '<=' when comparing strings" << endl;
						}
					}
					if(UpdateRecord && !skip)
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
				else if(op == ">=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare >= intComp)
							{
								UpdateRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!UpdateRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '>=' when comparing strings" << endl;
						}
					}
					if(UpdateRecord && !skip)
					{
						updateIndeces.push_back(i); // record to be editted
					}
				}
			}
			_dbTables[tblIndex].updateRecord(updateIndeces, attrNames, literals);
		}
	}
}
/****
	The dropTable() function deletes a table from the _dbTables vector.
****/
void Database::dropTable(string tableName)
{
	 _dbTables.erase(_dbTables.begin()+getTableIndex(tableName));
}
/****
	The deleteFromTable() function looks within the vector of attributes to find which column (index for _recordValues)
	in the table that is needed to be checked for each Record. The program checks if the attribute name exists and 
	proceeds to check what comparison operator is being passed in order to determine which attributes needs to be 
	deleted from the table if the comparison evaluates to true. This is done through many if statements for each 
	operator possible. Since strings can only be compared with "==" and "!=", the program outputs an error if an 
	attempt is made to compare a string with the other comparison operators. Once the comparisons are done, the 
	eraseRecord() function is called and deletes desired values that satisfied the condition.
****/
void Database::deleteFromTable(string tableName, string attrName, string op, vector<string> &comp)
{	
	int tblIndex = getTableIndex( _dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
	}
	else
	{
		vector<Attribute> currAttributes = _dbTables[tblIndex].getTblAttributes();
		int col = -1;
		bool stoiFLAG = false;	
		//find out what column has the attrName
		for(int i = 0; i < currAttributes.size(); i++)			
		{
			string vecAttrName = currAttributes[i].getName();
			string vecAttrType = currAttributes[i].getType();
			if(vecAttrName == attrName)
			{
				col = i;
				if(vecAttrType == "integer")
					stoiFLAG = true;
			}
		}
		if(col == -1)
		{
			cerr << "\nERROR: attribute name does not exist:: deleteFromTable" << endl;
		}
		else
		{
			vector<int> deleteIndeces;
			vector<Record> newRecords;
			vector<Record> currRecords = _dbTables[tblIndex].getTblRecords();
			for(int i = 0; i < currRecords.size(); i++)
			{
				vector<string> currRecordsString = currRecords[i].getRecord();
				string toCompare = currRecordsString[col];
				bool skip = false;
				bool deleteRecord = false;
				
				if(op == "==")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare == intComp)
								deleteRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							if(toCompare == comp[k])
							{
								deleteRecord = true;
							}
						}
					}
					if(deleteRecord )
						deleteIndeces.push_back(i);
				}
				else if(op == "!=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare != intComp)
								deleteRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							//cout << toCompare << " != " << comp[k] << endl;
							if(toCompare != comp[k]){
								deleteRecord = true;
								//cout <<"true"<<endl;
							}
							else
								skip = true;
						}
					}
					if(deleteRecord && !skip)
						deleteIndeces.push_back(i);
				}
				else if(op == "<")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare < intComp)
							{
								deleteRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!deleteRecord)
									skip = true;
							}
							
						}
						else
						{
							cerr << "ERROR: Can't use op '<' when comparing strings" << endl;
						}
					}
					if(deleteRecord && !skip)
						deleteIndeces.push_back(i);
				}
				else if(op == ">")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare > intComp)
							{
								deleteRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!deleteRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '>' when comparing strings" << endl;
						}
					}
					if(deleteRecord && !skip)
						deleteIndeces.push_back(i);
				}
				else if(op == "<=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare <= intComp)
							{
								deleteRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!deleteRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '<=' when comparing strings" << endl;
						}
					}
					if(deleteRecord && !skip)
						deleteIndeces.push_back(i);
				}
				else if(op == ">=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare >= intComp)
							{
								deleteRecord = true;
								if(skip)
									skip = false;
							}
							else
							{
								if(!deleteRecord)
									skip = true;
							}
						}
						else
						{
							cerr << "ERROR: Can't use op '>=' when comparing strings" << endl;
						}
					}
					if(deleteRecord && !skip)
						deleteIndeces.push_back(i);
				}
			}
			_dbTables[tblIndex].eraseRecord(deleteIndeces);
		}
	}
}
//======================== Relational Algebra functions =========================
/****
	The selection() function begins by searching for the column that contains the name of the attribute that was 
	passed to the function. When the appropriate column is found, we look at what comparison operator was called 
	and proceed to select all the tuples in the relation that satisfy the condition. For every instance where the 
	Tuple satisfies the condition, it is pushed into a new vector of Records which is then passed into a table 
	constructor to create a new table of only those Tuples.
****/
string Database::selection(string tableName, string attrName, string op, vector<string> &comp)
{
	
	bool isLiteral = true;
	int tblIndex = getTableIndex( _dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nSelection ERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		vector<Attribute> currAttributes = _dbTables[tblIndex].getTblAttributes();
		int col = -1;
		int col2 = -1;
		bool stoiFLAG = false;
		string vecAttrName;
		string vecAttrType;
		string vecAttrName2;
		string vecAttrType2;
		//find out what column has the attrName
		for(int i = 0; i < currAttributes.size(); i++)			
		{
			string vecAttrName = currAttributes[i].getName();
			string vecAttrType = currAttributes[i].getType();
			if(vecAttrName == attrName)
			{
				col = i;
				if(vecAttrType == "integer" || vecAttrType == "INTEGER")
					stoiFLAG = true;
			}
		}
		if(col == -1)
		{
			cerr << "\nERROR: attribute name does not exist:: selection" << endl;
			return "Selection ERROR";
		}
		if(comp[0].at(0) != '"' && !isdigit(comp[0].at(0)))
		{
			isLiteral = false;
			for(int i = 0; i < currAttributes.size(); i++)			
			{
				vecAttrName2 = currAttributes[i].getName();
				vecAttrType2 = currAttributes[i].getType();
				if(vecAttrName2 == comp[0])
				{
					col2 = i;
					if(vecAttrType2 == "integer" || vecAttrType2 == "INTEGER")
						stoiFLAG = true;
				}
			}
		}
		else
		{
			if(!isdigit(comp[0].at(0)))
			{
				string newComp = "";
				for(int x = 0; x<comp[0].size(); x++)
				{
					if(comp[0].at(x) != '"')
						newComp += comp[0].at(x);
				}
				comp[0] = newComp;
			}
		}
		if(col == -1 && !(!isLiteral && col2 == -1))
		{
			cerr << "\nERROR: attribute name does not exist:: selection stage 2" << endl;
			return "Selection ERROR";
		}
		else
		{
			vector<Record> newRecords;
			vector<Record> currRecords = _dbTables[tblIndex].getTblRecords();
			for(int i = 0; i < currRecords.size(); i++)
			{
				vector<string> currRecordsString = currRecords[i].getRecord();
				string toCompare = currRecordsString[col];
				bool skip = false;
				bool addRecord = false;
				if(!isLiteral)
				{

					if(op == "==")
					{
						if(currRecords[i].getRecord()[col] == currRecords[i].getRecord()[col2])
						{
							addRecord = true;
						}
					}
					else if(op == "!=")
					{
						if(currRecords[i].getRecord()[col] != currRecords[i].getRecord()[col2])
						{
							addRecord = true;
						}
					}
					else if(op == "<=" && stoiFLAG)
					{
						
						if(stoi(currRecords[i].getRecord()[col]) <= stoi(currRecords[i].getRecord()[col2]))
						{
							addRecord = true;
						}
					}
					else if(op == ">=" && stoiFLAG)
					{
						if(stoi(currRecords[i].getRecord()[col]) >= stoi(currRecords[i].getRecord()[col2]))
						{
							addRecord = true;
						}
					}
					else if(op == "<" && stoiFLAG)
					{
						if(stoi(currRecords[i].getRecord()[col]) < stoi(currRecords[i].getRecord()[col2]))
						{
							addRecord = true;
						}
					}
					else if(op == ">" && stoiFLAG)
					{
						if(stoi(currRecords[i].getRecord()[col]) > stoi(currRecords[i].getRecord()[col2]))
						{
							addRecord = true;
						}
					}
					else
						cout<<"Tried to compare a string to an int =("<<endl;
					if(addRecord )
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == "==")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare == intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							if(toCompare == comp[k]){
								addRecord = true;
							}
						}
					}
					if(addRecord )
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == "!=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare != intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							// use this if-stmt for strings
							if(toCompare != comp[k])
								addRecord = true;
							else
								skip = true;
						}
					}
					if(addRecord && !skip)
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == "<")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare < intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							cerr << "ERROR: Can't use op '<' when comparing strings" << endl;
						}
					}
					if(addRecord && !skip)
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == ">")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare > intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							cerr << "ERROR: Can't use op '>' when comparing strings" << endl;
						}
					}
					if(addRecord && !skip)
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == "<=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare <= intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							cerr << "ERROR: Can't use op '<=' when comparing strings" << endl;
						}
					}
					if(addRecord && !skip)
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
				else if(op == ">=")
				{
					for(int k = 0; k < comp.size(); k++)
					{
						if(stoiFLAG)
						{
							int intToCompare = stoi(toCompare);
							int intComp = stoi(comp[k]);
							
							if(intToCompare >= intComp)
								addRecord = true;
							else
								skip = true;
						}
						else
						{
							cerr << "ERROR: Can't use op '>=' when comparing strings" << endl;
						}
					}
					if(addRecord && !skip)
					{
						//add the record
						newRecords.push_back(Record(currRecordsString));
					}
				}
			}
			++selectionTblCount;
			string tblName = "selection-" + to_string(selectionTblCount);
			Table newTable(tblName, currAttributes, newRecords, true);
			_subQuery.push_back(tblName);
			_dbTables.push_back(newTable);
			return tblName;
		}
	}
}
/****
	The renaming() function searches through the list of attributes for a given name that that needs to be replaced. 
	If the name exists, the current names in the relation are replaced with the new name that is passed into the function.
	This only occurs if both of the attribute list sizes are identical because each element of the attribute list 
	corresponds directly with the new attribute list that contains the value we want to rename the current list with. 
	This is another risk/assumption that needs to be added to the updated design document as it can cause potential 
	problems if the vector sizes are not the same.
****/
string Database::renaming(string tableName, vector<string> names) //ready for testing
{
	int tblIndex = getTableIndex(_dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
	}
	else
	{
		for(int x = 0; x < _dbTables[tblIndex].getTblAttributes().size(); x++) //moves through Attributes list
		{
			if(x<names.size() && !names[x].empty()) // assigns name to new name if new name exists
			{
				// check for attribute keys
				string oldAttributeName = _dbTables[tblIndex].getTblAttributes()[x].getName();
				if (oldAttributeName.at(0) == '*')
				{
					string newAttribute = names[x];
					newAttribute.insert(0,"*");
					_dbTables[tblIndex].changeAttributeName(x, newAttribute);
				}
				else
				{	
					_dbTables[tblIndex].changeAttributeName(x, names[x]);
				}
			}
		}
	}
	return tableName;
}
/****
	The projection() function selects a subset of the attributes in a relation by doing two searches. The first 
	search consists of finding the attribute name within the given vector of attributes and putting the subset of 
	attributes that match the criteria into their own vector. The second search does the same type of search inside 
	the vector of records to find the corresponding attributes. Once the two searches are complete, a new table that 
	only contains the particular subset of relations we wanted is created.
****/
string Database::projection(string tableName, vector<string> projAttr)
{
	
	int tblIndex = getTableIndex(_dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		Table tableLeft = _dbTables[tblIndex];
		vector<int> cols;
		vector<Attribute> attr;
		vector<Record> records;
		for(int x = 0; x<tableLeft.getTblAttributes().size(); x++) //moves through Table Left attributes list
		{
			vector<Attribute> tmpV = tableLeft.getTblAttributes();
			for(int y = 0; y<projAttr.size(); y++) //finds attributes desired within Table Left
			{
				if(tmpV[x].getName() == projAttr[y])					
				{
					attr.push_back(tableLeft.getTblAttributes()[x]);
					cols.push_back(x);
				}
			}
		}
		if(attr.size() == 0)
		{
			cerr << "\nERROR: No attributes matched with the passed names"<<endl;
			return "ERROR projection"; // return this as table no table name can have spaces
		}
		else
		{
			// Search through the Records and get the correct attributes
			for(int z = 0; z < tableLeft.getTblRecords().size(); z++) //moves through Table Left
			{
				Record rec;
				Record currentRec = tableLeft.getTblRecords()[z]; //vector<Record> records[z]
				vector<string> newRecordVecStr;
				
				for(int i = 0; i < cols.size(); i++)
				{
					vector<string> currRecordVecStr = currentRec.getRecord();
					int index = cols[i];
					string toBepushback = currRecordVecStr[index];
					newRecordVecStr.push_back(toBepushback);
					rec.setVector(newRecordVecStr);		
				}
				records.push_back(rec); // rec = vector<string>
			}
			++projectTblCount;
			string tblName = "projection-" + to_string(projectTblCount);
			Table projTable(tblName, attr, records, relationalAlgebraBool);
			_dbTables.push_back(projTable);
			_subQuery.push_back(tblName);
			return tblName;
		}
	}
}
/****
	The setUnion() function checks if two relations are union compatible and then computes the set difference of 
	the two relations. If the tables are not union compatible, an error is thrown. However if the tables are 
	compatible, a new unionVector vector containing records is created that stores the records of both vectors. 
	If the same column is found from both vectors, only the first table's records are pushed to the unionVector 
	to avoid having two of the same columns when joining two tables. Finally, the new table is created from the
	two tables that have been merged.
****/
string Database::setUnion(string tableLeftName, string tableRightName)
{
	int tableLeftIndex = getTableIndex(_dbTables, tableLeftName);
	if (tableLeftIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableLeftName << ".db, because it does not exist" << endl;
		return "Table ERROR";
	}
	else
	{
		Table tableLeft = _dbTables[tableLeftIndex];
		int tableRightIndex = getTableIndex(_dbTables, tableRightName);
		if (tableRightIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableRightName << ".db, because it does not exist" << endl;
			return "TABLE ERROR";
		}
		else
		{
			Table tableRight = _dbTables[tableRightIndex];
			if(isUnionCompatible(tableLeft, tableRight))
			{
				vector<Record> unionVector;
				vector<Attribute> attr = tableLeft.getTblAttributes();
				for(int y = 0; y<tableLeft.getTblRecords().size(); y++)//traverses left table
				{
					bool found = false;
					for(int x = 0; x < tableRight.getTblRecords().size(); x++)//checks for a copy of R[0] in Right
					{
						if(tableLeft.getTblRecords()[y] == tableRight.getTblRecords()[x])
						{
							found = true;
						}
					}
					if(!found)
					unionVector.push_back(tableLeft.getTblRecords()[y]);
				}
				for(int y = 0; y<tableRight.getTblRecords().size(); y++) //adds remaining records
				{
					unionVector.push_back(tableRight.getTblRecords()[y]);
				}
				++unionTblCount;
				string tblName = "union-" + to_string(unionTblCount);
				Table unionTable(tblName ,attr,unionVector, relationalAlgebraBool);
				_dbTables.push_back(unionTable);
				_subQuery.push_back(tblName);
				return tblName;
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
	return "Could not complete Union, please check union compatability";
}
/****
	The setDifference() function returns all tuples that were not present in both tables. In order to compute 
	the set difference, the tables must first be checked to see if they are union compatible. The function searches 
	the records in both tables and puts the tuples that do not match into a vector named differenceVector. Another 
	vector named blacklist is created that contains the table index of only one instance of tuples from both tables 
	that match. To compute the set difference, the blacklist vector is used to get the elements from the getTblRecords()
	function to push into differenceVector.
****/
string Database::setDifference(string tableLeftName, string tableRightName) //ready for testing
{
	int tableLeftIndex = getTableIndex(_dbTables, tableLeftName);
	vector<int> blacklist;
	if (tableLeftIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableLeftName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		Table tableLeft = _dbTables[tableLeftIndex];
		int tableRightIndex = getTableIndex(_dbTables, tableRightName);
		if (tableRightIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableRightName << ".db, because it does not exist" << endl;
			return "Table Error";
		}
		else
		{
			Table tableRight = _dbTables[tableRightIndex];
			if(isUnionCompatible(tableLeft,tableRight))
			{
				vector<Attribute> attr = tableLeft.getTblAttributes();
				vector<Record> differenceVector;
				for(int y = 0; y<tableLeft.getTblRecords().size(); y++)//traverses R table
				{
					int rem = 0;
					bool found = false;
					for(int x = 0; x < tableRight.getTblRecords().size(); x++)//checks for a copy of R[0] in Right
					{
						if(tableLeft.getTblRecords()[y] == tableRight.getTblRecords()[x])
						{
							found = true;
							blacklist.push_back(x);
						}
					}
					if(found)
					{
						//removes Records that have redundancies
					}
					else
					{
						differenceVector.push_back(tableLeft.getTblRecords()[y]); //adds Records that have no redundancies
					}
				}
				for(int y = 0; y<tableRight.getTblRecords().size(); y++) //adds remaining Records
				{
					bool safe = true;
					for(int z = 0; z<blacklist.size(); z++)
					{
						if(y == blacklist[z])
							safe = false;
					}
					if(safe)
					differenceVector.push_back(tableRight.getTblRecords()[y]);
				}
				++setDifferenceCounter;
				string tblName = "setDifference-" + to_string(setDifferenceCounter);
				Table DifferenceTable(tblName,attr,differenceVector, relationalAlgebraBool);
				_dbTables.push_back(DifferenceTable);
				_subQuery.push_back(tblName);
				return tblName;
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
	return "Could not complete Difference, please check union compatability";
}
/****
	The setIntersect() function searches through the list of records comparing the records of a first table to those of 
	the second. It keeps track of which records are present and identical in both tables. If any records are identical in 
	both relations they will be listed in the result table. This method is important for recursiveDescent.
****/
string Database::setIntersect(string tableLeftName, string tableRightName)
{
	int tableLeftIndex = getTableIndex(_dbTables, tableLeftName);
	vector<int> blacklist;
	if (tableLeftIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableLeftName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		Table tableLeft = _dbTables[tableLeftIndex];
		int tableRightIndex = getTableIndex(_dbTables, tableRightName);
		if (tableRightIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableRightName << ".db, because it does not exist" << endl;
			return "Table Error";
		}
		else
		{
			Table tableRight = _dbTables[tableRightIndex];
			if(isUnionCompatible(tableLeft,tableRight))
			{
				vector<Attribute> attr = tableLeft.getTblAttributes();
				vector<Record> intersectVector;
				for(int x = 0; x<tableLeft.getTblRecords().size(); x++)
				{
					for(int y = 0; y<tableRight.getTblRecords().size(); y++)
					{
						if(tableLeft.getTblRecords()[x] == tableRight.getTblRecords()[y])
						{
							intersectVector.push_back(tableLeft.getTblRecords()[x]);
						}
					}
				}	
				++intersectTblCount;
				string tblName = "intersect-" + to_string(intersectTblCount);
				Table* intersectTable = new Table(tblName,attr,intersectVector, relationalAlgebraBool);
				_dbTables.push_back(*intersectTable);
				_subQuery.push_back(tblName);
				return tblName;
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
	return "Could not complete Intersect, please check union compatability";
}
/****
	The crossProduct() function goes through every record of a first table and combines each record of the 
	first table with all of the records of a second table. This creates a records a table containing every 
	possible combination of every record.
****/
string Database::crossProduct(string tableLeftName, string tableRightName) //ready for testing
{
	int tableLeftIndex = getTableIndex(_dbTables, tableLeftName);
	if (tableLeftIndex < 0)
	{
		cerr << "\nERROR: Can't load " << tableLeftName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		Table tableLeft = _dbTables[tableLeftIndex];
		int tableRightIndex = getTableIndex(_dbTables, tableRightName);
		if (tableRightIndex < 0)
		{
			cerr << "\nERROR: Can't load " << tableRightName << ".db, because it does not exist" << endl;
			return "Table Error";
		}
		else
		{
			Table tableRight = _dbTables[tableRightIndex];
			vector<Record> fusionRecords;
			vector<Attribute> fusionAttr = tableLeft.getTblAttributes();
			for(int x = 0; x<tableLeft.getTblRecords().size(); x++) //moves through all records of R
			{
				for(int y = 0; y<tableRight.getTblRecords().size(); y++) //moves through all records of Right
				{
					vector<string> records = tableLeft.getTblRecords()[x].getRecord();
					Record recTemp = Record(tableRight.getTblRecords()[y]);
					for(int z = 0; z<recTemp.getRecord().size(); z++)//for(int z = 0; z<Right.getTblAttributes().size(); z++) //appends every attribute of current Right record to current R record
					{
						if( x ==0 && y == 0)
						{
							fusionAttr.push_back(tableRight.getTblAttributes()[z]);
						}
						string temp = recTemp.getRecord()[z];
						records.push_back(temp);
					}
					fusionRecords.push_back(Record(records));
				}
			}
			
			++crossProductTblCount;
			string tblName = "crossProduct-" + to_string(crossProductTblCount);
			Table crossProdTable(tblName, fusionAttr, fusionRecords, relationalAlgebraBool);
			_dbTables.push_back(crossProdTable);
			_subQuery.push_back(tblName);
			return tblName;
		}
	}
	
}
/****
	The naturalJoin() function goes through every record of a first table and matches the common columns between the two. 
	Records are combined if every common column attribute matches between the two records. The result table thus contains 
	the logical combination of the two relations.
****/
string Database::naturalJoin(string tableLeftName, string tableRightName) //ready for testing
{
	vector<string> commonAttr;
	vector<int> rightCommonCol;
	vector<int> leftCommonCol;
	vector<Attribute> jointAttr;
	vector<Record> jointRec;
	int tableLeftIndex = getTableIndex(_dbTables, tableLeftName);
	if (tableLeftIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableLeftName << ".db, because it does not exist" << endl;
		return "Table Error";
	}
	else
	{
		Table tableLeft = _dbTables[tableLeftIndex];
		int tableRightIndex = getTableIndex(_dbTables, tableRightName);
		if (tableRightIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableRightName << ".db, because it does not exist" << endl;
			return "Table Error";
		}
		else
		{
			Table tableRight = _dbTables[tableRightIndex];
			jointAttr = tableLeft.getTblAttributes();
			for(int y=0; y<tableRight.getTblAttributes().size(); y++)
			{
				bool found = false;
				for(int z = 0; z<jointAttr.size(); z++)
				{
					if(jointAttr[z].getName() == tableRight.getTblAttributes()[y].getName())
						found = true;
				}
				if(!found)
					jointAttr.push_back(tableRight.getTblAttributes()[y]);
			}
			for(int x =0; x<tableLeft.getTblAttributes().size(); x++)
			{
				for(int y=0; y<tableRight.getTblAttributes().size(); y++)
				{
					if(tableLeft.getTblAttributes()[x].getName() == tableRight.getTblAttributes()[y].getName())
					{
						leftCommonCol.push_back(x);
						rightCommonCol.push_back(y);
						commonAttr.push_back(tableLeft.getTblAttributes()[x].getName());
					}
				}
			}
			for(int z = 0; z<tableLeft.getTblRecords().size(); z++) // traverses Table Left records
			{
				for(int i = 0; i<tableRight.getTblRecords().size(); i++) //traverses Table Right records
				{
					bool allRight = true;
					vector<string> record = tableLeft.getTblRecords()[z].getRecord();
					for(int n = 0; n<commonAttr.size(); n++) //matches common columns
					{		
						allRight = (allRight && tableLeft.getTblRecords()[z].getRecord()[leftCommonCol[n]] == tableRight.getTblRecords()[i].getRecord()[rightCommonCol[n]]);
					}
					if(allRight)//unions all other attributes
					{
						for(int n = 0; n<tableRight.getTblRecords()[i].getRecord().size(); n++)
						{
							bool found = false;
							for(int a = 0; a<rightCommonCol.size(); a++)
							{
								if(n==rightCommonCol[a])
									found = true;
							}
							if(!found)
								record.push_back(tableRight.getTblRecords()[i].getRecord()[n]);
						}
						jointRec.push_back(Record(record));
					}
				}
			}
			
		}
		++NaturalJoinCounter;
		string tblName = "naturalJoin-" + to_string(NaturalJoinCounter);	
		Table joinedTable(tblName,jointAttr, jointRec, relationalAlgebraBool);
		_dbTables.push_back(joinedTable);
		_subQuery.push_back(tblName);
		return tblName;
	}
	
}
//======================== Helper functions =========================
/****
	Retrieves table from _dbTables vector based on name
****/
Table* Database::getTableByName(string tblName)
{
	int tblIndex = getTableIndex(_dbTables, tblName);
	if (tblIndex < 0)
	{
		cerr << "ERROR: Cannot get table by name, " << tblName << ", table does not exist in database" << endl;
	}
	else
	{
		return getDBtable(tblIndex);
	}
}
/****
	The isUnionCompatible() function simply checks if two tables are able to be combined. To check compatibility, 
	we compare both table's attribute to make sure they are the same size. We then check to see if the attribute 
	names match. If both conditions are met, the two tables are considered union compatible and the function returns 
	true.
****/
bool Database::isUnionCompatible(Table tableLeft, Table tableRight) 
{
	bool compatible = true;
	if(tableLeft.getTblAttributes().size() == tableRight.getTblAttributes().size()) //checks to makes sure attribute lists are the same size
	{
		for(int x =0; x<tableLeft.getTblAttributes().size(); x++) //checks attribute name matching
		{
			if(tableLeft.getTblAttributes()[x].getName() != tableRight.getTblAttributes()[x].getName()){
				return false;
			}
		}
	}
	else
		compatible = false;
	return compatible;
}
/****
	The conditionParser() function is called by recursiveDescent() whenever a distinct function is located. 
	The conditionParser will then traverse this string and separate the elements into an attributeName, an operation, 
	and a list of constants if there are multiple, and finally calling selection with the arguments found.
****/
string Database::conditionParser(string tableName, string condition)
{
	int i = 0;
	while(i < condition.size())
	{
		bool loopFinished = true;
		string attName = "";
		string comp = "";
		vector<string> constList;
		string ops = "";
		string next = "";
		while(condition[i] != '<' && condition[i] != '>' && condition[i] != '=' && condition[i] != '!' || loopFinished == false)
		{
			if(condition[i] == '(')
				loopFinished = false;
			if(condition[i] == ')')
				loopFinished = true;
			if(condition[i] != ' ' && condition[i] != '(' &&  condition[i] != ')' )
				attName.push_back(condition[i]);
			i++;
		}
		while(condition[i] == '<' || condition[i] == '>' || condition[i] == '=' || condition[i] == '!' || loopFinished == false)
		{
			if(condition[i] == '(')
				loopFinished = false;
			if(condition[i] == ')')
				loopFinished = true;
			if(condition[i] !=' '&& condition[i] != '('&& condition[i] !=')')
				ops.push_back(condition[i]);
			i++;
		} 
		while((condition[i] != '|'&& condition[i] !='&' || loopFinished == false) && i<condition.size())
		{
			if(condition[i] == '(')
				loopFinished = false;
			if(condition[i] == ')')
				loopFinished = true;
			if(condition[i] == '|' || condition[i] == '&')
			{
				constList.push_back(comp);
				comp = "";
				i++;
			}
			else if(condition[i] !=' '&& condition[i] !='('&& condition[i] !=')')
			{
				comp.push_back(condition[i]);
			}
			i++;
		}
		constList.push_back(comp);
		return selection(tableName, attName, ops, constList);
	}
}
/****
	The recursiveDescent() function recursively goes through a conditional statement separating individual logic 
	functions which it passes to the conditionParser. If multiple logic statements are combined using || and && 
	it will compute the setUnion or setIntersect respectively.
****/
string Database::recursiveDescent(string tableName, string condition)
{
	string Right = tableName;
	Table temp =  _dbTables[getTableIndex(_dbTables, tableName)];
	temp.changeTableName("temp");
	_dbTables.push_back(temp);
	int start = 0;
	int finish = 0;
	int resolved = 0;
	string pass = "";
	int unint = 0;
	int i = 0;
	bool parIgnore = false;
	bool stringTime = false;
	while(i<condition.size())
	{
		if(resolved == 0)
		{
			if(condition[i] == '(' && !parIgnore)
			{
				if(resolved == 0)
					start = i+1;
				resolved--;
				i++;
			}
			else if((condition[i] == '&' || condition[i] == '|') && !parIgnore)
			{
				if(condition[i] == '|')
					unint = 1;
				else
					unint = 2;
				i = i+2;
			}
			else
			{
				
				if(parIgnore && !stringTime && condition[i] == '"')
					stringTime = true;
				if(!stringTime && condition[i]!=')'&&condition[i]!=' ') // && 
				{
					pass+=condition[i];
				}
				if(condition[i] == '=' && condition[i+1] == '(')
					parIgnore = true;
				if(parIgnore && condition[i] == ')')
				{
					stringTime = false;
					parIgnore = false;
				}
				i++;
			}
		}
		else
		{
			if(condition[i] == '(' && !parIgnore)
			{
				resolved--;
			}
			if(condition[i] == ')')
			{
				if(!parIgnore)
					resolved++;
				else
					parIgnore = false;
				finish = i-1;
				if(resolved == 0)
				{
					if(unint == 1)
					{
						string S = recursiveDescent("temp", condition.substr(start,finish) );
						Right = setUnion(Right, S);
					}
					else if(unint == 2)
					{
						string S = recursiveDescent("temp", condition.substr(start,finish));
						Right = setIntersect(Right, S);
					}
					else
					{
						Right = recursiveDescent("temp", condition.substr(start, finish));
					}
				}
				
			}
			i++;
		}
	}
	if(pass.size() > 0)
		Right = conditionParser("temp", pass);
	dropTable("temp");
	return Right;
}
/****
	The getAttributesInFile() opens an input file stream to read in text from a .db file. A vector of strings is 
	created that will hold all the attribute names from the file and determine whether or not the attribute is a key. 
	Once the attribute name is read in, it's type needs to be determined. If the type is a varchar, the value of the 
	int needs to be stored. An Attribute constructor, which contains a vector inside it's class, is called to store 
	the attributes pulled from the file into the vector.
****/
void Database::getAttributesInFile(ifstream &inputFile, vector<Attribute> &attributes)
{
    string attributeList;
	getline(inputFile, attributeList);
	stringstream sstream(attributeList);
    vector<string> vecAttributes = readFromFile(sstream);
    for(int i = 0; i < vecAttributes.size(); i++)
    {
        string attributeName = "";
        string attributeType = "";
        bool key = false;
        int maxSize = -1;
        // check for * indicating the attribute is a KEY
        stringstream sstream2(vecAttributes[i]);
        
        getline(sstream2, attributeName, '[');
        getline(sstream2, attributeType, ']');
		if(attributeName.at(0) == '*')
		{
            key = true;
			attributeName.erase(attributeName.begin());
		}
           
        // If the Type is a VARCHAR need to find the value of the INT 
        if(attributeType.at(0) == 'v' || attributeType.at(0) == 'V' )
        {
            unsigned first = attributeType.find("("); 
            unsigned last = attributeType.find(")");
            string num = attributeType.substr(first+1,last-(first+1));
            string::size_type sz;// alias of size_t (allows me to convert to decimal int)
            maxSize = stoi(num,&sz);
        }   
        Attribute tmpAttribute(attributeName, attributeType, maxSize, key);
        attributes.push_back(tmpAttribute);
    }    
}
/****
	The getRecordsInFile() reads in records from a .db file. The input read in is 
	then stored in the vector inside of the Record class.
****/
void Database::getRecordsInFile(ifstream &inputFile, vector<Record> &records)
{
    while(!(inputFile.eof()))
    {
        string recordList;
        getline(inputFile, recordList);
        stringstream sstream(recordList);

        vector<string> recordInfo = readFromFile(sstream);

        Record tmpRecord(recordInfo);
        records.push_back(tmpRecord);
        recordInfo.clear();
    }    
}
/****
	Retrieves the index in the _dbTables vector that the table with the given name resides at.
****/
int Database::getTableIndex(string tableName)
{
	 for(int index = 0; index < _dbTables.size(); index++)
    {
        string currTableName = _dbTables[index].getTableName();   
        if ( tableName == currTableName )
            return index;
    }
    return -1;
}
/****
	The getTableIndex() is the heart of reading from the file. This function allows the user to type in the '|' 
	character in as an input and keep it as part of the value. This ensures that erroneous user input that may 
	contain '|' is accounted for and does not interfere with how the input is parsed.
****/
int Database::getTableIndex(vector<Table>& _dbTables, string tableName)
{	
    // find if the table referenced by tableName exists return index, if not return -1
    for(int index = 0; index < _dbTables.size(); index++)
    {
        string currTableName = _dbTables[index].getTableName();     
        if ( tableName == currTableName )
            return index;
    }
    return -1;
}
/*****
	this function is the heart of reading from the file as it is what is actually
	correctly reading the input. this allows the user to type in the '|' character 
	in as input and keep it as part of the value
	CREDIT: the main approach for this function was given to me from Yin Qu the TA for 315, I simply edited his example to me to fit the needs of this program
*****/
vector<string> Database::readFromFile(stringstream& sstream)
{
    vector<string> attributeList;
    ostringstream result;
    bool openingVerticalLine = false;
    bool closingVerticalLine = false;
    bool escapeCHR = false;
    char c;
    while(!sstream.eof())
    {
        sstream.get(c);
        if(!openingVerticalLine)
        {
            if (c == '|')
            {
                openingVerticalLine = true;
                continue;
            }
            else
            {
                // error: NO Opening | line for attribute
                // this should only happen if our code is broken
                throw runtime_error("\nERROR code brokedid: reading from file, no opeing | character in .db file"); 
            }
        }
        if(!closingVerticalLine && !escapeCHR && c == '|')
        {
            // this is the end of the attribute, store the string in a vector of strings
            attributeList.push_back(result.str());
            result.str("");
            result.clear();
            openingVerticalLine = false;
            closingVerticalLine = false;
            escapeCHR = false;
            continue;
        }
        if(!escapeCHR && c == '\\')
        {
            escapeCHR = true;
        }
        else if (escapeCHR && c == '|')
        {
            result << c;
        }
        else
        {
            result << c;
            escapeCHR = false;
            closingVerticalLine = false;
        }
    }
    return attributeList;
}
/****
	The addRecord() function adds a new record to the _dbTables vector element and returns the record added.
****/
string Database::addRecordToTable(int index, Record &newRecord) 
{ 	
    string flag = _dbTables[index].addNewRecord(newRecord); 
    return flag;
}
/****
	This method was implemented to delete intermediate table values so that we can preserve
	system memory and improve runtimes for table lookups.
****/
void Database::dropQueryVec()
{
	if(_subQuery.size() >0)
	{
		for(int i = 0; i < _subQuery.size(); i++)
		{
			string dropQuery = _subQuery[i];
			if(getTableIndex(dropQuery) != -1)
			{
				dropTable(dropQuery);
				_subQuery.erase(_subQuery.begin()+i);
				i--;
			}
			else
			{
				_subQuery.erase(_subQuery.begin()+i);
				i--;
			}
		}
		
	}	
}