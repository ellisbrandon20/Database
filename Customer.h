#include "string.h"
class Customer
{
	string name;
	int dateOfPurchase;
	
	public:
	Customer() {name = " BADNAME"; dateOfPurchase = -1;}
	Customer(string newName, int dop) {name = newName; dateOfPurchase = dop; }
	string getName() {return name;}
	int getDateOfPurchase() {return dateOfPurchase;}
}