class Order
{
	int financingRate;
	int cost;
	int orderID;
	
	public:
	Order() {financingRate = -1; cost = -1; orderID = -1;}
	Order(int newFinancingRate, int newCost, int newOrderID) {financingRate = newFinancingRate; cost = newCost; orderID = newOrderID;}
	getFinancingRate() {return financingRate;}
	getCost() {return cost;}
	getOrderID() {return orderID;}
}