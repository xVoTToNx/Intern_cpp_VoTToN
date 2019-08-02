#pragma once

namespace aa {

	enum AntAlgorithm
	{
		PHERO,
		FULL,
		LENGTH
	};

	template<typename LengthType, typename PheromoneType>
	class AntEdge
	{
		int const index_;
		LengthType const length_;
		PheromoneType pheromone_;

	public:
		AntEdge(LengthType const& length, int const& index, PheromoneType const& pheromone = 0)
			: length_(length)
			, index_(index)
			, pheromone_(pheromone)
		{}

		AntEdge(AntEdge const&& edge)
			: length_(edge.Length())
			, index_(edge.Index())
			, pheromone_(edge.Pheromone())
		{}

		AntEdge(AntEdge const& edge)
			: length_(edge.Length())
			, index_(edge.Index())
			, pheromone_(edge.Pheromone())
		{}

		~AntEdge() {}

		void Vaporize(double coef)
		{
			pheromone_ *= 1.0 - coef;
		}

		void ChangePheromone(PheromoneType const& deltaPheromone)
		{
			pheromone_ += deltaPheromone;
		}

		LengthType Length() const 
		{
			return length_;
		}

		PheromoneType Pheromone() const 
		{
			return pheromone_;
		}

		int Index() const 
		{
			return index_;
		}

	private:
		AntEdge() : length_(0), index_(0) {}
	};
}
