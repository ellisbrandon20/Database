#include "Table.h"
#include <string>
#include "string.h"
#include "stdlib.h"
#include "Record.h"
#include "Attribute.h"


/*****
 Adam: I added a member to the class: Table, bool _query
 this will allow us to store ALL .db/queries in the database
 the reason we need this is one for a way to temporary storage of the query while program is running
 and two so when the CLOSE command is executed it says to save all changes to relations but 
 do not save queries so this bool will allow us to easily do this task.
*****/

bool relationalAlgebraBool = true;
using namespace std;

bool isUnionCompatible(Table R, Table S) 
{
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
void conditionParser(string condition)
{
	while(i < condition.size())
	{
		string attName;
		string comp;
		string ops;
		string next;
		while(condition.at(condition.begin()+i)!= ('<' || '>' || '=' || '!'))
		{
			if(condition.at(condition.begin()+i !=(' '||'('||')'))
				attName.push_back(condition.at(condition.begin()+i))
		}
		cout<<attName<<endl;
		while(condition.at(condition.begin()+i)== ('<' || '>' || '=' || '!'))
		{
			if(condition.at(condition.begin()+i!=(' '||'('||')'))
				ops.push_back(condition.at(condition.begin()+i))
		} 
		cout<<ops<<endl;
		while(condition.at(condition.begin()+i)!= ('|'||'&'))
		{
			if(condition.at(condition.begin()+i)!=(' '||'('||')'))
				comp.push_back(condition.at(condition.begin()+i))
		}
		cout<<comp<<endl;
		
		/*if(condition.at(condition.begin()+i) == '|'))
			selection(R, attName1, attName2, ops);
		else if(condition.at(condition.begin()+i) == '&'))
			selection(R, attName1, attName2, ops);
		else
			gg = selection(R, attName1, attName2, ops);*/
}
Table* selection(string tableRName, string condition)
{
	int tblRIndex = getTableIndex(_dbTables, tableRName);
	if (tblRIndex < 0)
	{
		cerr << "\nERROR: Can't open " << tableRName << ".db, because it does not exist" << endl;
	}
	else
	{
		Table R = _dbTables[tblRIndex];
		vector<Record> relation = R.getTblRecords();
		vector<Attribute> attr;
		vector<Record> selected;
		for(int x = 0; x<relation.size(); x++) //moves through table relation
		{
			if(relation[x].getRecord()[condCol] == condition)
				selected.push_back(relation[x]);
		}
		Table* selectionTable = new Table("CrossProduct",attr, selected, relationalAlgebraBool);
		return selectionTable;
	}
}
Table* projection(Table R, vector<string> projAttr) //ready for testing
{
	vector<int> cols;
	vector<Attribute> attr;
	vector<Record> projection;
	for(int x = 0; x<R.getTblAttributes().size(); x++) //moves through Table R attributes list
	{
		for(int y = 0; y<projAttr.size(); y++) //finds attributes desired within Table R
		{
			vector<Attribute> tmpV = R.getTblAttributes();
			if(tmpV[x].getName() == projAttr[y]) 
			{
				attr.push_back(R.getTblAttributes()[y]);
				cols.push_back(x);
			}
		}
	}
	for(int z = 0; z < R.getTblRecords().size(); z++) //moves through table R
	{
		Record rec;
		Record currentRec = R.getTblRecords()[z];
		for(int i = 0; i<cols.size(); i++)
		{
			rec.getRecord().push_back(currentRec.getRecord()[cols[i]]);
		}
		projection.push_back(rec);
	}
	Table* projTable = new Table("projection",attr,projection, relationalAlgebraBool);
	return projTable;
}
void renaming(Table t, vector<string> names) //ready for testing
{
	for(int x = 0; x<t.getTblAttributes().size(); x++) //moves through Attributes list
	{
		if(!names[x].empty()) // assigns name to new name if new name exists
		{
			t.getTblAttributes()[x].getName() = names[x];
		}
	}
}
Table* setUnion(Table R, Table S) //ready for testing
{
	if(isUnionCompatible)
	{
		vector<Record> unionVector;
		vector<Attribute> attr = R.getTblAttributes();
		while(R.getTblRecords().size()>0)//traverses R table
		{
			int rem = 0;
			bool found = false;
			for(int x = 0; x < S.getTblRecords().size(); x++)//checks for a copy of R[0] in Q
			{
				if(R.getTblRecords()[0] == S.getTblRecords()[x])
				{
					found = true;
					rem = x;
				}
			}
			unionVector.push_back(R.getTblRecords()[0]);
			R.getTblRecords().erase(R.getTblRecords().begin()); //erases records that have already been checked
			if(found)
			{
				S.getTblRecords().erase(S.getTblRecords().begin()+rem); //removes redundant records
			}
		}
		for(int y = 0; y<S.getTblRecords().size(); y++) //adds remaining records
		{
			unionVector.push_back(S.getTblRecords()[y]);
		}
		Table* unionTable = new Table("CrossProduct",attr,unionVector, relationalAlgebraBool);
		return unionTable;
	}
	else
		cerr<<"Not union compatible"<<endl;
}
Table* setDifference(Table R, Table S) //ready for testing
{
	if(isUnionCompatible)
	{
		vector<Attribute> attr = R.getTblAttributes();
		vector<Record> differenceVector;
		while(R.getTblRecords().size()>0)//traverses R table
		{
			int rem = 0;
			bool found = false;
			for(int x = 0; x < S.getTblRecords().size(); x++)//checks for a copy of R[0] in Q
			{
				if(R.getTblRecords()[0] == S.getTblRecords()[x])
				{
					found = true;
					rem = x;
				}
			}
			if(found)
			{
				S.getTblRecords().erase(S.getTblRecords().begin()+rem); //removes Records that have redundancies
			}
			else
			{
				differenceVector.push_back(R.getTblRecords()[0]); //adds Records that have no redundancies
			}
			R.getTblRecords().erase(R.getTblRecords().begin());
		}
		for(int y = 0; y<S.getTblRecords().size(); y++) //adds remaining Records
		{
			differenceVector.push_back(S.getTblRecords()[y]);
		}
		Table* DifferenceTable = new Table("SetDifference",attr,differenceVector, relationalAlgebraBool);
		return DifferenceTable;
	}
	else
		cerr<<"Not union compatible"<<endl;
}
Table* crossProduct(Table R, Table Q) //ready for testing
{
	vector<Record> fusion;
	vector<Attribute> attr = R.getTblAttributes();
	for(int x = 0; R.getTblRecords().size(); x++) //moves through all records of R
	{
		Record rec = R.getTblRecords()[x];
		for(int y = 0; Q.getTblRecords().size(); y++) //moves through all records of Q
		{
			for(int z = 0; z<Q.getTblAttributes().size(); z++) //appends every attribute of current Q record to current R record
			{
				if(y == 0)
				{
					attr.push_back(Q.getTblAttributes()[z]);
				}
				rec.getRecord().push_back(Q.getTblRecords()[y].getRecord()[z]);
			}
		}
		fusion.push_back(rec);
	}
	Table* crossTable = new Table("CrossProduct",attr,fusion, relationalAlgebraBool);
	return crossTable;
}
Table* naturalJoin(Table R, Table Q, string common) //ready for testing
{
	vector<Record> jointRec;
	vector<Attribute> jointAttr;
	int RcommonCol;
	int QcommonCol;
	bool RfoundCommon = false;
	bool QfoundCommon = false;
	for(int x = 0; x< R.getTblAttributes().size(); x++) //finds common column for table R
	{
		jointAttr.push_back(R.getTblAttributes()[x]);
		if(R.getTblAttributes()[x].getName() ==  common)
		{
			RcommonCol = x;	
			RfoundCommon = true;
		}
	}
	for(int x = 0; x< Q.getTblAttributes().size(); x++) //finds common column for table Q
	{
		if(Q.getTblAttributes()[x].getName() ==  common)
		{
			QcommonCol = x;	
			QfoundCommon = true;
		}
		else
			jointAttr.push_back(Q.getTblAttributes()[x]);
	}
	if(RfoundCommon && QfoundCommon)
	{
		for(int z = 0; z<R.getTblRecords().size(); z++) // traverses Table R records
		{
			for(int i = 0; i<Q.getTblRecords().size(); i++) //traverses Table Q records
			{
				if(R.getTblRecords()[z].getRecord()[RcommonCol] == Q.getTblRecords()[i].getRecord()[QcommonCol])//matches common columns and unions all other attributes
				{
					Record rec = R.getTblRecords()[z];
					for(int n = 0; n<Q.getTblRecords()[i].getRecord().size(); n++)
					{
						if(n!=QcommonCol)
							rec.getRecord().push_back(Q.getTblRecords()[i].getRecord()[n]);
					}
					jointRec.push_back(rec);
				}
			}
		}
	}
	Table* joinedTable = new Table("NaturalJoined",jointAttr,jointRec, relationalAlgebraBool);
	return joinedTable;
}

