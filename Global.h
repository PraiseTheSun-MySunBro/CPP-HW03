#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <deque>
#include <functional>


enum Heading
{
	N, S, W, E
};

const int HEADING_SIZE = 4;

enum CellType
{
	Treasure = -2, Wall = -1, StartPos = 0
};

typedef struct Position
{
	int x;
	int y;

	/*bool operator<(const Position & other) const
	{
		return std::pair<int, int>(x, y) < std::pair<int, int>(other.x, other.y);
	}*/

	bool operator==(const Position & other) const
	{
		return x == other.x && y == other.y;
	}

	Position(int x, int y) : x(x), y(y) {};
};

template <>
struct std::hash<Position>
{
	std::size_t operator()(const Position & position) const
	{
		auto temp = position.y + (position.x + 1) / 2;
		return position.x + temp * temp;
	}
};


typedef struct Direction
{
	Heading heading;
	int cost;

	Direction(Heading heading, int cost) : heading(heading), cost(cost) {};
};

typedef struct RouteDirection
{
	std::deque<Heading> headingDirections;
	int cost;

	RouteDirection(std::deque<Heading> headingDirections, int cost) : headingDirections(headingDirections), cost(cost) {};
};

template <typename T>
const bool contains(std::vector<T> & vector, const T & value)
{
	return std::find(vector.begin(), vector.end(), value) != vector.end();
}