class Car
{
	string color;
	int year;
	string model;
	string make;
	
	public:
	Car() {color = " BADCOLOR"; year = -1; model = " BADMODEL"; make = " BADMAKE";}
	Car(string newColor, int newYear, string newModel, string newMake) {color = newColor; year = newYear; model = newModel; make = newMake;}
	~Car();
	string getColor() {return color;}
	int getYear() {return year;}
	string getModel() {return model;}
	string getMake() {return make;}
}