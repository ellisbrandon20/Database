
#include "Parser.h"

int main()
{
	Parser p;	
	string input; 
	while(p.getExit()==false && getline(cin, input))
	{
		if(input.size()!=1)
			p.parse(input);
		input = "";
	}
}