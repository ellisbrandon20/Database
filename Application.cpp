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
		const char* s ="";
		int choice = 0;
		while(choice !=9)
		{
			cout<<"List of available commands"<<endl;
			cout<<"Please enter the number of the desired command"<<endl;
			cin>>choice;
			switch(choice)
			{
				case 0:
					s = "SHOW animals;\n";
					write( fileDescriptor_ParentToChild[1], s, strlen(s));
					usleep(1000);
					bytes_read = read(fileDescriptor_ChildToParent[0], readbuffer, sizeof(readbuffer)-1);
					readbuffer[bytes_read] = '\0';
					receive_output += readbuffer;
					cout<<receive_output<<endl;
				break;
				case 1:
					s = "OPEN animals;\n";
					write( fileDescriptor_ParentToChild[1], s, strlen(s));
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
				break;
				case 5:
				break;
				case 6:
				break;
				case 7:
				break;
				case 8:
				break;
				case 9:
				break;
			}
		}
		cout<<"Exiting"<<choice<<endl;
	}
	cout<<"Super Gone"<<endl;
}