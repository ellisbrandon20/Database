#include <iostream>
#include <istream>
#include <string>

using namespace std;
























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
	cout << " ---- Remove a pending Sale ---- " << endl;
	cout << "Do you have the Order ID? (yes/no)" << endl;
	string orderIDflag;
	cout << ">";
	cin >> orderIDflag;
	if(orderIDflag == "yes")
	{
		cout << "Enter the Order ID" << endl;
		cout << ">";
		string orderID;
		cin >> orderID;
		cout << "WHAT" << endl;
		string sql = "DELETE FROM Sales WHERE OrderID == " + orderID ;
		return sql;
	}
	else if (orderIDflag == "no")
	{
		cout << "Do you have the Employee ID, Name of Customer, and the Model of the Sale? (yes/no)" 
		     << endl;
		string validInfoFlag;
		cout << ">";
		cin >> validInfoFlag;
		if(validInfoFlag == "yes")
		{
			cout << "What is the Employee ID number? (Enter only digits)" << endl;
			string employeeID;
			cin >> employeeID;
			cout << "What is the name of the Customer for this pending Sale? (enter only alpha)" << endl;
			string customerName;
			cin >> customerName;
			cout << "What is the Model of the car for this pending Sale?" << endl;
			string model;
			cin >> model;
			string sql = "DELETE FROM Sales WHERE employeeID == " + employeeID + " && customerName == " 
						+customerName+ " && model == " + model + ";";
			return sql;
		}
		else
		{
			cout << "Sorry we need either the Order ID or the Employee ID, Name of the customer, and Model"
				 << " of the car that was sold to perform this action." << endl;
		}
	}
	else
	{
		cout << "Incorrect input please re run this command and try again." << endl;
		return "ERROR";
	}
}

int main()
{
	string sql;
	cout<<"\n\nWelcome to MEGAULTRASOOPER DEALERSHIP database software"<<endl;
	int choice = 0;
	
	printMenu();

	cout << "To choose what you want to do, please enter the number associated with that command." 
			 << endl;
			 
	while (choice != 1)
	{		
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

