#include "stdafx.h"
#include "Node.h"

bool Node::operator==(Node const & other) const
{
	return value == other.value && position == other.position;
}
