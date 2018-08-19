#pragma once

#include "Global.h"
#include <vector>
#include <memory>

class Node;

class Route
{
public:
	Node * nodes[HEADING_SIZE];
	RouteDirection * routes[HEADING_SIZE];

	Route() : nodes(), routes() {};
	~Route()
	{
		delete[] nodes;

		delete[] routes;
	}
private:
};