#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <exception>
#include "json.hpp"

using json = nlohmann::json;
using string = std::string;
using jvalue = json::value_type;

class Data
{
	json data;
public:
	Data(const string& path);
	Data() {};

	~Data() { data.clear(); }

	void toFile(const string& path);
	void fromFile(const string& path);
	template<typename T>
	void Insert(const string& key, const T& value) { data[key] = value; }
	string Find(const string& key);
	bool Remove(const string& key);
	jvalue GetMinValue();
	jvalue GetMaxValue();
};
