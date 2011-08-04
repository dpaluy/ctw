#ifndef NODE_INFO_H_
#define NODE_INFO_H_

#include "ctwtypes.h"

namespace ctw {

	struct NodeInfo {
		double P;
		KEY_TYPE key;
		size_t depth;

		NodeInfo()
			:P(0)
		{}
	};
}

#endif /* NODE_INFO_H_ */
