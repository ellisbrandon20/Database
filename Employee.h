class Employee
{
	string name;
	int EmployeeID;
	
	public:
	Employee() {name = " BADNAME"; EmployeeID = -1;}
	Employee(string newName, int newID) {name = newName; EmployeeID = newID}
	~Employee()
	getName() {return name;}
	getID() {return EmployeeID;}
};