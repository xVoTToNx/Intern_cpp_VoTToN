#include "CMake.cpp"
#include <gtest/gtest.h>

class TestData : public ::testing::Test {
protected:
	Data* data;

	void SetUp() {
		data = new Data;
		data->Insert<int>("test11", 44);
		data->Insert<double>("test12", 54.6);
		data->Insert<int>("test123", 0);
	}

	void TearDown() {
		delete data;
	}
};

TEST_F(TestData, test1) {
	std::cout << std::endl << data->Dump() << std::endl;
	EXPECT_EQ("0", nlohmann::to_string(data->GetMinValue()));
	std::cout << std::endl << data->Dump() << std::endl;
	EXPECT_TRUE(data->Remove("test123"));
	std::cout << std::endl << data->Dump() << std::endl;
	EXPECT_EQ("54.6", nlohmann::to_string(data->GetMaxValue()));
	EXPECT_EQ("44", nlohmann::to_string(data->GetMinValue()));
}

TEST_F(TestData, test2) {
	EXPECT_EQ("[44]", data->Find("test11"));
	EXPECT_EQ("[54.6]", data->Find("test12"));
	EXPECT_FALSE(data->Remove("test1234"));
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}