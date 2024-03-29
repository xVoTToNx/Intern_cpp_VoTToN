#include "CMake.cpp"
#include <gtest/gtest.h>

class TestData : public ::testing::Environment {
protected:
	Data* data;

	virtual void SetUp() override {
		data = new Data;
		data->Insert<int>("test11", 44);
		data->Insert<double>("test12", 54.6);
		data->Insert<int>("test123", 0);
	}

	virtual void TearDown() override {
		delete data;
	}
};

TEST_F(TestData, test1) {
	EXPECT_EQ("0", nlohmann::to_string(data->GetMinValue()));
	EXPECT_TRUE(data->Remove("test123"));
	EXPECT_EQ("54.6", nlohmann::to_string(data->GetMaxValue()));
	EXPECT_EQ("44", nlohmann::to_string(data->GetMinValue()));
}

TEST_F(TestData, test2) {
	EXPECT_EQ("[44]", data->Find("test11"));
	EXPECT_EQ("[54.6]", data->Find("test12"));
	EXPECT_FALSE(data->Remove("test1234"));
}

TEST_F(TestData, test3) {
	Data d;
	EXPECT_ANY_THROW(d.GetMaxValue());
	EXPECT_NO_THROW(d.Dump());
	EXPECT_NEAR(5, 34, 30);
	data->Insert<int>("test113", 44);
	EXPECT_STREQ(data->Find("test11").c_str(), data->Find("test113").c_str());
	EXPECT_STRNE(data->Find("test11").c_str(), data->Find("test12").c_str());
}

::testing::Environment* const foo_env =
	::testing::AddGlobalTestEnvironment(new TestData);

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}