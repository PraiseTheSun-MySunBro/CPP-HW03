#include "stdafx.h"
#include "MazeRunner.h"
#include <fstream>
#include <iostream>


MazeRunner::MazeRunner(std::string fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw std::exception("File not found");
	}

	auto lineNumber = 0;

	std::string fileLine;
	while (std::getline(file, fileLine))
	{
		auto line = std::vector<int>();

		for (int i = 0; i < fileLine.size(); ++i)
		{
			if (fileLine[i] == 'B')
			{
				line.push_back(CellType::StartPos);
				x = i;
				y = lineNumber;
			}
			else if (fileLine[i] == 'T')
			{
				line.push_back(CellType::Treasure);
			}
			else if (fileLine[i] >= '1' && fileLine[i] <= '9')
			{
				line.push_back(fileLine[i] - '0');
			}
			else
			{
				line.push_back(CellType::Wall);
			}
		}

		lineNumber++;
		map.push_back(line);
	}

	file.close();
}

MazeRunner::~MazeRunner()
{

}

bool MazeRunner::move(Heading heading)
{
	if (heading == Heading::N && y > 0 && (map[y - 1][x] >= 0 || map[y - 1][x] == CellType::Treasure))
	{
		y--;
		return true;
	}
	if (heading == Heading::E && x < map[y].size() - 1 && (map[y][x + 1] >= 0 || map[y][x + 1] == CellType::Treasure))
	{
		x++;
		return true;
	}
	if (heading == Heading::S && y < map.size() - 1 && (map[y + 1][x] >= 0 || map[y + 1][x] == CellType::Treasure))
	{
		y++;
		return true;
	}
	if (heading == Heading::W && x > 0 && (map[y][x - 1] >= 0 || map[y][x - 1] == CellType::Treasure))
	{
		x--;
		return true;
	}

	return false;
}

std::vector<std::vector<int>> MazeRunner::scan()
{
	auto result = std::vector<std::vector<int>>();

	for (int i = y - 1; i < y + 2; ++i)
	{
		auto line = std::vector<int>();
		for (int j = x - 1; j < x + 2; ++j)
		{
			if (i >= 0 && map.size() > i && j >= 0 && map[i].size() > j)
				line.push_back(map[i][j]);
			else
				line.push_back(-1);
		}
		result.push_back(line);
	}

	return result;
}

Position MazeRunner::getPosition()
{
	return Position(this->x, this->y);
}

std::pair<int, int> MazeRunner::getSize()
{
	if (this->map.size() == 0)
	{
		return std::pair<int, int>(0, 0);
	}
	return std::pair<int, int>(this->map.size(), this->map[0].size());
}
