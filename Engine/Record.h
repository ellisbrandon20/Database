#ifndef _RECORD_H
#define _RECORD_H

#include <vector>
#include <string>

using namespace std;

class Record
{
	vector<string> _recordValues;
	
public:

	Record();
	Record(vector<string> &newRecords);
	~Record();
	
    //getters
    vector<string> getRecord() { return _recordValues; }
	
	//setters
	void setVector(vector<string> &newRecords);
	void updateRecord(int vecStringIndex, string newValue);


    //overload
    bool operator==(Record& rhsRecord);
};

#endif