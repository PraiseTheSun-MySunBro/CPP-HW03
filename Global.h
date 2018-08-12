#pragma once

#include <utility>
#include <algorithm>
#include <vector>


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

	bool operator < (const Position & o) const
	{
		return std::pair<int, int>(x, y) < std::pair<int, int>(o.x, o.y);
	}

	Position(int x, int y) : x(x), y(y) {};
};

typedef struct Direction
{
	Heading heading;
	int cost;

	Direction(Heading heading, int cost) : heading(heading), cost(cost) {};
};

template <typename T>
const bool contains(std::vector<T> & vector, const T & value)
{
	return std::find(vector.begin(), vector.end(), value) != vector.end();
}