/*
	Parser.cpp
	
	
	Different commands:
	
	OPEN
	CLOSE
	WRITE
	EXIT
	SHOW ** have to worry about recursion with exr
	CREATE TABLE
	INSERT INTO **
	UPDATE
	DELETE FROM
	
	Different Queries:
	
	relation-name <- expr
	expr:=
			relation-name ** --> will always start with alpha 
			(expr)
			selection
			projection
			renaming
			uniondifference
			product
			natural-join



When we disect each command need a way to store the data, to correctly call the funcitons in Databas.cpp
	
*/

#include "Parser.h"

void Parser::parse(string input)
{
	if(input.size()>0)
	cout << "input: " << input << endl;
	string reserveWord;
	storedInput = input;
	position = storedInput.begin();
	
	while(position != storedInput.end())
	{
		if( *position == ' ' || *position == '\n' || *position == '\t' )
		{
			++position;
		}
		else if ( isalpha(*position) /*|| *position=='<' || *position=='-'*/) //check if starting input is characters
		{ 
			if ( (isupper(*position) && isupper(*(position+1))) /*|| (  (*position == '<') && (*(position+1)=='-') )*/  )
			{
				reserveWord = readReserveWord();
				
				cout << "HERE: " << reserveWord << endl;
			}
			else
			{
				readIdentifier();
			}
		}
		else if (*position=='<' || *(++position)=='-')
		{
			cout << "Query" << endl;
			reserveWord = readReserveWord();
		}
	}
}
string Parser::parseIt()
{
	string value;
	while(*position != ';')
	{
		cout<<*position;
		if(*position != ' ')
			value += *position;
		++position;
	}
	cout<<endl;
	cout<<"ParseIt() Value "<<value<<endl;
	return value;
}
void Parser::parseCreate()
{
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
		while(islower(*position))
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
			/*string item;
			item += '\"';
			item += value;
			item += "[";
			item += type;
			item += "]";
			item += '\"';
			cout<<item<<endl;*/
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
		while(islower(*position))
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
	db.createTable(tblName, attributeList, keys);
}
string Parser::readReserveWord()
{
	string reserveWord;
	int i = 0;
	while( position != storedInput.end() && *position != ';' && *position != ',' && *position != ')' && *position != '(' 
			&& (isupper(*position) || *position == ' ' || *position == '<' || *position == '-'))
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
		cout<<"wasn't a reserve word"<<endl;
		position-=i;
		readIdentifier();
	}
	return reserveWord;

}
vector<string> Parser::parseArgs()
{
	vector<string> args;
	while(*position!=';')
	{
		if( *position == ' ' || *position == '\n' || *position == '\t' )
		{
			++position;
		}
		else if(isalpha(*position) || isdigit(*position))
			args.push_back(readIdentifier());
		else
			++position;
	}
	return args;
}
bool Parser::isReserveWord(string reserveWord)
{
	cout << "||| " << reserveWord << " |||" << endl;
	if(reserveWord == "OPEN")
	{ 
		db.openTable(parseIt());
	}
	else if(reserveWord == "CLOSE")
	{
		db.closeTable(parseIt());
	}
	else if(reserveWord == "WRITE")
	{
		db.writeTable(parseIt());
	}
	else if(reserveWord == "EXIT")
	{
		exit = true;
	}
	else if(reserveWord == "SHOW")
	{ 
		db.show(parseIt()); 
	}
	else if(reserveWord == "CREATETABLE")
	{ 
		parseCreate();
	}
	else if(reserveWord == "PRIMARYKEY"){}
	else if(reserveWord == "INSERTINTO")
	{ 
		method = 1; 
		storedTable = readIdentifier();
	}
	else if(reserveWord == "VALUESFROM")
	{ 
		if(method == 1)
		{
			db.insertToTable(storedTable, parseArgs());
			method = 0;
		}
	}
	else if(reserveWord == "VALUESFROMRELATION")
	{
		if(method == 1)
		{
			string relation = readIdentifier();
			string logic = parseLogic();
			string tblName = readIdentifier();
			cout<<relation<<" "<<logic<<" "<<tblName<<endl;
			//db.insertToTable(insertTable, );
			method = 0;
		}
	}
	else if(reserveWord == "<-")
	{
		storedTable = lastIdentifier;
		string nextWord = readIdentifier();
		if(isExpr(nextWord))
		{
			parseExpression(nextWord);
		}
		else
		{
			string tblName2 = readIdentifier();
			
		}			
	}
	else if(reserveWord == "RELATION"){}
	else if(reserveWord == "UPDATE"){ method = 8; }
	else if(reserveWord == "SET"){}
	else if(reserveWord == "WHERE"){}
	else if(reserveWord == "DELETEFROM"){ method = 9;}
	else if(reserveWord == "VARCHAR"){ method = 10; }
	else if(reserveWord == "INTEGER"){ method = 11; }
	else
		return false;
	return true;
}
bool Parser::isExpr(string str)
{
	return true;
}
string Parser::parseExpression(string nextWord)
{
	if(nextWord == "select")
	{
		cerr << "here" << endl;
		string logic = parseLogic(); // extracts the condition
		while(*position == ' ')
			++position; // skips white space to atomic-expr
		string selectTable = readIdentifier(); // ERROR: Doesnt account for atoic-expr
		//string lookup = db.recursiveDescent(selectTable, logic);
		int checkum = db.getTableIndex(lookup);
		if(checkum >= 0){
		//	db.getDBtable(checkum)->changeTableName(storedTable);
		}
		return storedTable;
	}
	else if(nextWord == "rename")
	{}
	else if(nextWord == "project")
	{}
	else if(nextWord == "union")
	{}
	else if(nextWord == "difference")
	{}
	else if(nextWord == "product")
	{}
	else if(nextWord == "natural-join")
	{}
}
string Parser::parseLogic()
{
	string logic;
	bool closed = false;
	closed = false;
	int pairs = 0;
	while(!closed)
	{
		if(*position == '(')
		{
			pairs--;
		}
		if(*position == ')')
		{
			pairs++;
			if(pairs == 0)
				closed = true;
		}
		if(*position != ' ')
			logic += *position;
		++position;
	}
	cout<<logic<< endl;
	return logic;
}

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
	//cout << "identifier word: " << identifierWord << endl;
	lastIdentifier = identifierWord;
	return identifierWord;
}














