#include <iostream>
#include <istream>
#include <string>
#include <vector>

using namespace std;



/*



	For removeSale() if we can get delete from to work with multiple conditions then add the "pending" as a condition





*/



bool isNumber(string& input)
{
	string::iterator iter = input.begin();
	while(iter != input.end() && isdigit(*iter))
		iter++;
	
	return !input.empty() && iter == input.end();
}

string removeSale()
{
	cout << "\n\n";
	cout << " ---- Remove a pending Sale ---- " << endl;
	cout << "What do you want to delete by?" << endl;
	cout << "1. Order ID" << endl;
	cout << "2. Employee" << endl;
	cout << "3. Customer" << endl;
	cout << "4. Model" << endl;
	cout << ">";
	int deleteCommand;
	cin >> deleteCommand;
	
	string sql = "";
	string orderID = "";
	string orderOP = "";
	string employeeID = "";
	string employeeIDop = "";
	string customerOP = "";
	string customer = "";
	string model = "";
	string modelOP = "";
	string require = ""; // if == "yes" place "&&" in the sql otherwise place "||" in sql
	string require2 = ""; // if == "yes" place "&&" in the sql otherwise place "||" in sql
	
	switch(deleteCommand)
	{
		case 1:
			//Order ID
			cout << "\nWhat is the Order ID number?" << endl;
			cout << ">";
			
			cin >> orderID;

			cout << "\nAre you looking for Order ID eqaul to(=) or not eqaul(!=) to?" << endl;

			cout << ">";
			cin >> orderOP;
			
			if(!isNumber(orderID)) return "ERROR";
			
			if (orderOP == "==")
				sql = "(DELETE FROM sales WHERE OrderID == " + orderID + ");";
			else
				sql = "(DELETE FROM sales WHERE OrderID != " + orderID + ");";

			break;
		case 2:
			//Employee ID
			cout <<"\nWhat is the Employee ID?" << endl;
			cout << ">";
			cin >> employeeID;
			if(!isNumber(employeeID)) return "ERROR";
			
			//find op
			cout << "Are you deleting records that are eqaul (=) to or not equal (!=) to Employee ID?"
				 << endl;
			cout << ">";
			cin >> employeeIDop;
			
			if(employeeIDop == "=")
				sql = "DELETE FROM sales WHERE (employeeID == " + employeeID + ");";
			else
				sql = "DELETE FROM sales WHERE (employeeID != " + employeeID + ");";
			
			break;
		case 3:
			//Customer
			cout <<"\nWhat is the Customer's name?" << endl;
			cout << ">";
			cin >> customer;
			
			//find op
			cout << "Are you deleting records that are eqaul (=) to or not equal (!=) to Customer name?"
				 << endl;
			cout << ">";
			cin >> customerOP;
			
			if(customerOP == "=")
				sql = "DELETE FROM sales WHERE (customerName == " + customer + ");";
			else
				sql = "DELETE FROM sales WHERE (customerName != " + customer + ");";

			break;
		case 4:
			//Model
			cout <<"\nWhat is the Model name of the car?" << endl;
			cout << ">";
			cin >> model;
			
			//find op
			cout << "Are you deleting records that are eqaul (=) to or not equal (!=) to Model name"
				 << endl;
			cout << ">";
			cin >> modelOP;
			
			if(modelOP == "=")
				sql = "DELETE FROM sales WHERE (model == " + model + ");";
			else
				sql = "DELETE FROM sales WHERE (model != " + model + ");";
			
			break;
		default:
			break;
			
	}
	return sql;
}
string emplyoeeStatsAttribute(char attribute)
{
	//cout << "attr # " << attribute << endl;
	if (attribute == '0')
		return "orderID";
	else if (attribute == '1')
		return "employeeName";
	else if (attribute == '2')
		return "customerName";
	else if (attribute == '3')
		return "make";
	else if (attribute == '4')
		return "model";
	else if (attribute == '5')
		return "cost";
	else if (attribute == '6')
		return "dateOfPurchase";
	else if (attribute == '7')
		return "financingRate";
	else	
		return "ERROR";
}
string employeeStats()
{
	cout << "\n\n";
	cout << " ---- View Employee Stats ---- " << endl;
	
	cout << "\nDo you want to search by Employee ..." << endl;
	cout << "1. ID" << endl;
	cout << "2. Name" << endl;
	string search;
	cout << ">";
	cin >> search;
	
	string joinedTBL;
	string selectJoinedTBL;
	if(search == "1")
	{
		cout << "\nPlease enter the Employee's ID" << endl;
		cout << ">";
		string employeeID;
		cin >> employeeID;
		if(!isNumber(employeeID)) return "ERROR";
		
		joinedTBL = "(sales JOIN owns)";
		selectJoinedTBL = "(select (employeeID == " + employeeID 
								+ ") " + joinedTBL + ")";
	}
	else if (search == "2")
	{
		cout << "\nPlease enter the Employee's Name" << endl;
		cout << ">";
		string employeeName;
		cin >> employeeName;
		
		joinedTBL = "(sales JOIN owns)";
		selectJoinedTBL = "(select (employeeName == " + employeeName 
								+ ") " + joinedTBL + ")";
	}
	else
	{
		cout << "Sorry you did not enter valid input we were looking for \"1\" or \"2\"" << endl;
		return "ERROR";
	}
		//find out what the user wants to project
		vector<string> VECattributes;
		VECattributes.push_back("0. Order ID");
		VECattributes.push_back("1. Employee Name");
		VECattributes.push_back("2. Customer Name");
		VECattributes.push_back("3. Make of Car");
		VECattributes.push_back("4. Model of Car");
		VECattributes.push_back("5. Cost of Car");
		VECattributes.push_back("6. Date of Purchase");
		VECattributes.push_back("7. Financing Rate");
		
		
		cout << "\nWhat items would you like to look at about this employee?" << endl;
		for(int i = 0; i < VECattributes.size(); i++)
			cout << VECattributes[i] << endl;
		
		cout << "\nPlease enter one number at a time followed by a space then hit enter." << endl;
		cout << "Example input: 7 5 4 0" << endl;
		cout << ">";
		string attribute;
		vector<string> projectAttributes; // attributes to be projected
		cin.ignore();
		getline(cin, attribute);
		string::iterator iter = attribute.begin();
		while(iter != attribute.end())
		{
			if(*iter == ' '){
				iter++;
				continue;
			}
			
			string pushAttribute = emplyoeeStatsAttribute(*iter);
			projectAttributes.push_back(pushAttribute);
			++iter;
		}

		
		//============================
		string listAttributes = "(";
		for(int j = 0; j < projectAttributes.size(); j++)
		{
			listAttributes += projectAttributes[j];
			if(j<projectAttributes.size()-1)
				listAttributes += ",";
		}
		listAttributes += ")";
		
		
		//cout << "::" << selectJoinedTBL << endl;
		//cout << "SQL STATEMENT: ";
		string sql = "project " + listAttributes + " " + selectJoinedTBL;
		//cout << sql << endl;
		return sql;


}
string CompareEmployeeStats()
{
	cout << "\n\n";
	cout << " ---- Compare Employee Stats ---- " << endl;
	cout << "\nDo you want to search by Employee ..." << endl;
	cout << "1. ID" << endl;
	cout << "2. Name" << endl;
	string search;
	cout << ">";
	cin >> search;
	
	string joinedTBL;
	string selectJoinedTBL_1, selectJoinedTBL_2;
	
	if(search == "1")
	{
		joinedTBL = "(sales JOIN owns)";
		
		cout << "\nPlease enter the  first Employee's ID" << endl;
		cout << ">";
		string employeeID_1;
		cin >> employeeID_1;
		if(!isNumber(employeeID_1)) return "ERROR";
		
		
		selectJoinedTBL_1 = "(select (employeeID == " + employeeID_1 
								+ ") " + joinedTBL + ")";
								
		cout << "\nPlease enter the  second Employee's ID" << endl;
		cout << ">";
		string employeeID_2;
		cin >> employeeID_2;
		if(!isNumber(employeeID_2)) return "ERROR";
		
		selectJoinedTBL_2 = "(select (employeeID == " + employeeID_2 
								+ ") " + joinedTBL + ")";
								
								
	}
	else if (search == "2")
	{
		joinedTBL = "(sales JOIN owns)";
		
		cout << "\nPlease enter the  first Employee's Name" << endl;
		cout << ">";
		string employeeName_1;
		cin >> employeeName_1;
		
		selectJoinedTBL_1 = "(select (employeeName == " + employeeName_1
								+ ") " + joinedTBL + ")";
								
		cout << "\nPlease enter the  second Employee's ID" << endl;
		cout << ">";
		string employeeName_2;
		cin >> employeeName_2;
		
		selectJoinedTBL_2 = "(select (employeeID == " + employeeName_2 
								+ ") " + joinedTBL + ")";
	}
	else
	{
		cout << "Sorry you did not enter valid input we were looking for \"1\" or \"2\"" << endl;
		return "ERROR";
	}
		
		//find out what the user wants to project
		vector<string> VECattributes;
		VECattributes.push_back("0. Order ID");
		VECattributes.push_back("1. Employee Name");
		VECattributes.push_back("2. Customer Name");
		VECattributes.push_back("3. Make of Car");
		VECattributes.push_back("4. Model of Car");
		VECattributes.push_back("5. Cost of Car");
		VECattributes.push_back("6. Date of Purchase");
		VECattributes.push_back("7. Financing Rate");
		
		
		cout << "\nWhat items would you like to look at about this employee?" << endl;
		for(int i = 0; i < VECattributes.size(); i++)
			cout << VECattributes[i] << endl;
		
		cout << "\nPlease enter one number at a time followed by a space then hit enter." << endl;
		cout << "Example input: 7 5 4 0" << endl;
		cout << ">";
		string attribute;
		vector<string> projectAttributes; // attributes to be projected
		cin.ignore();
		getline(cin, attribute);
		string::iterator iter = attribute.begin();
		while(iter != attribute.end())
		{
			if(*iter == ' '){
				iter++;
				continue;
			}
			
			string pushAttribute = emplyoeeStatsAttribute(*iter);
			projectAttributes.push_back(pushAttribute);
			++iter;
		}

		
		//============================
		string listAttributes = "(";
		for(int j = 0; j < projectAttributes.size(); j++)
		{
			listAttributes += projectAttributes[j];
			if(j<projectAttributes.size()-1)
				listAttributes += ",";
		}
		listAttributes += ")";
		
		
		string employee1Sql = "project " + listAttributes + " " + selectJoinedTBL_1;
		string employee2Sql = "project " + listAttributes + " " + selectJoinedTBL_2;
		string sql = "("+ employee1Sql + ") - (" +employee2Sql + ")";
		cout << "e1: " << employee1Sql << endl;
		cout << "e2: " << employee2Sql << endl;
		cout << "sql: " << sql << endl;
		return sql;
		
}
void printMenu()
{
	cout << "\n\n";
	cout << "------------------------- MENU -------------------------" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "\n\n";
	cout << "1. Quit program" << endl;
	cout << "2. Make a pending/confirmer sale" << endl;
	cout << "3. Remove a pending Sale" << endl; // DELETE
	cout << "4. See Employee Stats" << endl; //PROJECT empName Orders, Car
	cout << "5. Compare Employees" << endl; // set DIFFERNCE
	cout << "6." << endl;
	cout << "7. Look up a Sale" << endl;
	cout << "8. Search by name, model, and/or employee" << endl;
	cout << "9. Expand Dealership" << endl;
	cout << "10. Log a payment" << endl; // update
	cout << "\n\n";
	cout << "--------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "\n" << endl;
	
}
int main()
{
	string sql;
	cout<<"\n\nWelcome to MEGAULTRASOOPER DEALERSHIP database software"<<endl;
	int choice = 0;
	
	
			 
	while (choice != 1)
	{		
		printMenu();
		cout << "To choose what you want to do, please enter the number associated with that command." 
			 << endl;
		cout << ">";
		cin >> choice;
		
		if(choice <= 0 || choice > 9)
		{
			cout << "Sorry, there is no command associated with your input please refer to the menu"
				 << " and try inputting again." << endl;
			//cout << ">";
			//cin >> choice;
		}
		else{
			cout << "Choice: " << choice << endl; // to double check for erroneous input
			
			//run switch statement to call correct funtion to execute correct dbms function
			switch(choice)
			{
				case 1:
						//addStock();
						break;
				case 2:
						break;
				case 3:
						sql = removeSale();
						// on bad input returns "ERROR"
						sql = "SHOW (" + sql + ");";
						cout << "SQL : " + sql << endl;
						cout << "Running command..." << endl;
						// run command here
						cout << "Command complete" << endl;
						break;
				case 4:
						sql = employeeStats();
						sql = "SHOW (" + sql + ");";
						cout << "SQL : " + sql << endl;
						cout << "Running command..." << endl;
						// run command here
						cout << "Command complete" << endl;
						break;
				case 5:
						sql = CompareEmployeeStats();
						cout << "SQL : " + sql << endl;
						sql = "SHOW (" + sql + ");";
						cout << "RUNNING: " << sql << endl;
						cout << "Running command..." << endl;
						// run command here
						cout << "Command complete" << endl;
						break;
				case 6:
						break;
				case 7:
						break;
				case 8:
						break;
				default:
					break;
				
			}
			// if (sql == "ERROR")
				// skipe sql statement
			// else
				// RUN THE SQL
		}	
	}
	// choice = 1
	//HERE RUN EXIT COMMAND
	return 0;
}

