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


int fileDescriptor_ParentToChild[2];
int fileDescriptor_ChildToParent[2];
string salesDB = "sales";
string ownsDB = "owns";

string readPipe()
{
	int bytes_read;
	string receive_output;
	char readbuffer[255] = "";
	usleep(9000);
	bytes_read = 0;
	bool stop = false;
	while(!stop)
	{
		bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
		readbuffer[bytes_read] = '\0';
		receive_output += readbuffer;//Opened file
		//cout<<bytes_read<<endl;
		if(bytes_read <254)
			stop = true;
	}
	return receive_output;
}
void writePipe(string sql)
{
	write(fileDescriptor_ParentToChild[1], sql.c_str(), strlen(sql.c_str()));
}
/*void goOffline()
{
	string dbCommand = "CLOSE sales;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "CLOSE owns;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "OPEN offline_sales;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "OPEN offline_owns;\n";
	writePipe(dbCommand);
	readPipe();
	salesDB = "offline_sales";
	ownsDB = "offline_owns";
}
void goOnline()
{
	string dbCommand = "OPEN sales;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "OPEN owns;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "INSERT INTO sales VALUES FROM RELATION offline_sales;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "INSERT INTO owns VALUES FROM RELATION offline_owns;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "offline_sales <- offline_sales - offline_sales;\n";
	writePipe(dbCommand);
	cout<<readPipe();
	dbCommand = "offline_owns <- offline_owns - offline_owns;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "CLOSE offline_sales;\n";
	writePipe(dbCommand);
	readPipe();
	dbCommand = "CLOSE offline_owns;\n";
	writePipe(dbCommand);
	readPipe();
	salesDB = "sales";
	ownsDB = "owns";
}*/
void guarunteedOpen()
{
	string sql = "OPEN sales;\n";
	writePipe(sql);
	/*if(readPipe().substr(0,11)== "OPEN sales;")
	{
		cout<<"Sales records successfully opened"<<endl;
	}
	else
	{
		sql = "CREATE TABLE sales (employeeName VARCHAR(20), employeeID INTEGER, orderID INTEGER,"
							"financingRate INTEGER, isPending INTEGER, cost INTEGER, customerName "
							"VARCHAR(20), dateOfPurchase INTEGER, amountOwed INTEGER) PRIMARY KEY (orderID);\n";
		writePipe(sql);
		cout<<readPipe();
		sql = "OPEN sales;\n";
		writePipe(sql);
		cout<<readPipe();
	}*/
	sql = "OPEN owns;\n";
	writePipe(sql);
	/*if(readPipe().substr(0,11)== "Opened file")
	{
		cout<<"Owns records successfully opened"<<endl;
	}
	else
	{
		sql = "CREATE TABLE owns (customerName VARCHAR(20), dateOfPurchase INTEGER, amountOwed INTEGER,"
							"color VARCHAR(20), year INTEGER, model VARCHAR(20), make VARCHAR(20))"
							"PRIMARY KEY (customerName, dateOfPurchase);\n";
		writePipe(sql);
		cout<<readPipe();
		sql = "OPEN owns;\n";
		writePipe(sql);
		cout<<readPipe();
	}
	cout<<endl;
	cout<<endl;*/
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
	string sql;
	cout << "\n\n";
	cout << " ---- Look up a Sale ---- " << endl;
	cout<<"\nPlease enter the order ID if you know it or -1 if you do not"<<endl;
	cout <<">";
	cin>>orderID;
	//cout<<orderID<<endl;
	if(orderID == "-1")
	{
		cout<<"\nPlease enter the customer name"<<endl;
		cout << ">";
		cin>>customer;
		cout<<"\nDisplaying all purchases made by this customer"<<endl;
		sql = "SHOW a <- select (customerName==\""+ customer +"\") sales;\n";
		writePipe(sql);
		cout<<readPipe()<<endl;
		cout<<"\nPlease enter the orderID of the desired purchase shown above or -1 to exit"<<endl;
		cout << ">";
		cin>>orderID;
		if(orderID!="-1")
			sql = "SHOW a <- select (orderID=="+ orderID +") "+salesDB+";\n";
	}
	else
		sql = "SHOW a <- select (orderID=="+ orderID +") "+salesDB+";\n";
	writePipe(sql);
	cout<<readPipe();
}
void search()
{
	int search;
		int subSearch, rangeOfDates;
		string sql,
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
			   year,
			   model,
			   make,
			   amountOwed,
			   op;
	cout << "\n\n";
	cout << " ---- Search ---- " << endl;
	cout<<"\nEnter one of the numbers below to select a search option"<<endl;
	cout<<"1. Search by Employee"<<endl;
	cout<<"2. Search by Order"<<endl;
	cout<<"3. Search by Customer"<<endl;
	cout<<"4. Search by Car"<<endl;
	cout<<endl;
	cout << ">";
	cin>>search;
	switch(search)
	{
		case 1:
			cout<<"\nSearching by employee"<<endl;
			cout<<"Search by Employee name(1) or Employee ID(2)?"<<endl;
			cout << ">";
			cin>>subSearch;
			if(subSearch == 1)
			{
				cout<<"\nEnter the name you would like to search for"<<endl;
				cout << ">";
				cin>>employee;
				if(subSearch ==1)
					sql = "SHOW (select (employeeName == \""+employee+"\") "+salesDB+");\n";
			}
			else if(subSearch == 2)
			{
				cout<<"\nEnter the ID you would like to search for"<<endl;
				cout << ">";
				cin>>employeeID;
				sql = "SHOW (select (employeeID == "+employeeID+") "+salesDB+");\n";
			}
			else
				cout<<"Not '1' or '2' returning to main menu"<<endl;
		break;
		case 2:
			cout<<"\nSearching by order"<<endl;
			cout<<"Search by Order ID(1), Financing Rate(2), or Cost(3)?"<<endl;
			cout << ">";
			cin>>subSearch;
			if(subSearch == 1)
			{
				cout<<"\nEnter the ID you would like to search for"<<endl;
				cout << ">";
				cin>>orderID;
				sql = "SHOW (select (orderID=="+ orderID +") "+salesDB+");\n";
			}
			else if(subSearch == 2)
			{
				cout<<"\nEnter the Financing Rate you would like to search for"<<endl;
				cout << ">";
				cin>>financingRate;
				cout<<"\nSearch for Financing Rates higher(>), lower(<), or equal to (=) entered rate"<<endl;
				cout << ">";
				cin>>op;
				if(op == "=")
					sql = "SHOW (select (financingRate == "+financingRate+") "+salesDB+");\n";
				else if(op == ">")
					sql = "SHOW (select (financingRate > "+financingRate+") "+salesDB+");\n";
				else if(op == "<")
					sql = "SHOW (select (financingRate < "+financingRate+") "+salesDB+");\n";
				else
					cout<<"\nDid not enter =,>,or < returning to main menu"<<endl;
			}
			else if(subSearch == 3)
			{
				cout<<"\nEnter the cost you would like to search for"<<endl;
				cout << ">";
				cin>>cost;
				cout<<"\nSearch for financing rates higher(>), lower(<), or equal to (=) entered rate"<<endl;
				cout<<">";
				cin>>op;
				if(op == "=")
					sql = "SHOW (select (cost == "+cost+") "+salesDB+");\n";
				else if(op == ">")
					sql = "SHOW (select (cost > "+cost+") "+salesDB+");\n";
				else if(op == "<")
					sql = "SHOW (select (cost < "+cost+") "+salesDB+");\n";
				else
					cout<<"\nDid not enter =,>,or < returning to main menu"<<endl;
			}
			else
				cout<<"\nNot '1', '2', or '3' returning to main menu"<<endl;
		break;
		case 3:
			cout<<"\nSearching by customer"<<endl;
			cout<<"Search by customer name(1), Date of Purchase(2), or Amount Owed(3)?"<<endl;
			cout << ">";
			cin>>subSearch;
			if(subSearch == 1)
			{
				cout<<"Enter the name you would like to search for"<<endl;
				cout<<endl;
				cin>>customer;
				sql = "SHOW (select (customerName == \""+customer+"\") "+salesDB+");\n";
			}
			else if(subSearch == 2)
			{
				string enteredDate;
				string dateOfPurchase = "";
				cout<<"\nEnter the date you would like to search for in Month/Day/Year format (ex"<<endl;
				cout<<">";
				cin>>enteredDate;
				for(int dateParse = 0; dateParse<enteredDate.size(); dateParse++)
				{
					if(isdigit(enteredDate[dateParse]))
						dateOfPurchase+=enteredDate;
				}
				enteredDate = "";
				enteredDate += dateOfPurchase.substr(4,4);
				enteredDate += dateOfPurchase.substr(0,2);
				enteredDate += dateOfPurchase.substr(2,2);
				dateOfPurchase = enteredDate;
				cout<<"\nSearch for dates before(1), after(2), or on(3) the specified date?"<<endl;
				cout<<">";
				cin>>rangeOfDates;
				if(rangeOfDates == 1)
				{
					sql = "SHOW (select (dateOfPurchase < "+dateOfPurchase+") "+salesDB+");\n";
				}
				else if(rangeOfDates == 2)
					sql = "SHOW (select (dateOfPurchase > "+dateOfPurchase+") "+salesDB+");\n";
				else if(rangeOfDates == 3)
					sql = "SHOW (select (dateOfPurchase == "+dateOfPurchase+") "+salesDB+");\n";
				else
				{
					cout<<"Did not enter 'on', 'before', or 'after' returning to main menu"<<endl;
					sql = "ERROR";
				}
			}
			else if(subSearch == 3)
			{
				cout<<"\nEnter the amount owed you would like to search for"<<endl;
				cout << ">";
				cin>>amountOwed;
				cout<<"\nSearch for amounts owed higher(>), lower(<), or equal to (=) entered rate"<<endl;
				cout << ">";
				cin>>op;
				if(op == "=")
					sql = "SHOW (select (amoutOwed == "+amountOwed+") "+salesDB+");\n";
				else if(op == ">")
					sql = "SHOW (select (amountOwed > "+amountOwed+") "+salesDB+");\n";
				else if(op == "<")
					sql = "SHOW (select (amountOwed < "+amountOwed+") "+salesDB+");\n";
				else
					cout<<"\nDid not enter =,>,or < returning to main menu"<<endl;
			}
			else
				cout<<"\nNot '1', '2', or '3' returning to main menu"<<endl;
		break;
		case 4:
			cout<<"\nSearching by car"<<endl;
			cout<<"\nSearch by Color(1), Year(2), Model(3), or Make(4)?"<<endl;
			cout << ">";
			cin>>subSearch;
			if(subSearch == 1)
			{
				cout<<"\nEnter the color you are looking for"<<endl;
				cout << ">";
				cin>>color;
				sql = "SHOW (select (color == \""+color+"\") "+ownsDB+");\n";
			}
			else if(subSearch == 2)
			{
				cout<<"\nEnter the year you are looking for"<<endl;
				cout << ">";
				cin>>year;
				cout<<"\nSearch for years before(1), after(2), or on(3) the specified year?"<<endl;
				cout << ">";
				cin>>rangeOfDates;
				if(rangeOfDates == 3)
				{
					sql = "SHOW (select (year == "+year+") "+ownsDB+");\n";
				}
				else if(rangeOfDates == 1)
					sql = "SHOW (select (year < "+year+") "+ownsDB+");\n";
				else if(rangeOfDates == 2)
					sql = "SHOW (select (year > "+year+") "+ownsDB+");\n";
				else
				{
					sql = "ERROR";
					cout<<"\nDid not enter 1, 2, or 3 returning to main menu"<<endl;
				}
			}
			else if(subSearch == 3)
			{
				cout<<"\nEnter the Model you are looking for"<<endl;
				cout << ">";
				cin>>model;
				sql = "SHOW (select (model == \""+model+"\") "+ownsDB+");\n";
			}
			else if(subSearch == 4)
			{
				cout<<"\nEnter the Make you are looking for"<<endl;
				cin>>make;
				sql = "SHOW (select (make == \""+make+"\") "+ownsDB+");\n";
			}
			else
				cout<<"\nDid not enter a '1', '2', '3', or '4' returning to main menu"<<endl;
		break;
	}
	writePipe(sql);
	cout<<readPipe();
}
void show()
{
	cout << "\n\n";
	cout << " ---- Show Table ---- " << endl;
	int choice = 0;
	cout<<"\nShow Sales(1), Owns(2), or both(3)?"<<endl;
	cout << ">";
	cin>>choice;
	cout<<endl;
	cout<<endl;
	if(choice == 1 || choice == 3)
	{
		string sql = "SHOW "+salesDB+";\n";
		writePipe(sql);
		cout<<readPipe();
		cout<<endl;
	}
	cout<<endl;
	cout<<endl;
	if(choice == 2 || choice == 3)
	{
		string sql = "SHOW "+ownsDB+";\n";
		writePipe(sql);
		cout<<readPipe();
	}
	cout<<endl;
	cout<<endl;
}
void writeTable()
{
	cout << "\n\n";
	cout << " ---- Writing Table ---- " << endl;
	string sql = "WRITE "+salesDB+";\n";
	writePipe(sql);
}
void open()
{
	//string s = "OPEN animals;\n";
	//writePipe(s);
	//cout<<readPipe();
	/*write( fileDescriptor_ParentToChild[1], s.c_str(), strlen(s.c_str()));
	cout<<"Looping1"<<endl;
	usleep(1000);
	bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
	readbuffer[bytes_read] = '\0';
	receive_output += readbuffer;
	cout<<receive_output<<endl;*/
}
void createSale()
{
	cout << "\n\n";
	cout << " ---- Make A Sale ---- " << endl;
	string customerName,
		   orderID,
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
	cout<<"\nEnter name of Employee making the sale"<<endl;
	cout << ">";
	cin>>employeeName;
	cout<<"\nEnter EmployeeID of Employee making the sale"<<endl;
	cout << ">";
	cin>>employeeID;
	cout<<"\nEnter orderID"<<endl;
	cout << ">";
	cin>>orderID;
	cout<<"\nEnter customer name"<<endl;
	cout << ">";
	cin>>customerName;
	cout<<"\nPlease Enter the Cost of this transaction"<<endl;
	cout << ">";
	cin>>cost;
	cout<<"\nPlease Enter the Financing Rate of this transaction"<<endl;
	cout << ">";
	cin>>financingRate;
	cout<<"\nDid this transaction occur today(y/n)?"<<endl;
	cout << ">";
	cin>>today;
	if(today == 'y')
	{
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
        todaysDate += to_string(now->tm_mon + 1);
        todaysDate += to_string(now->tm_mday);
		todaysDate += to_string(now->tm_year + 1900);
		dateOfPurchase = todaysDate;
	}
	else
	{
		cout<<"\nPlease enter the date the transaction occurec in month/day/year format (ex. 01/23/2016)"<<endl;
		cout << ">";
		cin>>date;
		for(int z =0; z<date.size(); z++)
		{
			if(isdigit(date[z]))
				dateOfPurchase+=date[z];
		}
		string enteredDate = dateOfPurchase.substr(4,4);
		enteredDate += dateOfPurchase.substr(0,2);
		enteredDate += dateOfPurchase.substr(2,2);
		dateOfPurchase = enteredDate;
	}
	cout<<"\nIs this transaction pending(y/n)?"<<endl;
	cout << ">";
	cin>>status;
	if(status == 'y')
		orderStatus = "1";
	else
		orderStatus = "0";
	string fullSale = " (\""+employeeName+"\" , "+employeeID+" , "+orderID+" , "+financingRate+" , "
					+cost+" , "+orderStatus+" , \""+customerName+"\" ,"+dateOfPurchase+" , "+cost+");\n";
	cout<<"\nWhat color is the car?"<<endl;
	cout << ">";
	cin>>color;
	cout<<"\nWhat year is the car?"<<endl;
	cout << ">";
	cin>>year;
	cout<<"\nWhat make is it?"<<endl;
	cout << ">";
	cin>>make;
	cout<<"\nWhat model is it?"<<endl;
	cout << ">";
	cin>>model;
	string ownsConstruct = " (\""+customerName+"\" , "+dateOfPurchase+" , "+cost+" , \""
						+color+"\" , "+year+" , \""+model+"\" , \""+make+"\");\n";
	//orderID++;
	string sql = "INSERT INTO "+salesDB+" VALUES FROM"+fullSale;
	writePipe(sql);
	if(orderStatus != "1")
	{
		sql = "INSERT INTO "+ownsDB+" VALUES FROM"+ownsConstruct;
		writePipe(sql);
	}
	//cout<<readPipe();
	cout<<"\nThe sale has been entered in the database successfully"<<endl;
}
void removeSale()
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
	//string require = ""; // if == "yes" place "&&" in the sql otherwise place "||" in sql
	//string require2 = ""; // if == "yes" place "&&" in the sql otherwise place "||" in sql
	
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
			
			if(!isNumber(orderID)) 
				sql="ERROR";
			if (orderOP == "=")
				sql = "DELETE FROM "+salesDB+" WHERE (orderID == " + orderID + ")";
			else
				sql = "DELETE FROM "+salesDB+" WHERE (orderID != " + orderID + ")";

			break;
		case 2:
			//Employee ID
			cout <<"\nWhat is the Employee ID?" << endl;
			cout << ">";
			cin >> employeeID;
			if(!isNumber(employeeID)) 
				sql="ERROR";
			
			//find op
			cout << "\nAre you deleting records that are eqaul (=) to or not equal (!=) to Employee ID?"
				 << endl;
			cout << ">";
			cin >> employeeIDop;
			
			if(employeeIDop == "=")
				sql = "DELETE FROM "+salesDB+" WHERE (employeeID == " + employeeID + ")";
			else
				sql = "DELETE FROM "+salesDB+" WHERE (employeeID != " + employeeID + ")";
			
			break;
		case 3:
			//Customer
			cout <<"\nWhat is the Customer's name?" << endl;
			cout << ">";
			cin >> customer;
			
			//find op
			cout << "\nAre you deleting records that are eqaul (=) to or not equal (!=) to Customer name?"
				 << endl;
			cout << ">";
			cin >> customerOP;
			
			if(customerOP == "=")
				sql = "DELETE FROM "+salesDB+" WHERE (customerName == " + customer + ")";
			else
				sql = "DELETE FROM "+salesDB+" WHERE (customerName != " + customer + ")";

			break;
		case 4:
			//Model
			cout <<"\nWhat is the Model name of the car?" << endl;
			cout << ">";
			cin >> model;
			
			//find op
			cout << "\nAre you deleting records that are eqaul (=) to or not equal (!=) to Model name"
				 << endl;
			cout << ">";
			cin >> modelOP;
			
			if(modelOP == "=")
				sql = "DELETE FROM "+salesDB+" WHERE (model == " + model + ")";
			else
				sql = "DELETE FROM "+salesDB+" WHERE (model != " + model + ")";
	
			break;
		default:
			break;
			
	}
	if(sql != "ERROR")
	{
		string sqltosend = sql + ";\n";
		writePipe(sqltosend);
	}
	else
		cerr << "ERROR you entered some wrong data, please re-run this command and try again" << endl;
}
string employeeStatsAttribute(char attribute)
{
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
void employeeStats()
{
	string sql;
	bool flag = false;
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
		if(!isNumber(employeeID)) 
			cerr<<"ERROR"<<endl;
		
		joinedTBL = "("+salesDB+" JOIN owns)";
		selectJoinedTBL = "(select (employeeID == " + employeeID 
								+ ") " + joinedTBL + ")";
	}
	else if (search == "2")
	{
		cout << "\nPlease enter the Employee's Name" << endl;
		cout << ">";
		string employeeName;
		cin >> employeeName;
		
		joinedTBL = "("+salesDB+" JOIN owns)";
		selectJoinedTBL = "(select (employeeName == \"" + employeeName 
								+ "\") " + joinedTBL + ")";
	}
	else
	{
		cout << "\nSorry you did not enter valid input we were looking for \"1\" or \"2\"" << endl;
		sql = "ERROR";
		//return "ERROR";
	}
	if (sql != "ERROR")
	{
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
			if(*iter != '0' && *iter != '1' && *iter != '2' && *iter != '3' && *iter != '4'
				&& *iter != '5' && *iter != '6' && *iter != '7')
			{	
				flag = true;
				break;
			}
			string pushAttribute = employeeStatsAttribute(*iter);
			projectAttributes.push_back(pushAttribute);
			++iter;
		}
		cout << "\n\n";
		
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
		sql = "project " + listAttributes + " " + selectJoinedTBL;
	}
		string sqltosend;
		//cout << "SQL: "<<sql << endl;
		if(sql == "ERROR" || flag)
		{
			cerr << "ERROR on input validation try again" << endl;

		}
		else
		{
			sqltosend = "SHOW (" + sql + ");\n";
			writePipe(sqltosend);
			cout<<readPipe();
		}
}
void CompareEmployeeStats()
{
	string sql;
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
		joinedTBL = "("+salesDB+" JOIN owns)";
		
		cout << "\nPlease enter the first Employee's ID" << endl;
		cout << ">";
		string employeeID_1;
		cin >> employeeID_1;
		if(!isNumber(employeeID_1)) 
			sql="ERROR";
		
		
		selectJoinedTBL_1 = "(select (employeeID == " + employeeID_1 
								+ ") " + joinedTBL + ")";
								
		cout << "\nPlease enter the second Employee's ID" << endl;
		cout << ">";
		string employeeID_2;
		cin >> employeeID_2;
		if(!isNumber(employeeID_2))
			sql="ERROR";
		
		selectJoinedTBL_2 = "(select (employeeID == " + employeeID_2 
								+ ") " + joinedTBL + ")";
								
								
	}
	else if (search == "2")
	{
		joinedTBL = "("+salesDB+" JOIN owns)";
		
		cout << "\nPlease enter the first Employee's Name" << endl;
		cout << ">";
		string employeeName_1;
		cin >> employeeName_1;
		
		selectJoinedTBL_1 = "(select (employeeName == \"" + employeeName_1
								+ "\") " + joinedTBL + ")";
								
		cout << "\nPlease enter the  second Employee's Name" << endl;
		cout << ">";
		string employeeName_2;
		cin >> employeeName_2;
		
		selectJoinedTBL_2 = "(select (employeeName == \"" + employeeName_2 
								+ "\") " + joinedTBL + ")";
	}
	else
	{
		cout << "Sorry you did not enter valid input we were looking for \"1\" or \"2\"" << endl;
		sql="ERROR";
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
		bool flag = false;
		while(iter != attribute.end())
		{
			if(*iter == ' '){
				iter++;
				continue;
			}
			//int input = (int)*iter;
			//cout << "in:" << input << endl;
			if(*iter != '0' && *iter != '1' && *iter != '2' && *iter != '3' && *iter != '4'
				&& *iter != '5' && *iter != '6' && *iter != '7')
			{	
				flag = true;
				break;
			}
			string pushAttribute = employeeStatsAttribute(*iter);
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
		
		if(sql == "ERROR" || flag)
		{
			cerr << "ERROR on input validation try again" << endl;
		}
		else
		{	
			string employee1Sql = "project " + listAttributes + " " + selectJoinedTBL_1;
			string employee2Sql = "project " + listAttributes + " " + selectJoinedTBL_2;
			sql = "("+ employee1Sql + ") - (" +employee2Sql + ")";
			//cout << "e1: " << employee1Sql << endl;
			//cout << "e2: " << employee2Sql << endl;
			//cout << "sql: " << sql << endl;
			string sqltosend = "SHOW (" + sql + ");\n";
			//cout<<sqltosend<<endl;
			//cout<<"WRITING"<<endl;
			writePipe(sqltosend);
		//	cout<<"DONE"<<endl;
			cout<<readPipe();
			//cout<<"FINISHED READING"<<endl;
		}
}
void updateAmountOwed()
{
	string sql;
	cout << "\n\n";
	cout << " ---- Update Customer Amount Owed ---- " << endl;
	cout << "\nWhat is the Customer's Name?" << endl;
	cout << ">";
	string customerName;
	cin >> customerName;
	
	cout << "\nWhat is the Date of Purchase? month/day/year (01/05/1990)" << endl;
	cout << ">";
	string date;
	cin >> date;
	string dateOfPurchase;
	for(int z =0; z<date.size(); z++)
	{
		if(isdigit(date[z]))
			dateOfPurchase+=date[z];
	}
	string enteredDate = dateOfPurchase.substr(4,4);
	//cout<<enteredDate<<endl;
	enteredDate += dateOfPurchase.substr(0,2);
	//cout<<enteredDate<<endl;
	enteredDate += dateOfPurchase.substr(2,2);
	//cout<<enteredDate<<endl;
	dateOfPurchase = enteredDate;
	
	cout << "\nWhat is the new amount that this customer owes?"<<endl;
	cout << ">";
	string amountOwed;
	cin >> amountOwed;
	
	sql = "UPDATE owns SET amountOwed = " +amountOwed+" WHERE (customerName == \"" 
		+customerName+ "\" && dateOfPurchase == " + dateOfPurchase + ");\n";
		
	
	
	
	if(!isNumber(amountOwed))
		sql = "ERROR";
	
	if(sql != "ERROR")
	{
		writePipe(sql);
		cout<<readPipe();
		sql = "UPDATE sales SET amountOwed = " +amountOwed+" WHERE (customerName == \"" 
			+customerName+ "\" && dateOfPurchase == " + dateOfPurchase + ");\n";
		writePipe(sql);
		cout<<readPipe();
	}
	else
		cerr << "ERROR you entered wrong information for the Update Customer Amount Owed" << endl;
	
}

int main()
{	
	//readbuffer[255] = ' ';
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
		exit(0);
	}
	else
	{
		close( fileDescriptor_ParentToChild[0]);//close read end of p2c
		close(fileDescriptor_ChildToParent[1]);//close write end of pFc
		guarunteedOpen();
		string choice = "0";
		while(stoi(choice) !=9)
		{
			cout << "\n\n";
			cout << "------------------------- MENU -------------------------" << endl;
			cout << "--------------------------------------------------------" << endl;
			cout<<"List of available commands"<<endl;
			cout<<"Please enter the number of the desired command"<<endl;
			//cout<<"0. Show Table (For Debugging will not be in final project)"<<endl;
			cout<<"1. Show Table"<<endl;
			cout<<"2. Employee Sale stats"<<endl;
			cout<<"3. Compare Employees"<<endl;
			cout<<"4. Make a sale"<<endl;
			cout<<"5. Look up a sale"<<endl;
			cout<<"6. Search by name/model/employee/etc."<<endl;
			cout<<"7. Remove a pending Sale" << endl;
			//cout<<"8. Switch between offline/online modes"<<endl;
			cout<<"8. Update Amount Owed with a Customer"<<endl;
			cout<<"9. Quit"<<endl;
			cout << "--------------------------------------------------------" << endl;
			cout << "--------------------------------------------------------" << endl;
			cout << "\n" << endl;
			cout << ">";
			cin>>choice;
			while(!isNumber(choice))
			{
				cout<<"Comon man, enter a number"<<endl;
				cout << ">";
				cin>>choice;
			}
			string sql1 = "";
			string sqltosend = "";
			switch(stoi(choice))
			{
				//case 0:
				//	show();
				//break;
				case 1:
					show();
				break;
				case 2:
					employeeStats();
				break;
				case 3:
					CompareEmployeeStats();
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
					removeSale();
				break;
				case 8:
					/*if(salesDB == "offline_sales")
					{
						cout<<"Going online"<<endl;
						goOnline();
					}
					else
					{
						cout<<"Going offline"<<endl;
						goOffline();
					}*/
					updateAmountOwed();
				break;
				case 9:
					writePipe("EXIT;\n");
					readPipe();
				break;
				case 10:
					writeTable();
				break;
				default:
					cerr << "Sorry this is not a command" << endl;
				break;
			}
		}
		cout<<"Goodbye, have a great day and thanks for using our program"<<endl;
	}
}