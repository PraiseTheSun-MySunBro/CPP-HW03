#pragma once

#include "Global.h"
#include "MazeRunner.h"
#include "Node.h"


class Solver
{
public:
	std::vector<Heading> solve();

	Solver(MazeRunner & mazeRunner) : mazeRunner(mazeRunner), root(nullptr), treasureNode(nullptr), illegalDirections({ CellType::Wall, CellType::StartPos }) {};
	~Solver();
private:
	MazeRunner & const mazeRunner;
	std::shared_ptr<Node> root;
	std::shared_ptr<Node> treasureNode;
	std::vector<int> illegalDirections;

	void setTreasureNode(std::shared_ptr<Node> currentNode);
	std::vector<Direction> getPossibleDirections();
	void move(Direction direction, std::vector<Direction> & route);
};