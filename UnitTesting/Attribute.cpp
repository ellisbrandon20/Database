#include "Attribute.h"

Attribute::Attribute()
{
	_name = "";
	_type = "";
}

Attribute::Attribute(string name, string type, int maxSize, bool key)
{
	_name = name;
	_type = type;
	_maxSize = maxSize;
	_key = key;
}

Attribute::~Attribute()
{
	_name = "";
	_type = "";
}