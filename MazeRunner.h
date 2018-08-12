#pragma once

#include "Global.h"
#include <string>
#include <vector>


class MazeRunner 
{
public:
	bool move(Heading heading);
	std::vector<std::vector<int>> scan();
	Position getPosition();
	std::pair<int, int> getSize();

	MazeRunner(std::string fileName);
	~MazeRunner();
private:
	int x;
	int y;
	std::vector<std::vector<int>> map;
};