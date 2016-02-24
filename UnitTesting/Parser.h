/*
	Parser.h
*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "Database.h"



class Parser
{
	//Database db; // need a connection to db to be able to execute commands/queries
	Database db;
	vector<string> tokens;
	string storedInput;
	string::iterator position;
	bool exit;
	bool commandAlreadyProcessed;
	int parenthesisCount;
	int tokenIndex;
	//int startDelimiter;
	int endDelimiter;
	bool showCommand;
	bool insertFromRelation;
	bool deleteFrom;
	bool update;
	vector<string> changes;
	vector<string> tablePersist;
	
	int method;
	string lastIdentifier;
	string storedTable;
	
	
	
public:
	Parser() {exit = false; commandAlreadyProcessed = false; }
	~Parser() {}
	
	void parse(string input);
	void readReserveWord();
	string readIdentifier();
	string parseRelationName();
	bool isStringUpper(string word);
	
	string Query();
	string callSelect();
	string callSetUnion();
	string callSetDifference();
	string callCrossProduct();
	string callProject();
	string callNaturalJoin();
	string callRename();
	string injectParen(string condition);
	string parseCondition();

	bool isReserveWord(string command);
	bool getExit(){return exit;}
	void parseCreate();
	vector<string> parseArgs();
	bool isCommand(string str);
	bool isExpr(string str);
	
	
};
#endif