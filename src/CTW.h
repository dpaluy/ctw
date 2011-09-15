#ifndef CTW_H_
#define CTW_H_

#include "ctwtypes.h"
#include "Context.h"
#include "TreeNode.h"
#include "xml/pugixml.hpp"
#include <boost/unordered_map.hpp>
#include <boost/circular_buffer.hpp>
#include "NodeInfo.h"

namespace ctw {

class CTW {

	typedef boost::unordered_map<KEY_TYPE , TreeNode* > umap;
	umap tree;
	Context context;

	bool collectProbability;
	boost::circular_buffer_space_optimized<NodeInfo*> collector;

    void top_nodes(pugi::xml_node&);
    void path_nodes(pugi::xml_node&);
	KEY_TYPE findNode(SIGNAL_TYPE , KEY_TYPE );
	KEY_TYPE createNode(SIGNAL_TYPE , KEY_TYPE );
	double calcChildrenPw(KEY_TYPE , size_t  );
    void node2XML(pugi::xml_node& , KEY_TYPE , KEY_TYPE , unsigned int );

public:

	CTW();
	CTW(const Context& );
	virtual ~CTW();

	void add(SIGNAL_TYPE , size_t );

	void toXML(std::string filename);

	void collectP() { collectProbability = true; }
	void dumpCollector();
};

}

#endif /* CTW_H_ */
