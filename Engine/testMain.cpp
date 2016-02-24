#include <iostream>
#include "Database.h"

using namespace std;


/* NOTE FOR DEVS: currently the relational algebra functions are calling SHOW() can simply take that out 
                later so it doesn't print the tables everytime, just needed it for debugging purposes
				- BRANDON: Show members about the cats_or_dogs tblName idea from example input
*/

// NOTE FOR GRADER: If you notice in the query tables that are printed from the relational algebra the tables are\
//  			never saved those tables to the folder /DatabaseFiles

int main()
{
	try
	{
		cout << "testing engine" << endl;
		
		Database db;
// =================  OPEN ====================================================================
		db.openTable("test");
		db.show("test");
		
		cout << "\n>> trying to open table that DNE";
		db.openTable("tableDNE");
		
// =================  INSERT ====================================================================
	    vector<string> literals;
	    literals.push_back("Brandon");
	    literals.push_back("3/15/2010");
	    literals.push_back("Toyota");
	    literals.push_back("Corolla");
	    literals.push_back("2010");
	    literals.push_back("White");
	    db.insertToTable("test", literals); // NO ERROR
		
		
		// INSERT test 2 causing error on KEY
		cout << "\n>> trying to insert a value that is not UNIQUE for a KEY attribute";
		literals.clear();
	    literals.push_back("Bob"); // error on unique value
	    literals.push_back("3/15/2010");
	    literals.push_back("Toyota");
	    literals.push_back("Corolla");
	    literals.push_back("2010");
	    literals.push_back("White");
		// ERROR: UNIQE VALUE FOR NAME
	    db.insertToTable("test", literals);
		
		
		// INSERT test 2 causing error on VARCHAR max length
		cout << "\n>> trying to insert a value that is not within the specified varchar max length";
		literals.clear();
	    literals.push_back("Bobbbbbbbbbbbbbbbbbbbbbb"); // error on length of string
	    literals.push_back("3/15/2010");
	    literals.push_back("Toyota");
	    literals.push_back("Corolla");
	    literals.push_back("2010");
	    literals.push_back("White");
		// ERROR: LENGTH OF STRING 
	    db.insertToTable("test", literals);
	    
	    // double checking worked correctly
	    cout << '\n' << '\n';
	    db.show("test");
		
// =================  WRITE ====================================================================		

		db.writeTable("test");
		
		cout << "\n\n>>Trying to write tableDNE.db but it DNE "<<endl; 
		db.writeTable("tableDNE");
		
		
// =================  DELETE ====================================================================		
		vector<string> comp;
		comp.push_back("Brandon");
		db.deleteFromTable("test", "*Name", "==", comp);
		cout << "\n\n>>Deleting Record containing Name == \"Brandon\"" << endl;
		db.show("test");
		db.writeTable("test");
		
		
		comp.clear();
		/*
		
		//UNCOMMENT THIS TO SEE THE DELETE WORK, THIS WILL WORK CORRECTLY JUST NEED THE VALUES FOR
		//FUTURE FUNCTION CALLS
		
		comp.push_back("2005");
		comp.push_back("1999");
		db.deleteFromTable("test", "Year", "!=", comp);
		cout << '\n' << '\n';
		db.show("test");
		
		*/
		
		
// =================  CREATE ====================================================================
		
	    vector<string> attributeList;
	    attributeList.push_back("*Attr1[varchar(20)]");
	    attributeList.push_back("Attr2[int]");
	    attributeList.push_back("Attr3[varchar(20)]");
	    attributeList.push_back("*Attr4[int]");
	    vector<string> keyAttributeList;
		
		string tblName = "NewTable";
		int i = 1;
		tblName = tblName + to_string(i);
	    db.createTable(tblName, attributeList, keyAttributeList);
	    cout << '\n' << '\n';
	    
	    //inserting record to make sure it works
		literals.clear();
	    literals.clear();
	    literals.push_back("BILLLLLYYY");
	    literals.push_back("123");
	    literals.push_back("WTF");
	    literals.push_back("69");
	    db.insertToTable(tblName, literals);
		db.show(tblName);
		db.writeTable(tblName);

		
// =================  UPDATE ====================================================================

/*
1.)
	UPDATE test 
	SET Color = "Ew", Make = "EWW"
	WHERE Year == 1990
	
2.)
	UPDATE test 
	SET Color = "Ew", Make = "EWW"
	WHERE Year <= 1999

3.)
	UPDATE test 
	SET Color = "Ew", Make = "EWW"
	WHERE Color == White

If you combine all 3 above statements it produced the following SQL
by breaking up these commands I can make it easier to construct the final product

FINAL.)
	UPDATE test 
	SET Color = "Ew", Make = "EWW"								
	WHERE Year == 1990, Year <= 1999  Color == "White"
*/
	db.openTable("testToUpdate");
	cout << ">> completeing SQL statement 1.) " << endl;
	vector<string> attrNames;
			attrNames.push_back("Color");
			attrNames.push_back("Make");
		vector<string> literals1;
			literals1.push_back("EW");
			literals1.push_back("EWW");
		vector<string> comp1;
			comp1.push_back("1990");
			//comp.push_back("1999");
		//updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals)
		db.updateTable("testToUpdate", "Year", "==", comp1, attrNames, literals1);
		db.show("testToUpdate");
			
			// UPDATE for second conditionParser
			cout << ">> completeing SQL statement 2.) " << endl;
			vector<string> attrNames2;
				attrNames2.push_back("Color");
				attrNames2.push_back("Make");
			vector<string> literals2;
				literals2.push_back("EW");
				literals2.push_back("EWW");
			vector<string> comp2;
				comp2.push_back("1999");
				//comp.push_back("1999");
			//updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals)
			db.updateTable("testToUpdate", "Year", "<=", comp2, attrNames2, literals2);
			db.show("testToUpdate");
			
			
				// UPDATE for third conditionParser
				cout << ">> completeing SQL statement 3.) " << endl;
				vector<string> attrNames3;
					attrNames3.push_back("Color");
					attrNames3.push_back("Make");
				vector<string> literals3;
					literals3.push_back("EW");
					literals3.push_back("EWW");
				vector<string> comp3;
					comp3.push_back("White");
					//comp.push_back("1999");
				//updateTable(string tableName, string attrName, string op, vector<string> &comp, vector<string> &attrNames, vector<string> &literals)
				db.updateTable("testToUpdate", "Color", "==", comp3, attrNames3, literals3);
				cout << "\n\n FINAL result after the 3 update commands"<<endl;
				db.show("testToUpdate");
		db.closeTable("testToUpdate");
				
// =================  CLOSE ====================================================================
		cout << "\n>>Close table testToUpdate" << endl;
		db.closeTable("testToUpdate");
		cout << ">>Trying to show testToUpdate but will produce error because it is no longer in the Database." << endl;
		db.show("testToUpdate");

// =================  RENAMING ====================================================================
		cout << ">> RENAMING A column in the testRename table to \"NEW NAME\"\n";
		db.openTable("testRename");
		int x = 6;
		vector<string> newATRnames(6);
		newATRnames.insert(newATRnames.begin()+3,"NEW NAME");
		db.renaming("testRename", newATRnames);
		db.show("testRename");
		db.closeTable("testRename");
		
// =================  PROJECTION  ====================================================================
		
		cout << "\n\n>>Projecting DOP and Model from file test.db\n";
		vector<string> attList;
		attList.push_back("DOP");
		attList.push_back("Model");
		db.projection("test", attList);

// =================  UNION  ====================================================================
		
		
		db.openTable("test2");
		cout << "\n\n>>test.db UNION test2.db, the record that is printed twice is JILL b/c the data is same in both files\n";
		db.setUnion("test", "test2");


// =================  DIFFERENCE  ====================================================================

		cout << "\n\nDifference between test.db and test2.db\n";
		db.setDifference("test", "test2");

		
// =================  CROSS PRODUCT  ====================================================================

		//ADAM: you had test1 for this function but there was no test1.db so i made a single attribute table
		// i don't remember exactly what you had so yea.
		db.openTable("test1");
		cout << "\n\n>>test1.db CROSS PRODUCT test2.db\n";
		db.crossProduct("test1", "test2");
		
// =================  NATURAL JOIN  ====================================================================		
		
		db.openTable("test3");
		db.openTable("test4");
		cout << "\n\n>>NATURAL JOIN with test3.db and test4.db\n";
		db.naturalJoin("test3", "test4");	

// =================  SELECT  ====================================================================
		
		cout << "\n\n\nSELECTION \n\n";
		//db.conditionParser("Model=='F-150'");
		//db.conditionParser("color==('Red'||'Blue')");
		//db.conditionParser("Make=='Ford'");
		string goobyPls = db.recursiveDescent("test", "(Model==\"F-150\")&&((Color==(Red||Blue))||(Make==\"Ford\"))");
		//string goobyPls = db.recursiveDescent("test", "(Color!=(Yellow||Red))||(Model==\"Mustang\")");
		db.show(goobyPls);


		
// **************************** **************************** ****************************

		//remove
		//db.show("test");
		//	cout << '\n' << '\n';
		//db.openTable("test2");
		//db.show("test2");
		//db.openTable("NewTable");
		//db.show("NewTable");
		
		
		

		
		
		

		

		

		
		//INTERSECT
		//db.setIntersect("test", "test2");
		
		//SELECTION
		//db.conditionParser("Model=='F-150'");
		//db.conditionParser("color==('Red'||'Blue')");
		//db.conditionParser("Make=='Ford'");
		//string goobyPls = db.recursiveDescent("test", "(Model==\"F-150\")&&((Color==(Red||Blue))||(Make==\"Ford\"))");
		//string goobyPls = db.recursiveDescent("test", "(Color!=(Yellow||Red))||(Model==\"Mustang\")");
		//db.show(goobyPls);
		
		// SHOW
	    //db.show("test");
/*
	    
	    // ****************************
	    
	    // WRITE
	    db.writeTable("test");
	    // ****************************


	   

			    /*
			    // CORRECT ERROR can't have the same values in first and last attribute
			    literals.clear();
			    literals.push_back("BOB");
			    literals.push_back("12345");
			    literals.push_back("WTF");
			    literals.push_back("69");
			    db.insertToTable("NewTable", literals);
			    */
	    cout << '\n' << '\n';
	    //db.show("NewTable");


	    // ****************************

	}
	catch(const runtime_error& error){
		cerr << error.what() << endl;
	}
}