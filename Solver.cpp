#include "stdafx.h"
#include "Solver.h"
#include <iostream>

std::map<Position, Node> Node::nodes;

Solver::~Solver()
{
}

void Solver::setTreasureNode(std::shared_ptr<Node> currentNode)
{
	if (currentNode->value != CellType::Treasure) return;

	treasureNode = currentNode;
	printf_s("Found treasure position: (%d,%d)", treasureNode->position.x, treasureNode->position.y);
}

std::vector<Direction> Solver::getPossibleDirections()
{
	std::vector<Direction> possibleDirections;

	auto map = mazeRunner.scan();

	int cost = map[0][1];
	if (cost != CellType::Wall)
	{
		possibleDirections.push_back(Direction(Heading::N, cost));
	}

	cost = map[1][0];
	if (cost != CellType::Wall)
	{
		possibleDirections.push_back(Direction(Heading::W, cost));
	}

	cost = map[1][2];
	if (cost != CellType::Wall)
	{
		possibleDirections.push_back(Direction(Heading::E, cost));
	}

	cost = map[2][1];
	if (cost != CellType::Wall)
	{
		possibleDirections.push_back(Direction(Heading::S, cost));
	}

	return possibleDirections;
}

void Solver::move(Direction direction, std::vector<Direction> & route)
{
	mazeRunner.move(direction.heading);
	route.push_back(direction);

	auto possibleDirections = getPossibleDirections();
	// TODO: do not count last move

	if (possibleDirections.size() == 0)
	{
		// TODO: throw an error;
		return;
	}

	if (possibleDirections.size() == 1)
	{
		move(possibleDirections[0], route);
	}
}

std::vector<Heading> Solver::solve()
{
	if (mazeRunner.getSize().first == 0 || mazeRunner.getSize().second == 0)
	{
		std::cout << "Map is empty" << std::endl;
		return std::vector<Heading>();
	}

	auto stack = std::vector<std::shared_ptr<Node>>();
	root = std::shared_ptr<Node>(&Node(CellType::StartPos, mazeRunner.getPosition()));
	stack.push_back(root);

	while (!stack.empty())
	{
		auto currentNode = stack.back();
		stack.pop_back();

		setTreasureNode(currentNode);

		auto possibleDirections = getPossibleDirections();
		for (auto direction : possibleDirections)
		{
			std::vector<Direction> route;
			move(direction, route);

			auto childNode = std::shared_ptr<Node>(&Node(mazeRunner.scan()[1][1], mazeRunner.getPosition()));
			stack.push_back(childNode);
		}
	}

	return std::vector<Heading>();
}
