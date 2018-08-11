#pragma once

#include <string>
#include <vector>

enum Heading 
{
	N, S, W, E
};

enum CellType
{
	Treasure = -2, Wall = -1, StartPos = 0
};

class MazeRunner 
{
public:
	MazeRunner(std::string fileName);
	~MazeRunner();
	bool move(Heading heading);
	std::vector<std::vector<int>> scan();
	std::pair<int, int> getPosition();
	std::pair<int, int> getSize();
private:
	std::vector<std::vector<int>> map;
	int x;
	int y;
};