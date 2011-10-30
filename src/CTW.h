#ifndef CTW_H_
#define CTW_H_

#include "ctwtypes.h"
#include "Context.h"
#include "TreeNode.h"
#include "xml/pugixml.hpp"
#include <boost/unordered_map.hpp>
#include <boost/circular_buffer.hpp>
#include "ICollector.h"

namespace ctw {

class CTW {

	typedef boost::unordered_map<KEY_TYPE , TreeNode* > umap;
	umap tree;
	Context context;
	const unsigned int NUMBER_OF_CHILDREN;
	const int MAX_NODES_2_XML;

	ICollector* collector;

    void top_nodes(pugi::xml_node&);
    void path_nodes(pugi::xml_node&);
	KEY_TYPE findNode(SIGNAL_TYPE , KEY_TYPE );
	KEY_TYPE createNode(SIGNAL_TYPE , KEY_TYPE );
	double calcChildrenPw(KEY_TYPE , size_t  );
    void node2XML(pugi::xml_node& , KEY_TYPE , KEY_TYPE , unsigned int );

public:

	CTW(ICollector* , int , unsigned int, int = 300);
	CTW(ICollector* , const Context& , unsigned int, int = 300);
	virtual ~CTW();

	void add(SIGNAL_TYPE , size_t , const char* , int);

	void toXML(std::string filename);
};

}

#endif /* CTW_H_ */
