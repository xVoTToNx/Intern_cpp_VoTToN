#pragma once
#include <vector>

#include "ant_edge.hpp"

namespace aa {

	template<typename LengthType, typename PheromoneType>
	class Ant
	{
		int const destiantion_;
		int const start_;
		int currentPosition_;
		LengthType pathLength_;
		std::vector<LengthType> path_;

	public:

		Ant(int const& start, int const& destination)
			: start_(start)
			, currentPosition_(start_)
			, destiantion_(destination)
			, pathLength_(0)
		{
			path_.push_back(currentPosition_);
		}

		~Ant() {}

		void Reset()
		{
			pathLength_ = 0;
			currentPosition_ = start_;
			path_.clear();
			path_.push_back(currentPosition_);
		}

		std::vector<LengthType> Path() const
		{
			return path_;
		}

		LengthType PathLength() const
		{
			return pathLength_;
		}

		int CurrentPosition() const
		{
			return currentPosition_;
		}

		bool isArrived() const
		{
			if (currentPosition_ == destiantion_)
				return true;
			return false;
		}

		void MoveTo(AntEdge<LengthType, PheromoneType> const& edge)
		{
			pathLength_ += edge.Length();
			path_.push_back(edge.Index());
			currentPosition_ = edge.Index();
		}
	};
}