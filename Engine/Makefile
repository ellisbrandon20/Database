all: Main

Main: Attribute.o Record.o Table.o Database.o testMain.o 
	g++ -std=c++11 Attribute.o Record.o Table.o Parser.o Database.o testMain.o -o a.out
	
Attribute.o: Attribute.cpp Attribute.h
	g++ -std=c++11 -c Attribute.cpp
	
Record.o: Record.cpp Record.h
	g++ -std=c++11 -c Record.cpp

Table.o: Table.cpp Table.h Record.h Attribute.h
	g++ -std=c++11 -c Table.cpp
	
Database.o: Database.cpp Database.h Table.h Record.h Attribute.h
	g++ -std=c++11 -c Database.cpp

testMain.o: testMain.cpp Database.h
	g++ -std=c++11 -c testMain.cpp
	
clean:
	rm *.o a.out

run:
	./a.out