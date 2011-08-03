#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <string>
#include <boost/circular_buffer.hpp>
#include "ctwtypes.h"

namespace ctw {

	class Context {
	public:

		Context(std::size_t );
		Context(const Context& );

		void add(SIGNAL_TYPE);
		size_t size();
		std::string toString() const;

		ContextReverseIterator rbegin();
		ContextReverseIterator rend();

	private:
		boost::circular_buffer_space_optimized<SIGNAL_TYPE> queue;
	};

}

#endif /* CONTEXT_H_ */
