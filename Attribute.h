#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <string>

using namespace std;

class Attribute
{
	string _name;
	string _type;
	int _maxSize;
	bool _key; 
	
public:

	Attribute();
	Attribute(string name, string type, int maxSize, bool key);
	~Attribute();
	
	//getters
	string getName() { return _name; }
	string getType() { return _type; }
	bool getKey() { return _key; }
	int getMaxSize() { return _maxSize; }
	
	//setters
	void changeAttributeName(string newAttribute) { _name = newAttribute; }
};

#endif