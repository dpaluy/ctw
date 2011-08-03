#include "Context.h"
#include <sstream> // For toString
#include <boost/foreach.hpp>

using namespace ctw;

Context::Context(std::size_t max_depth)
	:queue(max_depth)
{}

Context::Context(const Context& other)
	:queue(other.queue)
{}

void Context::add(SIGNAL_TYPE symbol)
{
	queue.push_back(symbol);
}

std::size_t Context::size()
{
	return queue.size();
}

std::string Context::toString() const
{
	std::ostringstream oss;
	BOOST_FOREACH( SIGNAL_TYPE ch, queue)
	{
		oss << (int)ch;
	}
	return oss.str();
}


ContextReverseIterator Context::rbegin()
{
	return queue.rbegin();
}

ContextReverseIterator Context::rend()
{
	return queue.rend();
}
