/*
	Parser.cpp
	ABV- Database Project
	CSCE 315
	This file contains the methods required to parse the pseudo SQL and interact with the database engine accordingly
*/
#include "Parser.h"

void Parser::parse(string input)
{
	tokens.clear();
	if(input.size()>0)
	cout << "input: " << input << endl;
	cerr << "input: " << input << endl;
	string reserveWord;
	storedInput = input;	
	commandAlreadyProcessed = false;
	showCommand = false;
	insertFromRelation = false;
	deleteFrom = false;
	update = false;
	bool semiTerminated = true;
	position = storedInput.end();
	while(*position != ';')
		--position;
	/****
		This method of semicolon termination does not account for a semicolon in the middle of the command
		however this was considered an edge case and for the time being semicolons being only at the end
		is one of our assumptions.
	****/
	if(position == storedInput.begin())
	{
		semiTerminated = false;
		cerr << "This line is not terminated with a semicolon, its' input will be ignored" << endl;
	}
	position = storedInput.begin();
	while(position != storedInput.end() && semiTerminated)
	{
		if( *position == ' ' || *position == '\n' || *position == '\t' )
		{
			++position;
		}
		else if ( isalpha(*position)) //check if starting input is characters
		{ 
			readReserveWord();
		}
		else if( isdigit(*position))
		{
			string num = "";
			while(isdigit(*position))
			{
				num+= *position;
				position++;
			}
			tokens.push_back(num);
		}
		else if (*position=='<' && *(position+1)=='-')
		{
			reserveWord = "<-";
			position += 2;
			tokens.push_back(reserveWord);
		}
		else if(*position == '(' || *position == ')')
		{
			string parenthesis = "";
			parenthesis += *position;
			tokens.push_back(parenthesis);
			++position;
		}
		else if(*position == '+' || *position == '-' || *position == '*')
		{
			string algebraOP = "";
			algebraOP += *position;
			isExpr(algebraOP);
			++position;
		}
		else if(*position == ';')
		{
			string endCommandChar = "";
			endCommandChar += *position;
			tokens.push_back(endCommandChar);
			position = storedInput.end();
			
		}
        else
		{
			++position;
        }
	}
	if(!commandAlreadyProcessed && semiTerminated)
	{
		/*for(int x =0; x<tokens.size(); x++)
			cout<<tokens[x]<<endl;*/
		parenthesisCount = 0;
		tokenIndex = 2;
		if(showCommand)
			tokenIndex = 1;
		else if(deleteFrom || update)
			tokenIndex = 0;
		endDelimiter = -1;
		if(tokens.size()>2)
		{
			string newTblName = "";
			if(insertFromRelation)
			{
				newTblName = Query();
				vector<Record> records = db.getTableByName(newTblName)->getTblRecords();
				for(int x = 0; x<records.size(); x++)
				{
					db.insertToTable(storedTable, records[x].getRecord());
				}
			}
			else if(deleteFrom)
			{
				newTblName = Query();
				newTblName = db.setDifference(storedTable, newTblName);
				db.dropTable(storedTable);
				db.getTableByName(newTblName)->changeTableName(storedTable);
			}
			else if(update)
			{
				newTblName = Query();
				vector<string> attr;
				vector<string> lit;
				for(int x = 0; x<changes.size(); x++)
				{
					attr.push_back(changes[x]);
					lit.push_back(changes[++x]);
				}
				string combine = db.setDifference(newTblName, storedTable);
				newTblName = db.updateTable(combine, newTblName, attr, lit);
				db.dropTable(storedTable);
				db.getTableByName(newTblName)->changeTableName(storedTable);
			
			}
			else if(tokens[3] == ";")
			{
				db.getTableByName(tokens[2])->changeTableName(tokens[0]);
			}
			else
			{
				newTblName = Query();
				Table* newTbl = db.getTableByName(newTblName);
					bool hasSpace = false;
					for(int c = 0; c < newTblName.size(); c++)
					{
						if(isspace(newTblName.at(c)))
						{
							hasSpace = true;
							break;
						}
					}
					if(!hasSpace)
					{
						newTbl->changeTableName(tokens[0]);
					}
			}
			if(showCommand)
				db.show(tokens[0]);
		}
		else
			db.show(tokens[0]);
	}

	db.dropQueryVec();
}
/****
	The Query() function iteratively checks the vector of tokens for different query commands.
	If a query is found, the respective function is called. If no query is found, then we save the 
	tables name because it does not need to be deleted.
****/
string Parser::Query()
{
	string tblName;
	while(tokenIndex < tokens.size())
	{
		if(tokens[tokenIndex] == "select")
		{
			++tokenIndex;
			tblName = callSelect();
			return tblName;
		}
		else if(tokens[tokenIndex] == "union")
		{
			++tokenIndex;
			tblName = callSetUnion();
			return tblName;
		}
		else if(tokens[tokenIndex] == "difference")
		{
			++tokenIndex;
			tblName = callSetDifference();
			return tblName;
		}
		else if(tokens[tokenIndex] == "crossProduct")
		{
			++tokenIndex;
			tblName = callCrossProduct();
			return tblName;
		}
		else if(tokens[tokenIndex] == "project")
		{
			++tokenIndex;
			tblName = callProject();
			return tblName;
		}
		else if(tokens[tokenIndex] == "naturalJoin")
		{
			++tokenIndex;
			tblName = callNaturalJoin();
			return tblName;
		}
		else if(tokens[tokenIndex] == "rename")
		{
			++tokenIndex;
			tblName = callRename();
			return tblName;
		}
		else
		{
			++tokenIndex;
		}
	}
	//changes the Name of the new table that was created to the name desired by the command
	Table* currentRelation = db.getTableByName(tblName);
	currentRelation->changeTableName(tokens[0]);		
}
/****
	The callSelect() function is called whenever the token vector contains "select" as a query.
	The function begins by checking for the next element in the token vector for the condition 
	associated with selection. If a closing parenthesis is found, this indicates that the next
	element is an atomic expression and we call the a separate function to handle the recursion
	named recursiveDescent().
****/
string Parser::callSelect()
{
	string tblName;
	string condition = tokens[tokenIndex];
	++tokenIndex;
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		tblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter +1;
		endDelimiter = tokenIndex;
	}
	else
	{
		// if here then reading relation-name
		string tblName = tokens[tokenIndex];
		++tokenIndex;
		if(tokens[tokenIndex] == ")")
		{	
			endDelimiter = tokenIndex;
			return db.recursiveDescent(tblName, condition);
		}
		else// if(startDelimiter == -1)
		{
			return db.recursiveDescent(tblName, condition);;
		}
		
	}
	return db.recursiveDescent(tblName, condition);
}
/****
	The callSetUnion() function takes a left hand side table and a right hand side table.
	Both are checked for arguments that can lead to recursion and then assigns the table 
	name returned from Query() to each. Once that is complete, the setUnion() function is 
	called which joins the left and right hand side tables.
****/
string Parser::callSetUnion()
{
	string tblName;	
	string lhsTblName;
	string rhsTblName;
	// recursively handling lhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		lhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = lhsTblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex = startDelimiter+1;		
		endDelimiter = tokenIndex;
	}
	else
	{
		lhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")")// && startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	// recursively handling rhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		rhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		rhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")" )//&& startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	return db.setUnion(lhsTblName, rhsTblName);
}
/****
	The callSetDifference() function takes a left and right hand side table and also checks for recursion 
	the same way as callSetUnion() does. If recursion is present, then Query() is called again to process 
	it. If not, then setDifference() is called and computes the difference of the two tables.
****/
string Parser::callSetDifference()
{
	string tblName;
	string lhsTblName;
	string rhsTblName;
	// recursively handling lhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		lhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = lhsTblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		lhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")")// && startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	// recursively handling rhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		rhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		rhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")" )//&& startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	return db.setDifference(lhsTblName, rhsTblName);
}
/****
	The callCrossProduct() function takes a left and right hand side table and checks for recursion. 
	If recursion is present, then Query() is called again. If not, then the function returns the 
	crossProduct() of the two tables.
****/
string Parser::callCrossProduct()
{
	string tblName;
	string lhsTblName;
	string rhsTblName;
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		lhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = lhsTblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		lhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")" )//&& startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;	
		}
		endDelimiter = tokenIndex;
	}
	// recursively handling rhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		rhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		rhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")")// && startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;	
		}
		endDelimiter = tokenIndex;
	}
	return db.crossProduct(lhsTblName, rhsTblName);
}
/****
	The callProject() function builds a vector of a list of attributes based off the tokens vector. 
	The function parses the tokens vector and only takes the elements that are attributes and disregards 
	any white spaces or commas. If the tokens vector contains an open parenthesis, the input contains
	recursion and the Query() function is called again. If no recursion exists, then the subset of 
	attributes contained in the attrList vector as well as the table name is passed the projection() 
	function.
****/
string Parser::callProject()
{
	// here tokens[tokenIndex] == some condition
	string tblName;
	string attributeList = tokens[tokenIndex];
	tokenIndex++;
	//build vector out of attributeList
	vector<string> attrList;
	int strIndex = 0;
	if(attributeList[strIndex] == '(')
	{
		strIndex++;
		bool endOfList = false;
		string nextAttr = "";
		for(int x = 1; x<attributeList.size()-1; x++)
		{
			if(attributeList[x] != ',' && attributeList[x] != ' ')
				nextAttr+= attributeList[x];
			else
			{
				attrList.push_back(nextAttr);
				nextAttr = "";
			}	
			
			if(x == attributeList.size()-2)
			{
				attrList.push_back(nextAttr);
			}
		}
		
	}
	else
		cerr << "FORMATTING ERROR IN RENAMING" << endl;	
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		tblName = Query();
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;	
		endDelimiter = tokenIndex;	
	}
	else
	{
		tblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")")// && startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	string retName = db.projection(tblName, attrList);
	return retName;
}
/****
	The callRename() function similarly builds a vector of a list of attributes. 
	The same type of parsing method of the token vector is used again and then recursion is checked for. 
	Once parsing and recursion is accounted for, the renaming() function is called to rename the 
	attributes in the relation and the name of the table is returned.
****/
string Parser::callRename()
{
	// here tokens[tokenIndex] == some condition
	string tblName;
	string attributeList = tokens[tokenIndex];
	tokenIndex++;
	//build vector out of attributeList
	vector<string> attrList;
	int strIndex = 0;
	if(attributeList[strIndex] == '(')
	{
		strIndex++;
		bool endOfList = false;
		string nextAttr = "";
		for(int x = 1; x<attributeList.size()-1; x++)
		{
			if(attributeList[x] != ',' && attributeList[x] != ' ')
				nextAttr+= attributeList[x];
			else if(attributeList[x] != ' ')
			{
				attrList.push_back(nextAttr);
				nextAttr = "";
			}	
			if(x == attributeList.size()-2 && nextAttr[0] != ' ')
			{
				attrList.push_back(nextAttr);
			}
		}
		
	}
	else
		cerr << "FORMATTING ERROR IN RENAMING" << endl;
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		tblName = Query();
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		tblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")")// && startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;		
		}
		endDelimiter = tokenIndex;
	}
	string retName = db.renaming(tblName, attrList);
	return retName;
}
/****
	The callNaturalJoin() function takes a left and right hand side table and checks for recursion.
	If recursion is present, then Query() is called again. If not, then the function returns the 
	naturalJoin() of the two tables.
****/
string Parser::callNaturalJoin()
{
	string tblName;
	string lhsTblName;
	string rhsTblName;
	// recursively handling lhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		lhsTblName = Query();	
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = lhsTblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;		
	}
	else
	{
		lhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")" )//&& startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;	
		}
		endDelimiter = tokenIndex;
	}	
	// recursively handling rhs of +
	if(tokens[tokenIndex] == "(")
	{
		int startDelimiter = tokenIndex;
		++parenthesisCount;
		++tokenIndex;
		rhsTblName = Query();
		//replace tokens[startDelimiter] with tblName
		tokens[startDelimiter] = tblName;
		//erase everything up to endDelimeter
		tokens.erase(tokens.begin()+startDelimiter+1, tokens.begin()+endDelimiter+1);
		tokenIndex =startDelimiter+1;
		endDelimiter = tokenIndex;
	}
	else
	{
		rhsTblName = tokens[tokenIndex];
		tokenIndex++;
		if(tokens[tokenIndex] == ")" )//&& startDelimiter != -1)
		{	
			endDelimiter = tokenIndex;
		}
		endDelimiter = tokenIndex;
	}
	return db.naturalJoin(lhsTblName, rhsTblName);
}


//==========================================================================================================
/****
	The parseRelationName() reads in a relation name and returns the relation name if it 
	follows the correct grammar.Otherwise, an error is thrown for having an invalid 
	name for a relation.
****/
string Parser::parseRelationName()
{
	string value;
	if(isalpha(*position))
	{		//checking for first char to be alpha
		while(*position != ';')
		{
			if(*position != ' ')
				value += *position;
			++position;
		}
		return value;
	}
	else
		cerr << "\nERROR: invalid name for a relation" << endl;
}
/****
	The parseCreate() function begins by checking the input for syntactic correctness 
	such as the right amount of opening and closing parentheses. Then string iterator 
	used is then checked to see if the next input is lower case and if it is, proceed 
	to store the following inputs as a value and it's associated type in a vector named 
	keys. After values are stored into the vector, the table is created.
****/
void Parser::parseCreate()
{
	//cout<<"called parseCreate"<<endl;
	vector<string> attributeList;
	vector<string> keys;
	string tblName;
	string value;
	string type;
	bool needsClose = false;
	while(*position != '(')
	{
		if(*position != ' ')
			tblName += *position;
		++position;
	}
	++position;
	bool entered;
	while(*position != ')')
	{	
		entered = false;
		while(islower(*position) || isupper(*position) || isdigit(*position) || *position == '_')
		{
			if(*position != ' ')
				value += *position;
			++position;
			entered = true;
		}
		while(*position == ' ' || *position == ',' || isupper(*position) || *position == '(' || isdigit(*position) || (needsClose && *position ==')'))
		{
			if(*position == '(')
				needsClose = true;
			if(*position == ')')
				needsClose = false;
			if(*position != ' ' && *position != ',')
				type += *position;
			++position;
			entered = true;
		}
		if(!entered)
		{
			++position;
		}
		else
		{
			attributeList.push_back(value+"["+type+"]");
			value = "";
			type = "";
		}
	}
	while(*position != '(')
		++position;
	while(*position != ';')
	{
		entered = false;
		while(islower(*position) || isupper(*position) || isdigit(*position) || *position == '_')
		{
			if(*position != ' ')
				value += *position;
			++position;
			entered = true;
		}
		if(!entered)
		{
			++position;
		}
		else
		{
			keys.push_back(value);
			value = "";
			type = "";
		}
	}
	/*cout<<"Creating table: "<<tblName<<endl;
	for(int x =0; x<attributeList.size(); x++)
		cout<<attributeList[x]<<endl;
	for(int y=0; y<keys.size(); y++)
		cout<<keys[y]<<endl;*/
	db.createTable(tblName, attributeList, keys);
}
/****
	The readReserveWord() function parses a string for any valid word. 
	The word is then checked by the function isReserveWord() function 
	and if the word is found to not be a reserve word, then readIdentifer() 
	is called which reads non reserve words.
****/
void Parser::readReserveWord()
{
	string reserveWord;
	int i = 0;
	while( position != storedInput.end() && *position != ';' && *position != ',' && *position != ')' && *position != '(' 
			&& ((isalpha(*position) && isupper(*position)) || *position == ' ' || *position == '<' || *position == '-'))
	{
		if(*position != ' ')
		{
			reserveWord += *position;
		}
		++position;
		++i;
	}
	if(!isReserveWord(reserveWord))
	{
		position -= i; // go back to where we started and call the read identifier function to read non reserve words
		readIdentifier();
	}
}
/****
	The readIdentifer() function parses a function based on whether is it an alphanumeric character
	or an underscore. We then check for the spaces and then store all presumed identifying words from 
	the string iterator. The identifying word is then passed to a function called isExpr() which checks 
	to see if the word is an expression and if it isn't, then the function stores the identifying word 
	into the tokens vector and returns the word.
****/
string Parser::readIdentifier()
{
	string identifierWord;
	// following the identifier grammar
	while( position != storedInput.end() && isalnum(*position) || *position == '_')
	{
		if(*position != ' ')
			identifierWord += *position;
		++position;
	}
	lastIdentifier = identifierWord;
	//return identifierWord;
	if(!isExpr(identifierWord))
	{
		tokens.push_back(identifierWord);
		return identifierWord;
	}
}
/****
	The parseArgs() function reads a string iterator until it reaches a semicolon. 
	If the iterator reads in a space, new line, or tab, it will increment the position 
	of the iterator. If not, then the function checks to see if the iterator is a alphabetic 
	character or a digit and proceeds to push it back to an array that contains arguments.
****/
vector<string> Parser::parseArgs()
{
	vector<string> args;
	int start = -1;
	int end = -1;
	int iter = 0;
	string currentArg;
	while(*position!=';')
	{
		if(*position == '"' && start == -1)
		{
			start = iter;
			++iter;
			++position;
			currentArg+='"';
		}
		else if(start != -1 && *position == '"')
		{
			end = iter;
			++iter;
			++position;
			currentArg+='"';
		}
		else if(start!= -1 && *position != ',' && *position!=')' && *position != ';' && *position!=' ')
		{
				currentArg+= (*position);
				++iter;
				++position;
		}
		else if(start!=-1 && (*position == ',' || *position == ')' || *position ==';'))
		{
			if(start != 0)
				currentArg = currentArg.substr(start,end-2);	
			else
				currentArg = currentArg.substr(start,end-1);
			args.push_back(currentArg);
			currentArg = "";
			iter = 0;
			start = -1;
			end = -1;
			++position;
		}
		else if(isdigit(*position))
		{
			while(!(*position == ',' || *position == ')' || *position ==';'))
			{
				currentArg += *position;
				++position;
			}
			args.push_back(currentArg);
		}
		else
		{
			++iter;
			++position;
		}
	
	}
	return args;
}
/****
	The isReserveWord() function checks a string through many if statements to see if it 
	matches a set of reserve words. Based off the reserve word that is found, the appropriate 
	function is called. If a function is called, then the return statement returns true, otherwise 
	false is returned stating that the string that was testing is not a reserve word.
****/
bool Parser::isReserveWord(string reserveWord)
{
	if(reserveWord == "OPEN")
	{ 
		commandAlreadyProcessed = true;
		db.openTable(parseRelationName());
	}
	else if(reserveWord == "CLOSE")
	{
		commandAlreadyProcessed = true;
		db.closeTable(parseRelationName());
	}
	else if(reserveWord == "WRITE")
	{
		cerr<<"Writing"<<endl;
		commandAlreadyProcessed = true;
		db.writeTable(parseRelationName());
	}
	else if(reserveWord == "EXIT")
	{
		cerr<<"Exiting"<<endl;
		commandAlreadyProcessed = true;
		exit = true;
		db.exit();
	}
	else if(reserveWord == "SHOW")
	{ 
		showCommand = true;
	}
	else if(reserveWord == "CREATETABLE")
	{ 
		//cout<<"calling parseCreate()"<<endl;
		commandAlreadyProcessed = true;
		parseCreate();
	}
	else if(reserveWord == "INSERTINTO")
	{ 
		storedTable = readIdentifier();
	}
	else if(reserveWord == "VALUESFROM")
	{ 
		commandAlreadyProcessed = true;
		vector<string> args = parseArgs();
		db.insertToTable(storedTable, args);
	}
	else if(reserveWord == "VALUESFROMRELATION")
	{
		insertFromRelation = true;
		tokenIndex = 0;
	}
	else if(reserveWord == "<-"){}
	else if(reserveWord == "UPDATE")
	{
		update = true;
		storedTable = readIdentifier();
	}
	else if(reserveWord == "SET")
	{
		while(!isReserveWord(readIdentifier()))
		{
			if(lastIdentifier!="")
				changes.push_back(lastIdentifier);
			else if(*position == '=')
			{
				position++;
			}
			else if(*position == ',')
			{
				position++;
			}
			else
				position++;
		}
	}
	else if(reserveWord == "WHERE")
	{
		tokens.clear();
		tokens.push_back("(");
		tokens.push_back("select");
		tokens.push_back(parseCondition());
		tokens.push_back(storedTable);
		tokens.push_back(")");
		tokens.push_back(";");
	}
	else if(reserveWord == "DELETEFROM")
	{
		deleteFrom = true;
		storedTable = readIdentifier();
	}
	else if(reserveWord == "VARCHAR"){}
	else if(reserveWord == "INTEGER"){}
	else if(reserveWord == "JOIN")
	{
		if(tokens[tokens.size()-1] != ")")
		{
			tokens.pop_back();
			tokens.push_back("naturalJoin");
			tokens.push_back(lastIdentifier);
		}
		else
		{
			int resolved = -1;
			vector<string>::iterator it = tokens.end();
			it = it - 2;
			while(resolved <0)
			{
				if((*it) == ")")
					resolved -=1;
				else if(*it == "(")
					resolved +=1;
				if(resolved != 0)
					it--;
			}
			tokens.insert(it, "naturalJoin");
		}
	}
	else
		return false;		
	return true;
}
/****
	The parseCondition() function is a function specially designed to extract a conditional 
	statement between two parentheses.
****/
string Parser::parseCondition()
{
	int parCount = 0;
	bool foundOpening = false;
	string condition = "";
	while(!foundOpening)
	{
		if(*position == '(')
		{
			foundOpening = true;
			parCount--;
			condition+='(';
		}
		position++;
	}
	while(parCount != 0 && *position != ';')
	{
		if(*position == '(')
			parCount--;
		else if(*position == ')')
			parCount++;
		condition+=*position;
		++position;
	}
	return condition;
}
/****
The isExpr() function checks to see if the string is an expression and configures special data 
that would not usually tokenize to a standard format that is better suited for the Query method 
to work with.
****/
bool Parser::isExpr(string reserveWord)
{
	if(reserveWord == "select")
	{
		tokens.push_back(reserveWord);
		//expect to read '(' followed by attr-list followed by ')' push all to vector
		while(*position == ' ')
			position++;
		if (*position != '(')
		{
			cerr << "ERROR Grammar issue missing ( on Select statement" << endl;
		}
		else
		{
			string condition = "";
			int ParenPairs = 0;
			condition += *position;
            ++ParenPairs; // the above operation reads in '('
			++position;
			while(ParenPairs != 0)
			{
				
                if (*position == '('){
					++ParenPairs;
                }
                else if (*position == ')')
				{
					--ParenPairs;
					condition += *position;
                }
				else if(ParenPairs != 0)
					condition += *position;
				++position;
            }
			condition = injectParen(condition);
			tokens.push_back(condition);
		}
	}
	else if(reserveWord == "rename")
	{
		tokens.push_back("rename");
		int parCount = 0;
		string columns;
		while(*position == ' ')
			position++;
		if(*position == '(')
			--parCount;
		while(parCount < 0)
		{
			columns += *position;
			position++;
			if(*position == '(')
				parCount--;
			if(*position == ')')
			{
				parCount++;
				if(parCount == 0)
				{
					columns += *position;
					position++;
				}
			}
		}
		tokens.push_back(columns);
	}
	else if(reserveWord == "project")
	{
		
		tokens.push_back("project");
		int parCount = 0;
		string columns;
		while(*position == ' ')
			position++;
		if(*position == '(')
			--parCount;
		while(parCount < 0)
		{
			columns += *position;
			position++;
			if(*position == '(')
				parCount--;
			if(*position == ')')
			{
				parCount++;
				if(parCount == 0)
				{
					columns += *position;
					position++;
				}
			}
		}
		tokens.push_back(columns);
		
	}
	else if(reserveWord == "+")
	{
		if(tokens[tokens.size()-1] != ")")
		{
			tokens.pop_back();
			tokens.push_back("union");
			tokens.push_back(lastIdentifier);
		}
		else
		{
			int resolved = -1;
			vector<string>::iterator it = tokens.end();
			it = it - 2;
			while(resolved <0)
			{
				if((*it) == ")")
					resolved -=1;
				else if(*it == "(")
					resolved +=1;
				if(resolved != 0)
					it--;
			}
			tokens.insert(it, "union");
		}
	}
	else if(reserveWord == "-")
	{
		if(tokens[tokens.size()-1] != ")")
		{
			tokens.pop_back();
			tokens.push_back("difference");
			tokens.push_back(lastIdentifier);
		}
		else
		{
			int resolved = -1;
			vector<string>::iterator it = tokens.end();
			it = it - 2;
			while(resolved <0)
			{
				if((*it) == ")")
					resolved -=1;
				else if(*it == "(")
					resolved +=1;
				if(resolved != 0)
					it--;
			}
			tokens.insert(it, "difference");
		}
	}
	else if(reserveWord == "*")
	{
		if(tokens[tokens.size()-1] != ")")//
		{//
			tokens.pop_back();//
			tokens.push_back("crossProduct");//
			tokens.push_back(lastIdentifier);//
		}//
		else//
		{//
			int resolved = -1;
			vector<string>::iterator it = tokens.end();
			it = it - 2;
			while(resolved <0)
			{
				if((*it) == ")")
					resolved -=1;
				else if(*it == "(")
					resolved +=1;
				if(resolved != 0)
					it--;
			}
			tokens.insert(it, "crossProduct");
		}//
	}
	
}
/****
	The injectParen() function is used to insert parenthesis before and after a condition 
	so that the format passed into the recursiveDescent() function are all the same and allows 
	the function to work properly.
****/
string Parser::injectParen(string condition)
{
	int start = -1;
	int finish = -1;
	int parCount = 0;
	if(condition[0] != '(')
	{
		parCount--;
	}
	else
		start = 1;
	for(int x = 0; x<condition.size(); x++)
	{
		if(condition[x] == '(')
			parCount--;
		if(condition[x] == ')')
			parCount++;
		if(parCount != 0 && (condition[x] == '&' || condition[x] == '|' || x == condition.size()-1))
		{
			finish = x;
			x++;
			if(start != -1 && finish != -1)
			{
				condition.insert(finish, ")");
				condition.insert(start, "(");
				x = x+2;
				parCount--;
			}
			else
				cerr<<"Didn't find start and finish"<<start<<" "<<finish<<endl;
			start = x+2;
		}
	}
	return condition;
}
/****
	The isStringUpper() function checks a string by each individual character to see if 
	it is an uppercase letter.
****/
bool Parser::isStringUpper(string word)
{
	for(int i = 0; i < word.size(); i++)
	{
		char currentChar = word.at(i);
		if(!isupper(currentChar))
			return false;
	}
	return true;
}













