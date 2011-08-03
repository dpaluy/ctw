#include "TreeNode.h"
#include <cstring>
#include <boost/lexical_cast.hpp> // For ToString

#ifdef DEBUG
#define ASSERT_CMP(value, MAX_VALUE, msg)	{												/
if (value >= MAX_VALUE) /
throw IndexOutOfRangeException(msg); /
}
#else
#define ASSERT_CMP(value, MAX_VALUE, msg)
#endif

using namespace ctw;

TreeNode::TreeNode(SIGNAL_TYPE child, KEY_TYPE parent) :
	child_index(child), parent_index(parent)
{
	cnt[0] = cnt[1] = cnt[2] = 1;
	P[0] = P[1] = P[2] = 0;
}

/*
TreeNode::TreeNode(const TreeNode& other) :
	child_index(other.child_index), parent_index(other.parent_index)
{
	memcpy(cnt, other.cnt, MAX_COUNTER_ENUM * sizeof cnt[0]);
	memcpy(P, other.P, MAX_COUNTER_ENUM * sizeof P[0]);
}
*/

bool TreeNode::operator==(const TreeNode & other) const
{
	return (child_index == other.child_index) && (parent_index
			== other.parent_index);
}

void TreeNode::updateUnique(size_t action_index)
{
	ASSERT_CMP(action_index, MAX_COUNTER_ENUM, "Wrong Action Index!");
	P[action_index] = 1 / (double)MAX_COUNTER_ENUM;
	++cnt[action_index];
}

void TreeNode::update(size_t action_index, double Pchildren)
{
	double Pe = calcPe(action_index);
//	double Pw  = Pe/2 + Pchildren/2;
	P[action_index] = Pe;
	++cnt[action_index];
}

double TreeNode::getP(size_t action_index)
{
	double result = (0 == P[action_index]) ? 1 : P[action_index];
	return result;
}

double TreeNode::getMaxP()
{
	double result = MAX(P[NO_ACTION], P[BUY]);
	result = MAX(result, P[SELL]);
	return result;
}

std::string TreeNode::toString()
{
	std::string str = "";
	for (int i = 0; i < MAX_COUNTER_ENUM; i++) {
		switch (i) {
		case BUY:
			str += " BUY ";
			break;
		case SELL:
			str += " SELL ";
			break;
		default:
			str += "NA ";
			break;
		}
		str += "[";
		std::string p = boost::lexical_cast<std::string>(P[i]);
		str += p;
		str += "]";

		std::string tmp = boost::lexical_cast<std::string>(cnt[i]);
		str += tmp;
		str += ", ";
	}

	return str;
}

// PRIVATE

double TreeNode::calcPe(size_t action_index) {
	double a = (double)cnt[action_index];
	double b = (double)cntSum();
	double result =  a / b;
	return result;
}

unsigned int TreeNode::cntSum() {
	unsigned int result = 0;
	for (int i = 0; i < MAX_COUNTER_ENUM; i++)
		result += cnt[i];
	return result;
}
