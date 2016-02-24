#include <iostream>
#include <vector>

#include <assert.h>
#define CATCH_CONFIG_MAIN
#define BOOST_TEST_MODULE
#include "catch.hpp"
#include "Database.cpp"
using namespace std;

TEST_CASE("CREATE DATABASE"){
	Database db;
	vector<Attribute> attr;
	vector<Record> record;
	Table tbl("test", attr, record);
	
	db.pushBACK(tbl);
	
	SECTION("Empty Attribute List"){
		INFO("Attribute list is not Empty FAILED!");
		REQUIRE(db.getdbTable().size() == 1);
	}
}