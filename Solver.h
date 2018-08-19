#pragma once

#include "Global.h"
#include "MazeRunner.h"
#include "Node.h"
#include <deque>


class Solver
{
public:
	std::vector<Heading> solve();

	Solver(MazeRunner & mazeRunner) : mazeRunner(mazeRunner), root(nullptr), treasureNode(nullptr), illegalDirections({ CellType::Wall, CellType::StartPos }) {};
	~Solver()
	{
		delete root;
		root = nullptr;

		delete treasureNode;
		treasureNode = nullptr;

		for (auto entry : Node::nodes)
		{
			delete entry.second;
			entry.second = nullptr;
		}
		Node::nodes.clear();
	}
private:
	MazeRunner & const mazeRunner;
	Node * root;
	Node * treasureNode;
	std::vector<int> illegalDirections;

	void setTreasureNode(Node * currentNode);
	std::vector<Direction> getPossibleDirections(Node const * currentNode);
	std::vector<Direction> getPossibleDirections(Heading lastHeading);
	void move(std::deque<Heading> headings);
	bool move(Direction direction, std::deque<Direction> & route);
	void moveToNode(Node const * destinationNode);
	Heading getReversedHeading(Heading heading);
	std::deque<Heading> getReversedHeadings(std::deque<Heading> const & route);
	std::deque<Heading> getRouteBetweenNodes(Node const * currentNode, Node const * destinationNode);
	Heading getRouteHeadingBetweenNodes(Node const * currentNode, Node const * destinationNode);
	void moveBack(std::deque<Direction> route);
	void setNodeMetadata(Node * currentNode, Node * childNode, std::deque<Direction> const & route);
	int calculateDistance(Node const * currentNode, std::deque<Direction> const & route);
	RouteDirection* collectRoute(std::deque<Direction> const & route, int distanceCost);
	RouteDirection* collectReversedRoute(std::deque<Direction> const & route, int distanceCost);
};