#pragma once

#include "Global.h"
#include <vector>
#include <memory>

class Node;

class Route
{
public:
	std::shared_ptr<Node> nodes[HEADING_SIZE];
	int distances[HEADING_SIZE];
	std::vector<Heading> routes[HEADING_SIZE];

	Route();
	~Route();
private:
};