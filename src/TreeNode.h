#ifndef TREENODE_H_
#define TREENODE_H_

#include "ctwtypes.h"

namespace ctw {

class TreeNode {
	double calcPe(size_t action_index);

	unsigned int cntSum();

	SIGNAL_TYPE child_index;
	KEY_TYPE parent_index;

	unsigned int cnt[MAX_COUNTER_ENUM];
	double P[MAX_COUNTER_ENUM];

public:

	TreeNode(SIGNAL_TYPE child, KEY_TYPE parent);
	//TreeNode(const TreeNode& other);

	bool operator==(const TreeNode & other) const;

	void updateUnique(size_t action_index);

	void update(size_t action_index, double Pchildren);

	double getRealP(size_t action_index) { return P[action_index]; }
	double getP(size_t action_index);
	double getMaxP();

	std::string toString();
};

}

#endif /* TREENODE_H_ */
