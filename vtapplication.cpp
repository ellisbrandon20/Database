//Use natural Join to join two small tables
//7. Expand Dealership/ getlargeInventory
	//setUnion
//8. Quit

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// Handling empty inputs
bool isNotApplicable(string input)
{
	if(input == "N/A" || "")
	{
		return true;
	}
	
	return false;
}

bool isNumber(string& input)
{
	string::iterator iter = input.begin();
	while(iter != input.end() && isdigit(*iter))
		iter++;
	
	return !input.empty() && iter == input.end();
}

bool isString(string& input)
{
	string::iterator iter = input.begin();
	while(iter != input.end() && isalpha(*iter))
		iter++;
	
	return !input.empty() && iter == input.end();
}

int main() 
{
	// Attribute types
	
	// Employee
	string employeeName;
	string employeeID;
	// Orders
	string orderID;
	string fincRate;
	string cost;
	string pending;
	// Customer
	string customerName;
	string DOP;
	// Car
	string color;
	string make;
	string model;
	string year;
	
	// User's choice
	int usersChoice = -1;

	// Set Union
	/*
	cout << "Enter the make of the car you're looking for or \"N/A\": " << endl;
	cin >> make;
	cout << "Enter the model of the car you're looking for or \"N/A\": " << endl;
	cin >> model;
	cout << "Enter the year of the car you're looking for or \"N/A\": " << endl;
	cin >> year;
	
	
	// Query for customers and DOP with this make, model, and year of a car from "owns" table
	string ownsQuery = "(project (name, DOP) (select (make == \"" + make + "\") select (model == \""
						+ model + "\") select (year == \"" + year + "\") owns));";
	*/
// ---------------------------	
	while(usersChoice != 0)
	{
		// Menu
		cout << "\n";
		cout << "Type the number of the operation you'd like to perform:" << endl << endl;
		cout << "0. Exit the program." << endl;
		cout << "--------Union-------" << endl;
		cout << "1. Find the cars sold from an employee's name." << endl;
		cout << "2. Find the cars sold from an employee's ID." << endl;
		cout << "3. Find a car from an order ID." << endl;
		cout << "4. Find cars sold by financing rate." << endl;
		cout << "5. Find cars sold by price." << endl;
		cout << "6. Find cars that have pending orders." << endl;
		cout << "--------Join--------" << endl;
		cout << "7. Check if customer data matches on both tables" << endl;
		

		cin >> usersChoice;


	// ---------------------------	
		// Choosing which operation to do
		// Cars sold from employee's name
		if(usersChoice == 1)
		{
			cout << "What is the name of the employee?" << endl;
			cin.ignore();
			getline(cin, employeeName);
			
			string query1 = "SHOW (project(employeeName, make, color, year, model) ((select "
							"(employeeName == \"" + employeeName + "\") sale) + owns));";
			cout << query1 << endl;
		}

		// Cars sold from employee's ID
		else if(usersChoice == 2)
		{
			cout << "What is the employee's ID?" << endl;
			cin >> employeeID;
			
			if(isNumber(employeeID))
			{	
				string query2 = "SHOW (project(employeeID, make, color, year, model) ((select "
							"(employeeID == \"" + employeeID + "\") sale) + owns));";
				
				cout << query2 << endl;
			}
			else
				cout << "That is not a number." << endl;
		}
		
		// Cars sold by an order ID
		else if(usersChoice == 3)
		{
			cout << "What is the order ID?" << endl;
			cin >> orderID;
			
			if(isNumber(orderID))
			{
				string query2 = "SHOW (project(orderID, make, color, year, model) ((select "
							"(orderID == \"" + orderID + "\") sale) + owns));";
				cout << query2 << endl;
			}
			else
				cout << "That is not a number." << endl;
		}
		
		// Cars sold at a particular financing rate
		else if(usersChoice == 4)
		{
			string operation;
			cout << "Enter a financing rate." << endl;
			cin >> fincRate;
			
			if(isNumber(fincRate))
			{
				cout << "Enter either >, <, >=, <=, or == to narrow down your search." << endl;
				cin >> operation;
				
				if(operation == ">")
				{
					string query4 = "SHOW (project(financingRate, make, color, year, model) ((select "
									"(financingRate > " + fincRate + ") sale) + owns));";
				}				
				else if(operation == "<")
				{
					string query4 = "SHOW (project(financingRate, make, color, year, model) ((select "
									"(financingRate < " + fincRate + ") sale) + owns));";
				}				
				else if(operation == ">=")
				{
					string query4 = "SHOW (project(financingRate, make, color, year, model) ((select "
									"(financingRate >= " + fincRate + ") sale) + owns));";
				}			
				else if(operation == "<=")
				{
					string query4 = "SHOW (project(financingRate, make, color, year, model) ((select "
									"(financingRate <= " + fincRate + ") sale) + owns));";
				}				
				else if(operation == "==")
				{
					string query4 = "SHOW (project(financingRate, make, color, year, model) ((select "
									"(financingRate == " + fincRate + ") sale) + owns));";
				}
				else
					cout << "That is not a valid operand." << endl;
			}
			else
				cout << "That is not a number." << endl;
		}
		
		// Find cars sold by price
		else if(usersChoice == 5)
		{
			string operation;
			cout << "Enter a price." << endl;
			cin >> cost;
			
			if(isNumber(cost))
			{
				cout << "Enter either >, <, >=, <=, or == to narrow down your search." << endl;
				cin >> operation;
				
				if(operation == ">")
				{
					string query5 = "SHOW (project(cost, make, color, year, model) ((select "
									"(cost > " + cost + ") sale) + owns));";
								
				}			
				else if(operation == "<")
				{
					string query5 = "SHOW (project(cost, make, color, year, model) ((select "
									"(cost < " + cost + ") sale) + owns));";
				}			
				else if(operation == ">=")
				{
					string query5 = "SHOW (project(cost, make, color, year, model) ((select "
									"(cost >= " + cost + ") sale) + owns));";
				}			
				else if(operation == "<=")
				{
					string query5 = "SHOW (project(cost, make, color, year, model) ((select "
									"(cost <= " + cost + ") sale) + owns));";
				}			
				else if(operation == "==")
				{
					string query5 = "SHOW (project(cost, make, color, year, model) ((select "
									"(cost == " + cost + ") sale) + owns));";
				}	
				else
					cout << "That is not a valid operand." << endl;
			}
			else
				cout << "That is not a number." << endl;
		}
		
		else if(usersChoice == 6)
		{
			// Find cars with pending orders
			cout << "Enter \"Pending\" for pending orders or \"Confirmed\" for confirmed orders." 
					<< endl;
			cin >> pending;
			
			if(isString(pending))
			{
				int i = 0;
				// Converts input to lowercase
				while(i != pending.size())
				{
					pending[i] = tolower(pending[i]);
					i++;
				}
				
				if(pending == "pending")
				{
					string query6 = "SHOW (project (orderID, isPending, make, color, year, model) "
									"((select (isPending == 1) sale) + owns));";
					cout << query6 << endl;;
				}
				else if(pending == "confirmed")
				{
					string query6 = "SHOW (project (orderID, isPending, make, color, year, model) "
									"((select (isPending == 0) sale) + owns));";
				}
				else
					cout << "Pending or Confirmed was not entered." << endl;
			}
			else
				cout << "Nonalphabetic character has been entered." << endl;
		}
		
		// Natural joins both tables to see if customer data is the same
		else if(usersChoice == 7)
		{
			string query7 = "SHOW ((project(name, dateOfPurchase, amountOwed) sale) + "
							"(project(name, dateOfPurchase, amountOwed) owns));";
		}
		
		else 
			cout << "Please enter a number from the menu." << endl;
	}

// ---------------------------	
/*
	// Query for customers name and DOP given the make of the car	
	cout << "Enter the make of the car you're looking for or type \"N/A\": " << endl;
	cin >> make;
	
	if(isNotApplicable(make) == true)
	{
		cout << "No order ID has been entered" << endl;
	}
	else
		string ownsQuery1 = "(project (customerName, DOP) (select (make == \"" + make + "\") owns));";
	
	// Query for make, model, year, and color given the customers name
	
	cout << "Enter the name of the customer you're looking for or type \"N/A\": " << endl;
	cin >> customerName;
	
	if(isNotApplicable(make) == true)
	{
		cout << "No order ID has been entered" << endl;
	}
	else
		string ownsQuery2 = "(project (make, model, year, color) (select (customerName == \"" +
								customerName + "\") owns));";

// ---------------------------	
	
	// Query for customer's name based off of order ID from "sale" table	
	cout << "Enter the order ID of the purchase you want to query or type \"N/A\": " << endl;
	cin >> orderID;
	
	// Checking input for empty space or "N/A" 
	if(isNotApplicable(orderID) == true)
	{
		cout << "No order ID has been inputted" << endl;
	}

	string saleQuery1 = "(project (customerName) (select (orderID == \"" + orderID + "\") sale));";

// ---------------------------	
	
	// Set Union of "owns" and "sale" tables
	// Union of ownsQuery1 and saleQuery1 would result in 

// ---------------------------		
	
	// Natural Join
	
	
// ---------------------------	
	// callSetUnion();
	// Union both ownsQuery and saleQuery to get information about customers

	// Natural Join
	
	// callNaturalJoin();
	
	// Exit uses "EXIT;" reserve word
*/	
	
}
