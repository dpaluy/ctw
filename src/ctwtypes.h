#ifndef CTWTYPES_H_
#define CTWTYPES_H_

#include <string>
#define KEY_TYPE std::string

typedef unsigned int SIGNAL_TYPE;

#define ContextReverseIterator boost::circular_buffer_space_optimized<SIGNAL_TYPE>::const_reverse_iterator

enum COUNTER_ENUM { NO_ACTION = 0, BUY, SELL, MAX_COUNTER_ENUM};

#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#endif /* CTWTYPES_H_ */
