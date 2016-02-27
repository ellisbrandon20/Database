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
#include <vector>
#include <string.h>

using namespace std;

int bytes_read;
string receive_output;
int fileDescriptor_ParentToChild[2];
int fileDescriptor_ChildToParent[2];
char readbuffer[255];
int orderID = 0;

string readPipe()
{
	usleep(1000);
	bytes_read = 0;
	bool stop = false;
	while(!stop)
	{
		bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
		readbuffer[bytes_read] = '\0';
		receive_output += readbuffer;//Opened file
		cout<<bytes_read<<endl;
		if(bytes_read <250)
			stop = true;
	}
	return receive_output;
}
void writePipe(string s)
{
	write(fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
}
void guarunteedOpen()
{
	string s = "OPEN sales;\n";
	writePipe(s);
	readPipe();
	if(readPipe().substr(0,11)== "OPEN sales;")
	{
		cout<<"Sales records successfully opened"<<endl;
	}
	else
	{
		s = "CREATE TABLE sales (employeeName VARCHAR(20), employeeID INTEGER, orderID INTEGER,"
							"financingRate INTEGER, isPending INTEGER, cost INTEGER, customerName "
							"VARCHAR(20), dateOfPurchase INTEGER, amountOwed INTEGER) PRIMARY KEY (orderID);\n";
		writePipe(s);
		cout<<readPipe();
		s = "OPEN sales;\n";
		writePipe(s);
		cout<<readPipe();
	}
	s = "OPEN owns;\n";
	writePipe(s);
	//readPipe();
	if(readPipe().substr(0,11)== "OPEN owns;")
	{
		cout<<"Owns records successfully opened"<<endl;
	}
	else
	{
		s = "CREATE TABLE owns (customerName VARCHAR(20), dateOfPurchase INTEGER, amountOwed INTEGER,"
							"color VARCHAR(20), year INTEGER, model VARCHAR(20), make VARCHAR(20))"
							"PRIMARY KEY (customerName, dateOfPurchase);\n";
		writePipe(s);
		cout<<readPipe();
		s = "OPEN owns;\n";
		writePipe(s);
		cout<<readPipe();
	}	
}
bool isNumber(string& input)
{
	string::iterator iter = input.begin();
	while(iter != input.end() && isdigit(*iter))
		iter++;
	
	return !input.empty() && iter == input.end();
}
void searchForSale()
{
	string orderID;
	string customer;
	string s;
	cout<<"Looking up a sale"<<endl;
	cout<<"Please enter the order ID if you know it or 0 if you do not"<<endl;
	cin>>orderID;
	if(orderID == "-1")
		cout<<"Please enter the customer name"<<endl;
		cin>>customer;
		cout<<"Displaying all purchases made by this customer"<<endl;
		s = "SHOW select (name==\""+ customer +"\") sales;\n";
		cout<<"Please enter the orderID of the desired purchase shown above or 0 to exit"<<endl;
		cin>>orderID;
		if(orderID!="-1")
			s = "SHOW select (orderID=="+ orderID +") sales;\n";
	else
		s = "SHOW select (orderID=="+ orderID +") sales;\n";
	writePipe(s);
	cout<<readPipe();
}
void search()
{
	int search;
		int subSearch;
		string s,
			   orderID, 
			   employee,
			   employeeID,
			   date,
			   todaysDate,
			   dateOfPurchase,
			   customer, 
			   car, 
			   finances, 
			   financingRate, 
			   cost, 
			   owed, 
			   color,
			   rangeOfDates,
			   year,
			   model,
			   make,
			   amountOwed;
		char op;
	cout<<"Search function"<<endl;
	cout<<"Enter one of the numbers below to select a search option"<<endl;
	cout<<"1. Search by Employee"<<endl;
	cout<<"2. Search by Order"<<endl;
	cout<<"3. Search by Customer"<<endl;
	cout<<"4. Search by Car"<<endl;
	cout<<endl;
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
				s = "SHOW a <- select employeeID == \""+employeeID+"\") sales;\n";
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
					s = "SHOW a <- select (financingRate == \""+financingRate+"\") sales;\n";
				else if(op == '>')
					s = "SHOW a <- select (financingRate > \""+financingRate+"\") sales;\n";
				else if(op == '<')
					s = "SHOW a <- select (financingRate < \""+financingRate+"\") sales;\n";
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
				cout<<endl;
				cin>>customer;
				s = "SHOW (select (name == \""+customer+"\") sales);\n";
			}
			else if(subSearch == 2)
			{
				string dateOfPurchase;
				cout<<"Enter the date you would like to search for"<<endl;
				cout<<endl;
				cin>>dateOfPurchase;
				cout<<"Search for dates before(1), after(2), or on(3) the specified date?"<<endl;
				cout<<endl;
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
	cout<<endl;
	cout<<receive_output<<endl;
	cout<<endl;
}
void show()
{
	string s = "SHOW sales;\n";
	writePipe(s);
	cout<<readPipe();
}
void writeTable()
{
	string s = "WRITE sales;\n";
	writePipe(s);
}
void open()
{
	string s = "OPEN animals;\n";
	write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
	cout<<"Looping1"<<endl;
	usleep(1000);
	bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
	readbuffer[bytes_read] = '\0';
	receive_output += readbuffer;
	cout<<receive_output<<endl;
}
void createSale()
{
	string customerName,
		   car,
		   cost,
		   financingRate,
		   date,
		   color,
		   finances,
		   dateOfPurchase,
		   orderStatus,
		   employeeName,
		   employeeID,
		   costOfCar,
		   year,
		   make,
		   model,
		   todaysDate;
	char today;
	char status;
	cout<<"Making a sale"<<endl;
	cout<<"Enter name of Employee making the sale"<<endl;
	cin>>employeeName;
	cout<<"Enter EmployeeID of Employee making the sale"<<endl;
	cin>>employeeID;
	/*string write;
	for(int i =0; i<employee.size(); i++)
	{
		if(employee[i] == ' ' && write.size()>1)
		{
			employeeData.push_back(write);
			write = "";
		}
		else if(employee[i] != ' ')
		{
			write+= employee[i];
			if(i == employee.size()-1)
				employeeData.push_back(write);
		}
	}	*/
	cout<<"Enter customer name"<<endl;
	cin>>customerName;
	/*cout<<"Enter the Year, Make, and Model (ex. '2007 Toyota Camry')";
	cin>>car;
	write = "";
	for(int x =0; x<car.size(); x++)
	{
		if(car[x] == ' ' && write.size()>1)
		{
			carData.push_back(write);
			write = "";
		}
		else if(car[x] != ' ')
		{
			write+= car[x];
			if(x == car.size()-1)
				carData.push_back(write);
		}
	}
	cout<<"Please Enter the color of the car"<<endl;
	cin>>color;*/
	cout<<"Please Enter the Cost of this transaction"<<endl;
	cin>>cost;
	cout<<"Please Enter the Financing Rate of this transaction"<<endl;
	cin>>financingRate;
	/*write = "";
	for(int y =0; y<car.size(); y++)
	{
		if(finances[y] == ' ' && write.size()>1)
		{
			financeData.push_back(write);
			write = "";
		}
		else if(finances[y] != ' ')
		{
			write+= finances[y];
			if(y == finances.size()-1)
				financeData.push_back(write);
		}
	}*/
	cout<<"Did this transaction occur today(y/n)?"<<endl;
	cin>>today;
	if(today == 'y')
		dateOfPurchase = todaysDate;
	else
	{
		cout<<"Please enter the date the transaction occurec in month/day/year format (ex. 01/23/2016)"<<endl;
		cin>>date;
		for(int z =0; z<date.size(); z++)
		{
			if(isdigit(date[z]))
				dateOfPurchase+=date[z];
		}
	}
	cout<<"Is this transaction pending(y/n)?"<<endl;
	cin>>status;
	if(status == 'y')
		orderStatus = "1";
	else
		orderStatus = "0";
	string fullSale = " (\""+employeeName+"\" , "+employeeID+" , "+to_string(orderID)+" , "+financingRate+" , "+orderStatus+" , "+cost+" ,\""+customerName+"\" ,"+dateOfPurchase+" , "+cost+");\n";
	cout<<fullSale<<endl;
	cout<<fullSale.size()<<endl;
	cout<<"What color is the car?"<<endl;
	cin>>color;
	cout<<"What year is the car?"<<endl;
	cin>>year;
	cout<<"What make is it?"<<endl;
	cin>>make;
	cout<<"What model is it?"<<endl;
	cin>>model;
	string ownsConstruct = " (\""+customerName+"\" , "+dateOfPurchase+" , "+cost+" , \""+color+"\" , "+year+" , \""+model+"\" , \""+make+"\");\n";
	orderID++;
	string s = "INSERT INTO sales VALUES FROM"+fullSale;
	writePipe(s);
	s = "INSERT INTO owns VALUES FROM"+ownsConstruct;
	writePipe(s);
	//cout<<readPipe();
	cout<<"The sale has been entered in the database successfully"<<endl;
}
int main()
{	
	readbuffer[255] = ' ';
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
		guarunteedOpen();
		int choice = 0;
		while(choice !=8)
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
			cout<<endl;
			cin>>choice;
			switch(choice)
			{
				case 0:
					show();
				break;
				case 1:
					open();
				break;
				case 2:
					
				break;
				case 3:
					
				break;
				case 4:
					createSale();
				break;
				case 5:
					searchForSale();
				break;
				case 6:
					search();
				break;
				case 7:
					
				break;
				case 8:
					writePipe("EXIT;\n");
					cout<<"Sending Request"<<endl;
					readPipe();
					readPipe();
				break;
				case 9:
					writeTable();
				break;
			}
		}
		cout<<"Goodbye, have a great day and thanks for using our program"<<endl;
	}
}