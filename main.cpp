
#include "Parser.h"
#include <cstring>
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


int main()
{
	Parser p;	
	string input;
	int bytes_read;
	while(p.getExit()==false && getline(cin, input))
	{
		cout<<input<<endl;
		if(input.size()!=1)
			p.parse(input);
		input = "";
	}
	cout<<"Leaving"<<endl;
}