#ifndef ICOLLECTOR_H_
#define ICOLLECTOR_H_

#include "ctwtypes.h"

namespace ctw {

class ICollector {
public:
	virtual ~ICollector() {}
	virtual bool add(int ctw_id, const char* datetime, const double* P, size_t depth) = 0;
};

} /* namespace ctw */
#endif /* ICOLLECTOR_H_ */
