#include "stdafx.h"
#include "Solver.h"
#include <iostream>
#include <queue>
#include <ctime>
#include <cstdio>

std::unordered_map<Position, Node *> Node::nodes;

static std::vector<std::string> debug;

void Solver::setTreasureNode(Node * currentNode)
{
	if (currentNode->value != CellType::Treasure) return;

	treasureNode = currentNode;
	printf_s("Found treasure position: (%d,%d)\n", treasureNode->position.x, treasureNode->position.y);
}

std::vector<Direction> Solver::getPossibleDirections(Node const * currentNode)
{
	std::vector<Direction> possibleDirections;

	auto map = mazeRunner.scan();

	int cost = map[0][1];
	if (cost != CellType::Wall && !currentNode->route->routes[Heading::N])
	{
		possibleDirections.push_back(Direction(Heading::N, cost));
	}

	cost = map[1][0];
	if (cost != CellType::Wall && !currentNode->route->routes[Heading::W])
	{
		possibleDirections.push_back(Direction(Heading::W, cost));
	}

	cost = map[1][2];
	if (cost != CellType::Wall && !currentNode->route->routes[Heading::E])
	{
		possibleDirections.push_back(Direction(Heading::E, cost));
	}

	cost = map[2][1];
	if (cost != CellType::Wall && !currentNode->route->routes[Heading::S])
	{
		possibleDirections.push_back(Direction(Heading::S, cost));
	}

	return possibleDirections;
}

std::vector<Direction> Solver::getPossibleDirections(Heading lastHeading)
{
	std::vector<Direction> possibleDirections;

	auto map = mazeRunner.scan();

	int cost = map[0][1];
	if (cost != CellType::Wall && lastHeading != getReversedHeading(Heading::N))
	{
		possibleDirections.push_back(Direction(Heading::N, cost));
	}

	cost = map[1][0];
	if (cost != CellType::Wall && lastHeading != getReversedHeading(Heading::W))
	{
		possibleDirections.push_back(Direction(Heading::W, cost));
	}

	cost = map[1][2];
	if (cost != CellType::Wall && lastHeading != getReversedHeading(Heading::E))
	{
		possibleDirections.push_back(Direction(Heading::E, cost));
	}

	cost = map[2][1];
	if (cost != CellType::Wall && lastHeading != getReversedHeading(Heading::S))
	{
		possibleDirections.push_back(Direction(Heading::S, cost));
	}

	return possibleDirections;
}

void Solver::move(std::deque<Heading> headings)
{
	if (headings.size() == 0) return;

	auto heading = headings.front();
	mazeRunner.move(heading);
	headings.pop_front();

	move(headings);
}

bool Solver::move(Direction direction, std::deque<Direction> & route)
{
	mazeRunner.move(direction.heading);
	route.push_back(direction);

	auto possibleDirections = getPossibleDirections(direction.heading);

	if (possibleDirections.size() == 0)
	{
		return false;
	}

	if (possibleDirections.size() == 1)
	{
		return move(possibleDirections[0], route);
	}

	return true;
}

std::deque<Heading> Solver::getRouteBetweenNodes(Node const * currentNode, Node const * destinationNode)
{
	for (auto i = 0; i < HEADING_SIZE; ++i)
	{
		if (currentNode->route->nodes[i] && *currentNode->route->nodes[i] == *destinationNode)
		{
			return currentNode->route->routes[i]->headingDirections;
		}
	}
}

Heading Solver::getRouteHeadingBetweenNodes(Node const * currentNode, Node const * destinationNode)
{
	for (auto i = 0; i < HEADING_SIZE; ++i)
	{
		if (currentNode->route->nodes[i] && *currentNode->route->nodes[i] == *destinationNode)
		{
			return (Heading) i;
		}
	}
}

void Solver::moveToNode(Node const * destinationNode)
{
	if (!destinationNode) return;

	auto currentNode = Node::nodes.at(mazeRunner.getPosition());

	if (*currentNode == *destinationNode) return;

	auto beginNode = std::begin(currentNode->route->nodes);
	auto endNode = std::end(currentNode->route->nodes);

	auto node = std::find(beginNode, endNode, destinationNode);
	if (node != endNode)
	{
		auto route = getRouteBetweenNodes(currentNode, destinationNode);
		move(route);
	}
	else
	{
		auto route = getRouteBetweenNodes(currentNode, currentNode->parent);
		move(route);
		moveToNode(destinationNode);
	}
}

std::vector<Heading> Solver::solve()
{
	if (mazeRunner.getSize().first == 0 || mazeRunner.getSize().second == 0)
	{
		std::cout << "Map is empty" << std::endl;
		return std::vector<Heading>();
	}

	auto startTime = clock();

	auto stack = std::vector<Node *>();
	root = new Node(CellType::StartPos, mazeRunner.getPosition());
	stack.push_back(root);

	while (!stack.empty())
	{
		auto currentNode = stack.back();
		stack.pop_back();

		setTreasureNode(currentNode);
		moveToNode(currentNode);

		auto possibleDirections = getPossibleDirections(currentNode);
		for (auto direction : possibleDirections)
		{
			std::deque<Direction> route;
			if (move(direction, route))
			{
				Node * childNode = nullptr;
				if (!Node::nodes.count(mazeRunner.getPosition()))
				{
					debug.push_back("Creating node: (" + std::to_string(mazeRunner.getPosition().x) + ";" + std::to_string(mazeRunner.getPosition().y) + ") at: " + std::to_string(clock() - startTime));
					childNode = new Node(mazeRunner.scan()[1][1], mazeRunner.getPosition());
					childNode->parent = currentNode;
					stack.push_back(childNode);
				}
				else 
				{
					childNode = Node::nodes.at(mazeRunner.getPosition());
				}

				setNodeMetadata(currentNode, childNode, route);
			}

			moveBack(route);
		}

		/*if (!currentNode->parent) continue;

		int count = 0;
		Node * childNode = nullptr;

		for (auto i = 0; i < HEADING_SIZE; ++i)
		{
			if (currentNode->route->nodes[i])
			{
				++count;

				if (currentNode->parent && !(*currentNode->route->nodes[i] == *currentNode->parent))
				{
					childNode = currentNode->route->nodes[i];
				}
			}
		}

		if (count == 2)
		{
			auto parentHeading = getRouteHeadingBetweenNodes(currentNode->parent, currentNode);
			auto currentHeading = getRouteHeadingBetweenNodes(currentNode, childNode);

			auto parentRoute = currentNode->parent->route->routes[parentHeading];
			auto currentRoute = currentNode->route->routes[currentHeading];

			auto childHeading = getReversedHeading(currentRoute->headingDirections.back());

			int sum = parentRoute->cost + currentRoute->cost;

			auto finalRoute = std::deque<Heading>();

			finalRoute = parentRoute->headingDirections;
			for (auto s : currentRoute->headingDirections)
			{
				finalRoute.push_back(s);
			}

			currentNode->parent->route->nodes[parentHeading] = childNode;
			childNode->route->nodes[childHeading] = currentNode->parent;

			currentNode->parent->route->routes[parentHeading] = new RouteDirection(finalRoute, sum);
			childNode->route->routes[childHeading] = new RouteDirection(getReversedHeadings(finalRoute), sum);

			childNode->parent = currentNode->parent;

			currentNode->parent = nullptr;
			std::fill_n(currentNode->route->nodes, HEADING_SIZE, nullptr);

			delete currentNode;
			currentNode = nullptr;
		}*/
	}

 	return std::vector<Heading>();
}

void Solver::setNodeMetadata(Node * currentNode, Node * childNode, std::deque<Direction> const & route)
{
	auto currentNodeHeading = route.front().heading;
	auto childNodeHeading = getReversedHeading(route.back().heading);
	
	currentNode->route->nodes[currentNodeHeading] = childNode;
	childNode->route->nodes[childNodeHeading] = currentNode;

	auto distanceCost = calculateDistance(currentNode, route);

	currentNode->route->routes[currentNodeHeading] = collectRoute(route, distanceCost);
	childNode->route->routes[childNodeHeading] = collectReversedRoute(route, distanceCost);
}

int Solver::calculateDistance(Node const * currentNode, std::deque<Direction> const & route)
{
	int sum = 0;

	for (auto dir : route)
	{
		sum += dir.cost;
	}

	return sum;
}

RouteDirection* Solver::collectRoute(std::deque<Direction> const & route, int distanceCost)
{
	auto headings = std::deque<Heading>();

	for (auto dir : route)
	{
		headings.push_back(dir.heading);
	}

	return new RouteDirection(headings, distanceCost);
}

RouteDirection* Solver::collectReversedRoute(std::deque<Direction> const & route, int distanceCost)
{
	auto headings = std::deque<Heading>();

	for (auto dir : route)
	{
		headings.push_front(getReversedHeading(dir.heading));
	}

	return new RouteDirection(headings, distanceCost);
}

std::deque<Heading> Solver::getReversedHeadings(std::deque<Heading> const & route)
{
	auto reversedRoute = std::deque<Heading>();

	for (auto h : route)
	{
		reversedRoute.push_front(getReversedHeading(h));
	}

	return reversedRoute;
}

Heading Solver::getReversedHeading(Heading heading)
{
	switch (heading)
	{
	case Heading::N:
		return Heading::S;
	case Heading::S:
		return Heading::N;
	case Heading::W:
		return Heading::E;
	case Heading::E:
		return Heading::W;
	}

	throw;
}

void Solver::moveBack(std::deque<Direction> route)
{
	if (route.size() == 0) return;

	auto direction = route.back();
	auto backHeading = getReversedHeading(direction.heading);
	route.pop_back();

	mazeRunner.move(backHeading);
	moveBack(route);
}

