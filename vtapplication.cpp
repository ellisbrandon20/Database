//Use natural Join to join two small tables
//7. Expand Dealership/ getlargeInventory
	//setUnion
//8. Quit

#include <string>
#include <iostream>
#include <stdlib.h>
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
	// Customer
	string customerName;
	string DOP;
	// Car
	string color;
	string make;
	string model;
	string year;
	

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
	
	
}
