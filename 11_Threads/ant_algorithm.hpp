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
		std::vector<std::vector<aa::AntEdge<LengthType, PheromoneType>>> const& graph,
		AntAlgorithm mod)
	{
		while (true)
		{
			std::vector<aa::AntEdge<LengthType, PheromoneType>> possiblePaths;
			PheromoneType pheromoneSum = 0;
			LengthType lengthSum = 0;

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
						possiblePaths.push_back(possibleEdge);
						pheromoneSum += possibleEdge.Pheromone();
						lengthSum += possibleEdge.Length();
					}
				}
			}


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


			int chosen = -1;
			for (auto i = possiblePaths.begin(); i != possiblePaths.end(); ++i)
			{
				std::vector<LengthType> antPath = ant->Path();
				float chanceForThisPath;
				switch (mod)
				{
				case AntAlgorithm::PHERO:
						if (pheromoneSum != 0)
							chanceForThisPath = float(i->Pheromone()) / pheromoneSum;
						else
							chanceForThisPath = float(1.0) / possiblePaths.size();
						chanceStack += chanceForThisPath;
						break;

				case AntAlgorithm::LENGTH:
					chanceForThisPath = float(i->Length()) / lengthSum;
					chanceStack += chanceForThisPath;
					break;

				case AntAlgorithm::FULL:
					if (pheromoneSum != 0)
						chanceForThisPath = float(i->Pheromone()) * i->Length() / (lengthSum + pheromoneSum);
					else
						chanceForThisPath = float(i->Length()) / lengthSum;
					chanceStack += chanceForThisPath;
					break;
				}

				if (randNum < chanceStack)
				{
					chosen = i->Index();
					ant->MoveTo(*i);
					hasMoved = true;
					break;
				}

			}

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
		double vaporizeCoef = 0.5, int const& antsNumber = 2, int const& runsNumber = 100,
		AntAlgorithm mod = AntAlgorithm::PHERO)
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
				threads.push_back(std::thread(RunAnt<LengthType, PheromoneType>, &(*i), graph, mod));
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
			++currentRun;
		}

		return bestPath;
	}
}
