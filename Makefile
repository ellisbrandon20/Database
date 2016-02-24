all: Main

Main: Attribute.o Record.o Table.o Database.o Parser.o main.o 
	g++ -std=c++11 Attribute.o Record.o Table.o Parser.o Database.o main.o -o main
	
Attribute.o: Attribute.cpp Attribute.h
	g++ -std=c++11 -c Attribute.cpp
	
Record.o: Record.cpp Record.h
	g++ -std=c++11 -c Record.cpp

Table.o: Table.cpp Table.h Record.h Attribute.h
	g++ -std=c++11 -c Table.cpp
	
Database.o: Database.cpp Database.h Table.h Record.h Attribute.h
	g++ -std=c++11 -c Database.cpp

Parser.o: Parser.cpp Parser.h Database.h
	g++ -std=c++11 -c Parser.cpp
	
main.o: main.cpp Parser.h
	g++ -std=c++11 -c main.cpp
	
clean:
	rm *.o main