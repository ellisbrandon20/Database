#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main()
{	
	int bytes_read;
	string receive_output;
	int fileDescriptor_ParentToChild[2];
	int fileDescriptor_ChildToParent[2];
	pipe(fileDescriptor_ParentToChild);//create pipe-to-child
	pipe(fileDescriptor_ChildToParent);//create pipe-from-child
	int childPID = fork();
	if (childPID < 0)
	{
		cout << "Fork failed" << endl;
		exit(-1);
	}
	else if (childPID == 0)
	{
		dup2(fileDescriptor_ParentToChild[0], 0);//close stdout & make read end of p2c into stdout
		close(fileDescriptor_ParentToChild[0]);//close read end of p2c
		close(fileDescriptor_ParentToChild[1]);//close write end of p2c
		dup2(fileDescriptor_ChildToParent[1], 1);//close stdin & make read end of pFc into stdin
		close(fileDescriptor_ChildToParent[1]);//close write end of pFc
		close(fileDescriptor_ChildToParent[0]);//close read end of pFc

		//Execute the required program
		execvp("./main", NULL);
		cout<<"Child is exiting"<<endl;
		exit(0);
	}
	else
	{
		close( fileDescriptor_ParentToChild[0]);//close read end of p2c
		close(fileDescriptor_ChildToParent[1]);//close write end of pFc
		char readbuffer[255];
		string s ="";
		int choice = 0;
		int search;
		int subSearch;
		string orderID;
		string employee;
		string customer;
		string car;
		string finances;
		string financingRate;
		string cost;
		string owed;
		string color;
		string rangeOfDates;
		string year;
		string model;
		string make;
		string amountOwed;
		char op;
		string employeeID;
		while(choice !=9)
		{
			cout<<"List of available commands"<<endl;
			cout<<"Please enter the number of the desired command"<<endl;
			cout<<"0. Show Table (For Debugging will not be in final project)"<<endl;
			cout<<"1. Open Table (Should be Add/Remove Stock of cars)"<<endl;
			cout<<"2. Unimplemented (Employee Sale stats)"<<endl;
			cout<<"3. Unimplemented (Compar Employees)"<<endl;
			cout<<"4. Make a sale"<<endl;
			cout<<"5. Look up a sale"<<endl;
			cout<<"6. Search by name/model/employee/etc."<<endl;
			cout<<"7. Expand Dealershup/ getLargeInventory"<<endl;
			cout<<"8. Quit"<<endl;
			cin>>choice;
			switch(choice)
			{
				case 0:
					s = "SHOW animals;\n";
					write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
					usleep(1000);
					bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
					readbuffer[bytes_read] = '\0';
					receive_output += readbuffer;
					cout<<receive_output<<endl;
				break;
				case 1:
					s = "OPEN animals;\n";
					write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
					cout<<"Looping1"<<endl;
					usleep(1000);
					bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
					readbuffer[bytes_read] = '\0';
					receive_output += readbuffer;
					cout<<receive_output<<endl;
				break;
				case 2:
				break;
				case 3:
				break;
				case 4:
					cout<<"Making a sale"<<endl;
					cout<<"Enter name of Employee making the sale"<<endl;
					cin>>employee;
					cout<<"Enter customer name"<<endl;
					cin>>customer;
					cout<<"Enter the Year, Make, and Model (ex. '2007 Toyota Camry')";
					cin>>car;
					cout<<"Please Enter the Cost and Financing Rate of this transaction (ex. $32000 5%)"<<endl;
					cin>>finances;
					cout<<"The sale has been entered in the database successfully"<<endl;
				break;
				case 5:
					cout<<"Looking up a sale"<<endl;
					cout<<"Please enter the order ID if you know it or 0 if you do not"<<endl;
					cin>>orderID;
					if(orderID == "0")
						cout<<"Please enter the customer name"<<endl;
						cin>>customer;
						cout<<"Displaying all purchases made by this customer"<<endl;
						s = "SHOW select (name==\""+ customer +"\") sales;\n";
						cout<<"Please enter the orderID of the desired purchase shown above or 0 to exit"<<endl;
						cin>>orderID;
						if(orderID!="0")
							s = "SHOW select (orderID=="+ orderID +") sales;\n";
					else
						s = "SHOW select (orderID=="+ orderID +") sales;\n";
				break;
				case 6:
					
					cout<<"Search function"<<endl;
					cout<<"Enter one of the numbers below to select a search option"<<endl;
					cout<<"1. Search by Employee"<<endl;
					cout<<"2. Search by Order"<<endl;
					cout<<"3. Search by Customer"<<endl;
					cout<<"4. Search by Car"<<endl;
					cin>>search;
					switch(search)
					{
						case 1:
							cout<<"Searching by employee"<<endl;
							cout<<"Search by Employee name(1) or Employee ID(2)?"<<endl;
							cin>>subSearch;
							if(subSearch == 1)
							{
								cout<<"Enter the name you would like to search for"<<endl;
								cin>>employee;
								if(subSearch ==1)
									s = "SHOW (select (name == \""+employee+"\") sales);\n";
							}
							else if(subSearch == 2)
							{
								cout<<"Enter the ID you would like to search for"<<endl;
								cin>>employeeID;
								s = "SHOW (select (employeeID == \""+employeeID+"\") sales);\n";
							}
							else
								cout<<"Not '1' or '2' returning to main menu"<<endl;
						break;
						case 2:
							cout<<"Searching by order"<<endl;
							cout<<"Search by Order ID(1), Financing Rate(2), or Cost(3)?"<<endl;
							cin>>subSearch;
							if(subSearch == 1)
							{
								cout<<"Enter the ID you would like to search for"<<endl;
								cin>>orderID;
								s = "SHOW (select (orderID == \""+orderID+"\") sales);\n";
							}
							else if(subSearch == 2)
							{
								cout<<"Enter the Financing Rate you would like to search for"<<endl;
								cin>>financingRate;
								cout<<"Search for Financing Rates higher(>), lower(<), or equal to (=) entered rate"<<endl;
								cin>>op;
								if(op == '=')
									s = "SHOW (select (employeeID == \""+financingRate+"\") sales);\n";
								else if(op == '>')
									s = "SHOW (select (employeeID > \""+financingRate+"\") sales);\n";
								else if(op == '<')
									s = "SHOW (select (employeeID < \""+financingRate+"\") sales);\n";
								else
									cout<<"Did not enter =,>,or < returning to main menu"<<endl;
							}
							else if(subSearch == 3)
							{
								cout<<"Enter the cost you would like to search for"<<endl;
								cin>>financingRate;
								cout<<"Search for financing rates higher(>), lower(<), or equal to (=) entered rate"<<endl;
								cin>>op;
								if(op == '=')
									s = "SHOW (select (cost == \""+cost+"\") sales);\n";
								else if(op == '>')
									s = "SHOW (select (cost > \""+cost+"\") sales);\n";
								else if(op == '<')
									s = "SHOW (select (cost < \""+cost+"\") sales);\n";
								else
									cout<<"Did not enter =,>,or < returning to main menu"<<endl;
							}
							else
								cout<<"Not '1', '2', or '3' returning to main menu"<<endl;
						break;
						case 3:
							cout<<"Searching by customer"<<endl;
							cout<<"Search by customer name(1), Date of Purchase(2), or Amount Owed(3)?"<<endl;
							cin>>subSearch;
							if(subSearch == 1)
							{
								cout<<"Enter the name you would like to search for"<<endl;
								cin>>customer;
								s = "SHOW (select (name == \""+customer+"\") sales);\n";
							}
							else if(subSearch == 2)
							{
								string dateOfPurchase;
								cout<<"Enter the date you would like to search for"<<endl;
								cin>>dateOfPurchase;
								cout<<"Search for dates before(1), after(2), or on(3) the specified date?"<<endl;
								cin>>rangeOfDates;
								if(rangeOfDates == "on")
								{
									s = "SHOW (select (dateOfPurchase == \""+dateOfPurchase+"\") sales);\n";
								}
								else if(rangeOfDates == "after")
									s = "SHOW (select (dateOfPurchase < \""+dateOfPurchase+"\") sales);\n";
								else if(rangeOfDates == "before")
									s = "SHOW (select (dateOfPurchase > \""+dateOfPurchase+"\") sales);\n";
								else
									cout<<"Did not enter 'on', 'before', or 'after' returning to main menu"<<endl;
							}
							else if(subSearch == 3)
							{
								cout<<"Enter the amount owed you would like to search for"<<endl;
								cin>>amountOwed;
								cout<<"Search for amounts owed higher(>), lower(<), or equal to (=) entered rate"<<endl;
								cin>>op;
								if(op == '=')
									s = "SHOW (select (amoutOwed == \""+amountOwed+"\") sales);\n";
								else if(op == '>')
									s = "SHOW (select (amountOwed > \""+amountOwed+"\") sales);\n";
								else if(op == '<')
									s = "SHOW (select (amountOwed < \""+amountOwed+"\") sales);\n";
								else
									cout<<"Did not enter =,>,or < returning to main menu"<<endl;
							}
							else
								cout<<"Not '1', '2', or '3' returning to main menu"<<endl;
						break;
						case 4:
							cout<<"Searching by car"<<endl;
							cout<<"Search by Color(1), Year(2), Model(3), or Make(4)?"<<endl;
							cin>>subSearch;
							if(subSearch == 1)
							{
								cout<<"Enter the color you are looking for"<<endl;
								cin>>color;
								s = "SHOW (select (color == \""+color+"\") sales);\n";
							}
							else if(subSearch == 2)
							{
								cout<<"Enter the year you are looking for"<<endl;
								cin>>year;
								cout<<"Search for years before(1), after(2), or on(3) the specified year?"<<endl;
								cin>>rangeOfDates;
								if(rangeOfDates == "on")
								{
									s = "SHOW (select (year == \""+year+"\") sales);\n";
								}
								else if(rangeOfDates == "after")
									s = "SHOW (select (year < \""+year+"\") sales);\n";
								else if(rangeOfDates == "before")
									s = "SHOW (select (year > \""+year+"\") sales);\n";
								else
									cout<<"Did not enter 'on', 'before', or 'after' returning to main menu"<<endl;
							}
							else if(subSearch == 3)
							{
								cout<<"Enter the Model you are looking for"<<endl;
								cin>>model;
								s = "SHOW (select (model == \""+model+"\") sales);\n";
							}
							else if(subSearch == 4)
							{
								cout<<"Enter the Make you are looking for"<<endl;
								cin>>make;
								s = "SHOW (select (make == \""+make+"\") sales);\n";
							}
							else
								cout<<"Did not enter a '1', '2', '3', or '4' returning to main menu"<<endl;
						break;
					}
					write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
					cout<<"Sending Request"<<endl;
					usleep(1000);
					bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
					readbuffer[bytes_read] = '\0';
					receive_output += readbuffer;
					cout<<receive_output<<endl;
				break;
				case 7:
				break;
				case 8:
				break;
				case 9:
					s = "EXIT;\n";
					write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
					cout<<"Sending Request"<<endl;
				break;
			}
		}
		cout<<"Goodbye, have a great day and thanks for using our program"<<endl;
	}
}