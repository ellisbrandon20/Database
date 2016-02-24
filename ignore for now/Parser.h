/*
	Parser.h
*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "Database.h"



class Parser
{
	//Database db; // need a connection to db to be able to execute commands/queries

	string storedInput;
	string::iterator position;
	int method;
	string lastIdentifier;
	string storedTable;
	Database db;
	bool exit;
	
public:
	Parser() {exit = false;}
	~Parser() {}
	
	void parse(string input);
	string parseIt();
	string readReserveWord();
	string readIdentifier();
	bool isReserveWord(string command);
	bool getExit(){return exit;}
	void parseCreate();
	vector<string> parseArgs();
	bool isCommand(string str);
	string parseLogic();
	bool isExpr(string str);
	string parseExpression(string nextWord);
	
	
};
#endif