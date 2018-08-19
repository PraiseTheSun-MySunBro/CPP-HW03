#pragma once

#include "Global.h"
#include "Route.h"
#include <memory>
#include <unordered_map>

class Node
{
public:
	static std::unordered_map<Position, Node *> nodes;

	const int value;
	const Position position;
	Route * route;
	Node * parent;

	Node(int value, Position position) : value(value), position(position)
	{
		Node::nodes.insert(std::make_pair(position, this));
		route = new Route();
	};
	~Node()
	{
		delete parent;
		parent = nullptr;

		delete route;
		route = nullptr;
	}
	bool operator==(Node const & other) const;
private:
};