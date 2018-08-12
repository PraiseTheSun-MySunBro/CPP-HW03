#pragma once

#include "Global.h"
#include "Route.h"
#include <map>
#include <memory>

class Node
{
public:
	static std::map<Position, Node> nodes;

	const int value;
	const Position position;
	Route route;
	std::shared_ptr<Node> parent;

	Node(int value, Position position) : value(value), position(position), parent(nullptr), route(Route())
	{
		Node::nodes.insert(std::make_pair(position, *this));
	};
	~Node();
private:
};