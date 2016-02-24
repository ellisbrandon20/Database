/*
 Team project 1 - Database.cpp
*/
#include <cstdlib>
#include "Database.h"
int selectionTblCount = 0;
int unionTblCount = 0;
int intersectTblCount = 0;
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
void Database::openTable(string tableName)
{   
	/****
		The openTable() function accepts a name of a table as an argument to open a .db file that has 
		the information stored in it. From the file, the attributes and records are copied into separate 
		vectors associated with their class type. The table is then added into a vector of type Table 
		that stores every Table that is opened through the openTable function.
	****/
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
		cout << "opened file " << file << endl;
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
void Database::closeTable(string tableName)
{
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
			//cout << "CLOSE: WRITE: " << tableName << endl;
			writeTable(tableName);
			_dbTables.erase(_dbTables.begin()+tblIndex);
		}
    }   
}
void Database::writeTable(string tableName)
{
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
	ofstream outputFile;
    int tblIndex = getTableIndex(_dbTables, tableName);
   
    if(tblIndex < 0)
    {
        cerr << "ERROR: Couldn't find file " << tableName << ".db in the database" << endl;
    }
    else
    {
		string file = DBFILEPATH + tableName + ".db";
		outputFile.open(file.c_str());
        // temporary table object
        Table tempTable = _dbTables[tblIndex];
       // vector<Attribute> tblAttributes = _dbTables[tblIndex].getTblAttributes();
        vector<Attribute> tblAttributes = tempTable.getTblAttributes();
      
        // Checking for "|" 
        //vector<string> printVec;
        for(int i = 0; i < tblAttributes.size(); i++)
        {
            string attrName  = tblAttributes[i].getName();
            size_t pos = attrName.find('|');
            if(pos != string::npos)
                attrName.insert(pos, "\\");
                
            // printVec[i] = attrName;
            string printVal = attrName + "[" + tblAttributes[i].getType() + "]";
			//cout <<"attrname: " << printVal << endl;
            //printVec.push_back(printVal);
            outputFile << "|" << printVal << "|";
        } 

        // NOW PRINT RECORDS
        
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
void Database::exit()
{
	/****
		The exit() function exits the program.
	****/
}
void Database::show(string tableName)
{
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
            string attribute = attributeName + "[" + attributeType + "]";
            cout << setw(20) << left << attribute;
        }
        cout << '\n';
        
        // print the records values
        vector<Record> tblRecords = _dbTables[tblIndex].getTblRecords();
        for(int i = 0; i < tblRecords.size(); i++)
        {
            vector<string> currRecord = tblRecords[i].getRecord();
            
            for(int j = 0; j < currRecord.size(); j++)
                cout << setw(20) << left << currRecord[j];

            cout << '\n';
        }
    }
     
}
void Database::createTable(string tableName, vector<string> attributeList, vector<string> keyAttributeList)
{
	/****
		The createTable() function needs the name of the table, the vector of attributes, and the keys associated with 
		it as arguments. The program iterates through the attributes vector looking for the '*' symbol. This indicates 
		that the attribute is a key. We then check the type of the attribute to see if its a varchar. If it is, find the 
		int value associated with it and then push that into a new attribute vector that only contains keys. This attribute 
		vector containing keys is the one used to create the table.
	****/
    int tblIndex = getTableIndex(_dbTables, tableName);

    if(tblIndex < 0)
    {
        // "*name[varchar(#)]"  |  "name[int]" = attributeList[i]

        string attributeName = "";
        string attributeType = "";
        int maxSize = -1;
        bool key = false;

        vector<Attribute> attributes;

        for(int i = 0; i < attributeList.size(); i++)
        {
            stringstream sstream(attributeList[i]);

            int peek = sstream.peek(); // returns the DECIMAL value of the next character
            // if peek == 42 (ASCII value for '*') then the attribute is a KEY
            if(peek == 42)
                key = true;

            getline(sstream, attributeName, '[');
            getline(sstream, attributeType, ']'); 

        
            // If the Type is a VARCHAR need to find the value of the INT 
            if(attributeType.at(0) == 'v' || attributeType.at(0) == 'V' )
            {
                unsigned first = attributeType.find("("); 
                unsigned last = attributeType.find(")");
                string num = attributeType.substr(first+1,last-(first+1));
                //cout << "num: " << num << endl;

                string::size_type sz; // alias of size_t (allows me to convert to decimal int)
                maxSize = stoi(num,&sz);
                //cout << "maxSize: " << maxSize << endl;
            }

            Attribute tmpAttribute(attributeName, attributeType, maxSize, key);
            attributes.push_back(tmpAttribute);

        }

        // create the table
        vector<Record> empty; 
        // ******************* ^^ will have to make sure this is how we want to pass this argument


        Table newTable(tableName, attributes, empty);
        _dbTables.push_back(newTable);
    }
    else{
        cerr << "\nERROR: The table " << tableName << " already exists." << endl;
    }
}
void Database::insertToTable(string tableName, vector<string> literals)
{
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
        string flag = addRecord(tblIndex, tmpRecord);

        if (flag != "")
            cerr << flag;
    }
}
void Database::updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals)
{
	/****
		The updateTable() function modifies a table based on given criteria. If a row matches the arguments passed along with 
		the operations applied to it, updateTable() allows records of that row to be changed. The function first searches for 
		the column that matches the attribute name of the record. If that attribute name exists, the name of the record is 
		searched for and, if found, checks if the comparison on the test conditions evaluate to true. If true, the records 
		that were searched for are then updated to their new values.
	****/
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
			cerr << "\nERROR: attribute name does not exist:: selection" << endl;
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
							//cout << toCompare << "==" << comp[k] << endl;
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
void Database::deleteFromTable(string tableName, string attrName, string op, vector<string> &comp)
{
	/****
		The deleteFromTable() function looks within the vector of attributes to find which column (index for _recordValues)
		in the table that is needed to be checked for each Record. The program checks if the attribute name exists and 
		proceeds to check what comparison operator is being passed in order to determine which attributes needs to be 
		deleted from the table if the comparison evaluates to true. This is done through many if statements for each 
		operator possible. Since strings can only be compared with "==" and "!=", the program outputs an error if an 
		attempt is made to compare a string with the other comparison operators. Once the comparisons are done, the 
		eraseRecord() function is called and deletes desired values that satisfied the condition.
	****/
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
			cerr << "\nERROR: attribute name does not exist:: selection" << endl;
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
							cout << toCompare << " != " << comp[k] << endl;
							if(toCompare != comp[k]){
								deleteRecord = true;
								cout <<"true"<<endl;
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
string Database::selection(string tableName, string attrName, string op, vector<string> &comp)
{
	/****
		The selection() function begins by searching for the column that contains the name of the attribute that was 
		passed to the function. When the appropriate column is found, we look at what comparison operator was called 
		and proceed to select all the tuples in the relation that satisfy the condition. For every instance where the 
		Tuple satisfies the condition, it is pushed into a new vector of Records which is then passed into a table 
		constructor to create a new table of only those Tuples.
	****/
	int tblIndex = getTableIndex( _dbTables, tableName);
	
	if (tblIndex < 0)
	{
		cerr << "\nSelection ERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
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
			cerr << "\nERROR: attribute name does not exist:: selection" << endl;
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
				
				if(op == "==")
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
							//cout << toCompare << "==" << comp[k] << endl;
							if(toCompare == comp[k]){
								addRecord = true;
								//cout << "true"<<endl;
							}
							//else
								//skip = true;
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
			
			string tblName = "selection" + to_string(selectionTblCount);
			cout << "tblName: " << tblName << endl;
			Table newTable(tblName, currAttributes, newRecords, true);
			_dbTables.push_back(newTable);
			
			//show(tblName);
			
			return tblName;
		}
	
	}
}
void Database::renaming(string tableName, vector<string> names) //ready for testing
{
	/****
		The renaming() function searches through the list of attributes for a given name that that needs to be replaced. 
		If the name exists, the current names in the relation are replaced with the new name that is passed into the function.
		This only occurs if both of the attribute list sizes are identical because each element of the attribute list 
		corresponds directly with the new attribute list that contains the value we want to rename the current list with. 
		This is another risk/assumption that needs to be added to the updated design document as it can cause potential 
		problems if the vector sizes are not the same.
	****/
	int tblIndex = getTableIndex(_dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
	}
	else
	{
		//Table t = _dbTables[tblIndex];
		
		for(int x = 0; x < _dbTables[tblIndex].getTblAttributes().size(); x++) //moves through Attributes list
		{
			if(x<names.size() && !names[x].empty()) // assigns name to new name if new name exists
			{
				// check for attribute keys
				string oldAttributeName = _dbTables[tblIndex].getTblAttributes()[x].getName();
				if (oldAttributeName.at(0) == '*')
				{
					//cout << "here" << endl;
					string newAttribute = names[x];
					newAttribute.insert(0,"*");
					//t.getTblAttributes()[x].getName() = newAttribute;
					//cout << "nAtr: " << newAttribute << endl;
					_dbTables[tblIndex].changeAttributeName(x, newAttribute);
				}
				else
				{	
					//cout << "else" << endl;
					//t.getTblAttributes()[x].getName() = names[x];	
					_dbTables[tblIndex].changeAttributeName(x, names[x]);
				}
			}
		}
	}
	
}
void Database::projection(string tableName, vector<string> projAttr) //ready for testing
{
	/****
		The projection() function selects a subset of the attributes in a relation by doing two searches. The first 
		search consists of finding the attribute name within the given vector of attributes and putting the subset of 
		attributes that match the criteria into their own vector. The second search does the same type of search inside 
		the vector of records to find the corresponding attributes. Once the two searches are complete, a new table that 
		only contains the particular subset of relations we wanted is created.
	****/
	int tblIndex = getTableIndex(_dbTables, tableName);
	if (tblIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblIndex];
		vector<int> cols;
		vector<Attribute> attr;
		vector<Record> records;
		for(int x = 0; x<R.getTblAttributes().size(); x++) //moves through Table R attributes list
		{
			vector<Attribute> tmpV = R.getTblAttributes();
			for(int y = 0; y<projAttr.size(); y++) //finds attributes desired within Table R
			{
				//cout << tmpV[x].getName() << " ?==?" << projAttr[y] << endl;
				if(tmpV[x].getName() == projAttr[y])					
				{
					attr.push_back(R.getTblAttributes()[x]);
					cols.push_back(x);
				}
			}
		}
		// Search through the Records and get the correct attributes
		for(int z = 0; z < R.getTblRecords().size(); z++) //moves through table R
		{
			Record rec;
			Record currentRec = R.getTblRecords()[z]; //vector<Record> records[z]
			vector<string> newRecordVecStr;
			
			for(int i = 0; i < cols.size(); i++)
			{
				//cout << "col[i] = " << cols[i] << endl;
				//cout << "push_back: " << currentRec.getRecord()[cols[i]] << endl;
				vector<string> currRecordVecStr = currentRec.getRecord();
				int index = cols[i];
				string toBepushback = currRecordVecStr[index];
				//cout << "toBepushback: " << toBepushback << endl;
				newRecordVecStr.push_back(toBepushback);
				rec.setVector(newRecordVecStr);
				//Record rec(currRecordVecStr);
				
				//currentRec.push_back(currRecordVecStr);
				//rec.getRecord().push_back(currentRec.getRecord()[cols[i]]);
				
			}
			records.push_back(rec); // rec = vector<string>
		}

		Table* projTable = new Table("projection", attr, records, relationalAlgebraBool);
		_dbTables.push_back(*projTable);

		show("projection");
	}
}
string Database::setUnion(string tableRName, string tableSName) //ready for testing
{
	/****
		The setUnion() function checks if two relations are union compatible and then computes the set difference of 
		the two relations. If the tables are not union compatible, an error is thrown. However if the tables are 
		compatible, a new unionVector vector containing records is created that stores the records of both vectors. 
		If the same column is found from both vectors, only the first table's records are pushed to the unionVector 
		to avoid having two of the same columns when joining two tables. Finally, the new table is created from the
		two tables that have been merged.
	****/
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		int tblSIndex = getTableIndex(_dbTables, tableSName);
		if (tblSIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableSName << ".db, because it does not exist" << endl;
		}
		else
		{
			Table S = _dbTables[tblSIndex];
			if(isUnionCompatible(R, S))
			{
				vector<Record> unionVector;
				vector<Attribute> attr = R.getTblAttributes();
				for(int y = 0; y<R.getTblRecords().size(); y++)//while(R.getTblRecords().size()>0)//traverses R table
				{
					bool found = false;
					for(int x = 0; x < S.getTblRecords().size(); x++)//checks for a copy of R[0] in Q
					{
						if(R.getTblRecords()[y] == S.getTblRecords()[x])
						{
							found = true;
						}
					}
					if(!found)
					unionVector.push_back(R.getTblRecords()[y]);
				}
				for(int y = 0; y<S.getTblRecords().size(); y++) //adds remaining records
				{
					unionVector.push_back(S.getTblRecords()[y]);
				}
				++unionTblCount;
				string tblName = "union" + to_string(unionTblCount);
				Table unionTable(tblName ,attr,unionVector, relationalAlgebraBool);
				_dbTables.push_back(unionTable);
				
				show(tblName);
				
				return tblName;
				//show(tblName);
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
}
void Database::setDifference(string tableRName, string tableSName) //ready for testing
{
	/****
		The setDifference() function returns all tuples that were not present in both tables. In order to compute 
		the set difference, the tables must first be checked to see if they are union compatible. The function searches 
		the records in both tables and puts the tuples that do not match into a vector named differenceVector. Another 
		vector named blacklist is created that contains the table index of only one instance of tuples from both tables 
		that match. To compute the set difference, the blacklist vector is used to get the elements from the getTblRecords()
		function to push into differenceVector.
	****/
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	vector<int> blacklist;
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		int tblSIndex = getTableIndex(_dbTables, tableSName);
		if (tblSIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableSName << ".db, because it does not exist" << endl;
		}
		else
		{
			Table S = _dbTables[tblSIndex];
			if(isUnionCompatible(R,S))
			{
				vector<Attribute> attr = R.getTblAttributes();
				vector<Record> differenceVector;
				for(int y = 0; y<R.getTblRecords().size(); y++)//while(R.getTblRecords().size()>0)//traverses R table
				{
					int rem = 0;
					bool found = false;
					for(int x = 0; x < S.getTblRecords().size(); x++)//checks for a copy of R[0] in Q
					{
						if(R.getTblRecords()[y] == S.getTblRecords()[x])
						{
							found = true;
							blacklist.push_back(x);
						}
					}
					if(found)
					{
						//S.getTblRecords().erase(S.getTblRecords().begin()+rem); //removes Records that have redundancies
					}
					else
					{
						differenceVector.push_back(R.getTblRecords()[y]); //adds Records that have no redundancies
					}
					//R.getTblRecords().erase(R.getTblRecords().begin());
				}
				for(int y = 0; y<S.getTblRecords().size(); y++) //adds remaining Records
				{
					bool safe = true;
					for(int z = 0; z<blacklist.size(); z++)
					{
						if(y == blacklist[z])
							safe = false;
					}
					if(safe)
					differenceVector.push_back(S.getTblRecords()[y]);
				}
				Table* DifferenceTable = new Table("SetDifference",attr,differenceVector, relationalAlgebraBool);
				_dbTables.push_back(*DifferenceTable);
				show("SetDifference");
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
}
string Database::setIntersect(string tableRName, string tableSName) //ready for testing
{
	/****
		The setIntersect() function searches through the list of records comparing the records of a first table to those of 
		the second. It keeps track of which records are present and identical in both tables. If any records are identical in 
		both relations they will be listed in the result table. This method is important for recursiveDescent.
	****/
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	vector<int> blacklist;
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		int tblSIndex = getTableIndex(_dbTables, tableSName);
		if (tblSIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableSName << ".db, because it does not exist" << endl;
		}
		else
		{
			Table S = _dbTables[tblSIndex];
			if(isUnionCompatible(R,S))
			{
				vector<Attribute> attr = R.getTblAttributes();
				vector<Record> intersectVector;
				for(int x = 0; x<R.getTblRecords().size(); x++)
				{
					for(int y = 0; y<S.getTblRecords().size(); y++)
					{
						if(R.getTblRecords()[x] == S.getTblRecords()[y])
						{
							intersectVector.push_back(R.getTblRecords()[x]);
						}
					}
				}
				
				++intersectTblCount;
				string tblName = "intersect" + to_string(intersectTblCount);
				Table* intersectTable = new Table(tblName,attr,intersectVector, relationalAlgebraBool);
				_dbTables.push_back(*intersectTable);
				return tblName;
				//show("SetIntersect");
			}
			else
				cerr<<"Not union compatible"<<endl;
		}
	}
}
void Database::crossProduct(string tableRName, string tableQName) //ready for testing
{
	/****
		The crossProduct() function goes through every record of a first table and combines each record of the 
		first table with all of the records of a second table. This creates a records a table containing every 
		possible combination of every record.
	****/
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't load " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		int tblQIndex = getTableIndex(_dbTables, tableQName);
		if (tblQIndex < 0)
		{
			cerr << "\nERROR: Can't load " << tableQName << ".db, because it does not exist" << endl;
		}
		else
		{
			Table Q = _dbTables[tblQIndex];
			vector<Record> fusionRecords;
			vector<Attribute> fusionAttr = R.getTblAttributes();
			for(int x = 0; x<R.getTblRecords().size(); x++) //moves through all records of R
			{
				for(int y = 0; y<Q.getTblRecords().size(); y++) //moves through all records of Q
				{
					vector<string> rec = R.getTblRecords()[x].getRecord();
					Record recTemp = Record(Q.getTblRecords()[y]);
					for(int z = 0; z<recTemp.getRecord().size(); z++)//for(int z = 0; z<Q.getTblAttributes().size(); z++) //appends every attribute of current Q record to current R record
					{
						if( x ==0 && y == 0)
						{
							fusionAttr.push_back(Q.getTblAttributes()[z]);
						}
						string temp = recTemp.getRecord()[z];
						rec.push_back(temp);
					}
					fusionRecords.push_back(Record(rec));
				}
			}
			Table crossTable = Table("CrossProduct",fusionAttr,fusionRecords, relationalAlgebraBool);
			_dbTables.push_back(crossTable);
			show("CrossProduct");
		}
	}
}
void Database::naturalJoin(string tableRName, string tableQName) //ready for testing
{
	/****
		The naturalJoin() function goes through every record of a first table and matches the common columns between the two. 
		Records are combined if every common column attribute matches between the two records. The result table thus contains 
		the logical combination of the two relations.
	****/
	vector<string> commonAttr;
	vector<int> qCommonCol;
	vector<int> rCommonCol;
	vector<Attribute> jointAttr;
	vector<Record> jointRec;
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		int tblQIndex = getTableIndex(_dbTables, tableQName);
		if (tblQIndex < 0)
		{
			cerr << "\nERROR: Can't open " << tableQName << ".db, because it does not exist" << endl;
		}
		else
		{
			Table Q = _dbTables[tblQIndex];
			jointAttr = R.getTblAttributes();
			for(int y=0; y<Q.getTblAttributes().size(); y++)
			{
				bool found = false;
				for(int z = 0; z<jointAttr.size(); z++)
				{
					if(jointAttr[z].getName() == Q.getTblAttributes()[y].getName())
						found = true;
				}
				if(!found)
					jointAttr.push_back(Q.getTblAttributes()[y]);
			}
			for(int x =0; x<Q.getTblAttributes().size(); x++)
			{
				for(int y=0; y<Q.getTblAttributes().size(); y++)
				{
					if(R.getTblAttributes()[x].getName() == Q.getTblAttributes()[y].getName())
					{
						rCommonCol.push_back(x);
						qCommonCol.push_back(y);
						commonAttr.push_back(R.getTblAttributes()[x].getName());
					}
				}
			}
			for(int z = 0; z<R.getTblRecords().size(); z++) // traverses Table R records
			{
				for(int i = 0; i<Q.getTblRecords().size(); i++) //traverses Table Q records
				{
					bool allRight = true;
					vector<string> rec = R.getTblRecords()[z].getRecord();
					for(int n = 0; n<commonAttr.size(); n++) //matches common columns
					{		
						allRight = (allRight && R.getTblRecords()[z].getRecord()[rCommonCol[n]] == Q.getTblRecords()[i].getRecord()[qCommonCol[n]]);
					}
					if(allRight)//unions all other attributes
					{
						for(int n = 0; n<Q.getTblRecords()[i].getRecord().size(); n++)
						{
							bool found = false;
							for(int a = 0; a<qCommonCol.size(); a++)
							{
								if(n==qCommonCol[a])
									found = true;
							}
							if(!found)
								rec.push_back(Q.getTblRecords()[i].getRecord()[n]);
						}
						jointRec.push_back(Record(rec));
					}
				}
			}
			
		}
		Table* joinedTable = new Table("NaturalJoined",jointAttr, jointRec, relationalAlgebraBool);
		_dbTables.push_back(*joinedTable);
		show("NaturalJoined");
	}
	
}
//======================== Helper functions =========================
bool Database::isUnionCompatible(Table R, Table S) 
{
	/****
		The isUnionCompatible() function simply checks if two tables are able to be combined. To check compatibility, 
		we compare both table's attribute to make sure they are the same size. We then check to see if the attribute 
		names match. If both conditions are met, the two tables are considered union compatible and the function returns 
		true.
	****/
	bool compatible = true;
	if(R.getTblAttributes().size() == S.getTblAttributes().size()) //checks to makes sure attribute lists are the same size
	{
		for(int x =0; x<R.getTblAttributes().size(); x++) //checks attribute name matching
		{
			if(R.getTblAttributes()[x].getName() != S.getTblAttributes()[x].getName())
				return false;
		}
	}
	else
		compatible = false;
	return compatible;
}
string Database::conditionParser(string tableName, string condition)
{
	/****
		The conditionParser() function is called by recursiveDescent() whenever a distinct function is located. 
		The conditionParser will then traverse this string and separate the elements into an attributeName, an operation, 
		and a list of constants if there are multiple, and finally calling selection with the arguments found.
	****/
	/*********************************
	It is vitally important that the 
	parser ensures that there are parentheses
	in all the locations necessary to understand
	distinct statements




	************************************/
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

string Database::recursiveDescent(string tableName, string condition)
{
	/****
		The recursiveDescent() function recursively goes through a conditional statement separating individual logic 
		functions which it passes to the conditionParser. If multiple logic statements are combined using || and && 
		it will compute the setUnion or setIntersect respectively.
	****/
	string Q = tableName;
	Table temp =  _dbTables[getTableIndex(_dbTables, tableName)];
	temp.changeTableName("temp");
	_dbTables.push_back(temp);
	int start = 0;
	int finish = 0;
	int resolved = 0;
	string pass;
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
				if(!stringTime && condition[i] != '"' && condition[i]!=')')
					pass+=condition[i];
				if(condition[i] == '=' && condition[i+1] == '(')
					parIgnore = true;
				if(parIgnore && condition[i] == ')')
				{
					//pass.pop_back();
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
						Q = setUnion(Q, S);
					}
					else if(unint == 2)
					{
						string S = recursiveDescent("temp", condition.substr(start,finish));
						Q = setIntersect(Q, S);
					}
					else
						Q = recursiveDescent("temp", condition.substr(start, finish));
				}
				
			}
			i++;
		}
	}
	if(pass.size() > 0)
		Q = conditionParser("temp", pass);
	return Q;
}
void Database::getAttributesInFile(ifstream &inputFile, vector<Attribute> &attributes)
{
	/****
		The getAttributesInFile() opens an input file stream to read in text from a .db file. A vector of strings is 
		created that will hold all the attribute names from the file and determine whether or not the attribute is a key. 
		Once the attribute name is read in, it's type needs to be determined. If the type is a varchar, the value of the 
		int needs to be stored. An Attribute constructor, which contains a vector inside it's class, is called to store 
		the attributes pulled from the file into the vector.
	****/
    /*
    this function along with the function readFromFile will correctly read the .db file
    and place the correct structure in the vector of Attributes
    **/
    string attributeList;
	getline(inputFile, attributeList);
	
	stringstream sstream(attributeList);
	
    //string attributeStr;
    
	
    vector<string> vecAttributes = readFromFile(sstream);

    for(int i = 0; i < vecAttributes.size(); i++)
    {
        string attributeName = "";
        string attributeType = "";
        bool key = false;
        int maxSize = -1;
        //cout << "atr: "<< vecAttributes[i]<< endl;

        // check for * indicating the attribute is a KEY
        stringstream sstream2(vecAttributes[i]);

        int peek = sstream2.peek(); // returns the DECIMAL value of the next character
        // if peek == 42 (ASCII value for '*') then the attribute is a KEY
        if(peek == 42)
            key = true;

        getline(sstream2, attributeName, '[');
        getline(sstream2, attributeType, ']');   
        
        //cout << "attributeType: " << attributeType << endl;
        
        // If the Type is a VARCHAR need to find the value of the INT 
        if(attributeType.at(0) == 'v' || attributeType.at(0) == 'V' )
        {
            unsigned first = attributeType.find("("); 
            unsigned last = attributeType.find(")");
            string num = attributeType.substr(first+1,last-(first+1));
            //cout << "num: " << num << endl;

            string::size_type sz;// alias of size_t (allows me to convert to decimal int)
            maxSize = stoi(num,&sz);
            //cout << "maxSize: " << maxSize << endl;
        }
        
        Attribute tmpAttribute(attributeName, attributeType, maxSize, key);
        attributes.push_back(tmpAttribute);
    }    
}
void Database::getRecordsInFile(ifstream &inputFile, vector<Record> &records)
{
	/****
		The getRecordsInFile() reads in records from a .db file. The input read in is 
		then stored in the vector inside of the Record class.
	****/
	/*
    this function along with the function readFromFile will correctly read the .db file
    and place the correct structure in the vector of Records
    */
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

int Database::getTableIndex(vector<Table>& _dbTables, string tableName)
{
	/****
		The getTableIndex() is the heart of reading from the file. This function allows the user to type in the '|' 
		character in as an input and keep it as part of the value. This ensures that erroneous user input that may 
		contain '|' is accounted for and does not interfere with how the input is parsed.
	****/
    // find if the table referenced by tableName exists return index, if not return -1
    for(int index = 0; index < _dbTables.size(); index++)
    {
        string currTableName = _dbTables[index].getTableName();
        
        if ( tableName == currTableName )
            return index;
    }
    return -1;
}
vector<string> Database::readFromFile(stringstream& sstream)
{
    /*****
    this function is the heart of reading from the file as it is what is actually
    correctly reading the input. this allows the user to type in the '|' character 
    in as input and keep it as part of the value
    CREDIT: the main approach for this function was given to me from Yin Qu the TA for 315, I simply edited his example to me to fit the needs of this program
    *****/
    vector<string> attributeList;
    //cout << "sstream.gcount(): " << sstream.str().size() << endl;

    ostringstream result;
    bool openingVerticalLine = false;
    bool closingVerticalLine = false;
    bool escapeCHR = false;
    char c;

    while(!sstream.eof())
    {
		
		//cout << "sstream.good(): " << sstream.good() << endl;
		
		
		
        sstream.get(c);
       // cout << "C:" << c << endl;
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
            //cout << "r:" << result.str() << endl;
            //clear result and set OVL and EVL ESCAPE == false
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

string Database::addRecord(int index, Record &newRecord) 
{ 
	/****
		The addRecord() function adds a new record to the _dbTables vector element and returns the record added.
	****/
    string flag = _dbTables[index].addNewRecord(newRecord); 
    return flag;
}