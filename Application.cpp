#include <iostream>
#include <istream>
#include <string>

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
						cout << "SQL : " + sql << endl;
						cout << "Running command..." << endl;
						// run command here
						cout << "Command complete" << endl;
						break;
				case 4:
						break;
				case 5:
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

