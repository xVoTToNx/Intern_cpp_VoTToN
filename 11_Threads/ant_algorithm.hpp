#pragma once

#include <thread>
#include <limits>
#include <random> 

#include "ant.hpp"
#include "ant_edge.hpp"

#define matrix(X) std::vector<std::vector<X>>

namespace aa {

	template<typename LengthType = int, typename PheromoneType = float>
	void RunAnt(aa::Ant<LengthType, PheromoneType>* ant,
		std::vector<std::vector<aa::AntEdge<LengthType, PheromoneType>>> const& graph)
	{
		while (true)
		{
			std::vector<aa::AntEdge<LengthType, PheromoneType>> possiblePaths;
			PheromoneType pheromoneSum = 0;

			//std::cout << "\nPheromones\n[ ";


			std::default_random_engine eng((std::random_device())());

			for (int i = 0; i < graph.size(); ++i)
			{
				// If not current and not one of previouses ones
				std::vector<LengthType> antPath = ant->Path();
				aa::AntEdge<LengthType, PheromoneType> possibleEdge = graph[ant->CurrentPosition()][i];
				if (possibleEdge.Length() != 0)
				{
					auto it = std::find(antPath.begin(), antPath.end(), possibleEdge.Index());

					if (it == antPath.end())
					{
						//std::cout << possibleEdge.Pheromone() << ", ";
						possiblePaths.push_back(possibleEdge);
						pheromoneSum += possibleEdge.Pheromone();
					}
				}
			}

			/*std::cout << "\b\b ]" << std::endl;

			std::cout << "\nPheroSum: " << pheromoneSum << std::endl;

			std::cout << "\nPossiblePath\n[ ";
			for (int i = 0; i < possiblePaths.size(); ++i)
			{
				std::cout << possiblePaths[i].Index() << ", ";
			}
			std::cout << "\b\b ]" << std::endl;
			*/
			std::uniform_real_distribution<float> random(0, 1.0);
			float randNum = random(eng);

			float chanceStack = 0;
			bool hasMoved = false;

			// CHANCE STACK EXPLANATION
			// For example
			// RandNum == 0, 5
			//
			// 1st path 
			// chanceForThisPath == 0,3
			// chanceBuffer = 0 + 0.3 = 0.3
			// RandNum > chanceBuffer, go next
			//
			// 2st path 
			// chanceForThisPath == 0,4
			// chanceBuffer = 0.4 + 0.3 = 0.7
			// RandNum < chanceBuffer, moveTo(thisPath)
			  ////std::cout << "\nRandNum:\n" << randNum << std::endl;
			  ////std::cout << "\nChances:\n[ ";
			int chosen = -1;
			for (auto i = possiblePaths.begin(); i != possiblePaths.end(); ++i)
			{
				std::vector<LengthType> antPath = ant->Path();

				float chanceForThisPath;
				if (pheromoneSum != 0)
					chanceForThisPath = i->Pheromone() / pheromoneSum;
				else
					chanceForThisPath = 1.0 / possiblePaths.size();
				chanceStack += chanceForThisPath;
				//std::cout << chanceForThisPath << ", ";
				if (randNum < chanceStack)
				{
					chosen = i->Index();
					ant->MoveTo(*i);
					hasMoved = true;
					break;
				}

			}
			/*std::cout << "\b\b ]" << std::endl;
			std::cout << "\nChosen: " << chosen;
			std::cout << "\nHas moved?" << std::endl;
			if(hasMoved)
			std::cout << "Yes" << std::endl;
			else
				std::cout << "No" << std::endl;
*/
			/*std::cout << "\nPath\n[ ";
			for (int i = 0; i < ant->Path().size(); ++i)
			{
				std::cout << ant->Path()[i] << ", ";
			}
			std::cout << "\b\b ]" << std::endl;
			*/
			if (ant->isArrived())
			{
				return;
			}
		}
	}

	template<typename LengthType = int, typename PheromoneType = float>
	std::vector<LengthType> antAlgorithm(
		matrix(LengthType) const& lengthMatrix, 
		matrix(PheromoneType) const& pheromoneMatrix,
		int  const& startIndex, int  const& destinationIndex, 
		double vaporizeCoef = 0.5, int const& antsNumber = 2, int const& runsNumber = 100)
	{
		// Single matrix for length and pheromone
		std::vector<std::vector<AntEdge<LengthType, PheromoneType>>> graph;

		for (int i = 0; i != lengthMatrix.size(); ++i)
		{
			graph.push_back(std::vector<AntEdge<LengthType, PheromoneType>>());

			for (int j = 0; j != lengthMatrix[i].size(); ++j)
			{
				graph[i].push_back(AntEdge<LengthType, PheromoneType>(lengthMatrix[i][j], j, pheromoneMatrix[i][j]));
			}
		}

		// Breed ants ^.^

		std::vector<Ant<LengthType, PheromoneType>> ants(antsNumber, Ant<LengthType, PheromoneType>(startIndex, destinationIndex));
		std::vector<std::thread> threads;
		std::vector<int> bestPath;
		LengthType bestPathLength = std::numeric_limits<LengthType>::max();

		int currentRun = 0;

		while (currentRun < runsNumber)
		{
			// Start all ants;
			for (auto i = ants.begin(); i != ants.end(); ++i)
			{
				threads.push_back(std::thread(RunAnt<LengthType, PheromoneType>, &(*i), graph));
			}

			// Wait until everyone has finished;
			for (int i = 0; i < antsNumber; ++i)
			{
				threads[i].join();
			}

			threads.clear();

			// Calculate new pheromone values
			for (auto i = ants.begin(); i != ants.end(); ++i)
			{
				std::vector<LengthType> antPath = i->Path();
				/*std::cout << "\nPathFINALL\n[ ";
				for (int i = 0; i < antPath.size(); ++i)
				{
					std::cout << antPath[i] << ", ";
				}
				std::cout << "\b\b ]" << std::endl;
				*/
				//std::cin.get();
				PheromoneType deltaPheromone = 1.0 / i->PathLength();


				if (i->PathLength() < bestPathLength)
				{
					bestPathLength = i->PathLength();
					bestPath = i->Path();
				}

				auto jPrevious = antPath.begin();
				for (auto j = ++antPath.begin(); j != antPath.end(); ++j)
				{
					graph[*jPrevious][*j].Vaporize(vaporizeCoef);
					graph[*jPrevious][*j].ChangePheromone(deltaPheromone);

					graph[*j][*jPrevious].Vaporize(vaporizeCoef);
					graph[*j][*jPrevious].ChangePheromone(deltaPheromone);
					jPrevious = j;
				}

				i->Reset();
			}
			/*for (int i = 0; i < 5; ++i)
			{
				std::cout << std::endl;
				for (int j = 0; j < 5; ++j)
				{
					std::cout << rounDD(graph[i][j].Pheromone()) << "             ";
				}
			}
			std::cout << std::endl;
			*/
			++currentRun;
			//d(currentRun);
		}

		return bestPath;
	}
}
