#include "CMake.h"

using json = nlohmann::json;
using string = std::string;
using jvalue = json::value_type;

Data::Data(const string& path) {
	std::ifstream i(path.c_str());
	i >> data;
}

void Data::toFile(const string& path) {
	std::ofstream o(path.c_str());
	o << std::setw(data.size() + 2) << data << std::endl;
}

void Data::fromFile(const string& path) {
	std::ifstream i(path.c_str());
	i >> data;
}

bool Data::Remove(const string& key) {
	if (data.find(key) != data.end()) {
		data.erase(data.find(key));
		return true;
	}

	return false;
}

string Data::Find(const string& key) {
	json j;
	j.push_back(data[key]);
	return j.dump();
}


jvalue Data::GetMinValue() {
	if (data.is_null()) {
		throw std::runtime_error("Data is empty.");
	}
	auto minItem = data.begin().value();

	for (auto item : data.items()) {
		if (minItem > item.value())
			minItem = item.value();
	}

	return minItem;
}

jvalue Data::GetMaxValue() {
	if (data.is_null()) {
		throw std::runtime_error("Data is empty.");
	}
	auto maxItem = data.begin().value();

	for (auto item : data.items()) {
		if (maxItem < item.value())
			maxItem = item.value();
	}

	return maxItem;
}
