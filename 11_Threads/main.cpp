#include "ant_algorithm.hpp"
#include <iostream>

int main()
{
	std::vector<std::vector<float>> phero;
	phero.push_back(std::vector<float>(5, 0));
	phero.push_back(std::vector<float>(5, 0));
	phero.push_back(std::vector<float>(5, 0));
	phero.push_back(std::vector<float>(5, 0));
	phero.push_back(std::vector<float>(5, 0));

	std::vector<std::vector<int>> len;
	len.push_back(std::vector<int>{0, 1, 0, 0, 3});
	len.push_back(std::vector<int>{1, 0, 5, 8, 10});
	len.push_back(std::vector<int>{0, 5, 0, 1, 0});
	len.push_back(std::vector<int>{0, 8, 1, 0, 1});
	len.push_back(std::vector<int>{3, 10, 0, 1, 0});

	std::vector<int> kek = aa::antAlgorithm<int, float>(len, phero, 1 - 1, 4 - 1, 0.4, 5, 100, aa::AntAlgorithm::LENGTH);

	std::cout << "[ ";
	for (int i = 0; i < kek.size(); ++i)
	{
		std::cout << kek[i] << ", ";
	}
	std::cout << "\b\b ]" << std::endl;

	return 0;
}