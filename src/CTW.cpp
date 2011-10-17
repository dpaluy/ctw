#include "CTW.h"
#include "CtwException.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/circular_buffer.hpp> // for XML output
#include <stack>
#include <string>
#include <fstream> // File load/save

using namespace ctw;

#define ROOT_INDEX "0"
#define ROOT_PARENT ROOT_INDEX
#define NULL_KEY ""
#define KEY(parent, child) ((parent) + "321"+ boost::lexical_cast<std::string>(child))	// TODO: review pad

CTW::CTW(int tree_depth, unsigned int number_of_children, int max_nodes_2_xml)
	:context(tree_depth),
	 NUMBER_OF_CHILDREN(number_of_children),
	 MAX_NODES_2_XML(max_nodes_2_xml),
	 collectProbability(false),
	 collector(2*tree_depth)
{
	TreeNode* root = new TreeNode(0, ROOT_PARENT);
	SIGNAL_TYPE signal = 0;
	context.add(signal);
	tree[ROOT_INDEX] = root;
}

CTW::CTW(const Context& initialContext, unsigned int number_of_children, int max_nodes_2_xml)
	:context(initialContext),
	 NUMBER_OF_CHILDREN(number_of_children),
	 MAX_NODES_2_XML(max_nodes_2_xml)
{
	TreeNode* root = new TreeNode(0, ROOT_PARENT);
	tree[ROOT_INDEX] = root;
}

CTW::~CTW()
{
	BOOST_FOREACH(umap::value_type node, tree)
	{
		delete node.second;
	}
}

// signal - context
// action - DO Nothing, BUY, SELL (see also COUNTER_ENUM)
void CTW::add(SIGNAL_TYPE signal, size_t action)
{
	if (signal >= NUMBER_OF_CHILDREN)
	{
		BOOST_THROW_EXCEPTION(CtwException("Illegal signal: " + boost::lexical_cast<std::string>(signal)));
	}

	size_t depth = 0;
	NodeInfo* maxProbabilityNode = 0;

	if (collectProbability)
		maxProbabilityNode = new NodeInfo();

	std::stack<KEY_TYPE> path;
	KEY_TYPE parent = ROOT_INDEX;
	path.push(parent);

	bool unique = false;
	for (ContextReverseIterator cii = context.rbegin(); cii != context.rend(); ++cii)
	{
		++depth;
		SIGNAL_TYPE prevSignal = *cii;
		KEY_TYPE index = (unique)? NULL_KEY: findNode(prevSignal, parent);
		if (NULL_KEY == index) //  unique path
		{
			index = createNode(prevSignal, parent);
			unique = true;
			tree[index]->updateUnique(action);
		}
		else
		{
			path.push(index);
			parent = index;
			if ( maxProbabilityNode != 0 && tree[index]->getMaxP() > maxProbabilityNode->P )
			{
				maxProbabilityNode->P = tree[index]->getMaxP();
				maxProbabilityNode->depth = depth;
				maxProbabilityNode->key = index;
			}

		}
	}

	if ( maxProbabilityNode != 0)
	{
		collector.push_back(maxProbabilityNode);
	}

	context.add(signal);

	// Update Pw
	while (!path.empty())
	{
		KEY_TYPE index = path.top();
		path.pop();

		double Pw = 0;//calcChildrenPw(index, action);
		tree[index]->update(action, Pw);
	}
}

void CTW::dumpCollector()
{
	std::ofstream myfile;
	myfile.open ("collector.csv");
	myfile << "NA,BUY,SELL,Depth\n";
	BOOST_FOREACH( NodeInfo* node, collector)
	{
		TreeNode *treeNode = tree[node->key];
		// depth,
		myfile << treeNode->getRealP(NO_ACTION) << ","
				<< treeNode->getRealP(BUY) << "," << treeNode->getRealP(SELL) << "," << node->depth << std::endl;

	}
	myfile.close();
}

double CTW::calcChildrenPw(KEY_TYPE parent, size_t action)
{
	double result = 1;
	for (unsigned int child = 0; child < NUMBER_OF_CHILDREN; child++)
	{
		KEY_TYPE index = findNode(child, parent);
		if (0 != tree[index]) // node exists
		{
			result *= tree[index]->getP(action);
		}
	}
	return result;
}

KEY_TYPE CTW::createNode(SIGNAL_TYPE child, KEY_TYPE parent)
{
	KEY_TYPE key = KEY(parent, child);
	TreeNode* node = new TreeNode(child, parent);
	tree[key] = node;
	return key;
}

KEY_TYPE CTW::findNode(SIGNAL_TYPE child, KEY_TYPE parent)
{
	KEY_TYPE key = KEY(parent, child);
	umap::const_iterator node = tree.find(key);
	if (node == tree.end())
		return NULL_KEY; // NOT FOUND
	else
		return node->first;
}

void CTW::top_nodes(pugi::xml_node & graph_node)
{
    boost::circular_buffer<KEY_TYPE> nodeQueue(MAX_NODES_2_XML + 1);
    nodeQueue.push_back(ROOT_INDEX);
    int counter = 0;
    while (!nodeQueue.empty() && (counter++ < MAX_NODES_2_XML))
	{
		KEY_TYPE parent = nodeQueue[0];
		nodeQueue.pop_front();

		for (unsigned int child = 0; child < NUMBER_OF_CHILDREN; child++)
		{
			KEY_TYPE index = findNode(child, parent);
			if (0 != tree[index]) // node exists
			{
				node2XML(graph_node, parent, index, child);
				nodeQueue.push_back(index);
			}
		}
	}
}

// TODO: remove
void CTW::path_nodes(pugi::xml_node& graph_node)
{
    SIGNAL_TYPE arr[] = { 0, 0, 1, 1, 2,
    					  2, 2, 3, 3, 3,
    					  3, 3, 2, 2, 2,
    					  1, 1, 0, 0, 0
    };

    KEY_TYPE parent = ROOT_INDEX;
    BOOST_FOREACH(SIGNAL_TYPE child, arr)
    {
    	KEY_TYPE index = findNode(child, parent);
    	if (0 != tree[index]) // node exists
    	{
    		node2XML(graph_node, parent, index, child);
    		parent = index;
    	}
    }
}

void CTW::toXML(std::string filename)
{
	pugi::xml_document doc;
	std::string header = "<dotml:graph file-name=\"" + filename + "\" xmlns:dotml=\"http://www.martin-loetzsch.de/DOTML\" margin=\"0.2,0.1\" ranksep=\"0.2\" nodesep=\"0.5\" bgcolor=\"#F0F0FF\"></dotml:graph>";
	doc.load(header.c_str());

	pugi::xml_node graph_node = doc.root().first_child();

	std::string root_content = "dotml:node id='" + boost::lexical_cast<std::string>(ROOT_INDEX) + "' label='" + tree[ROOT_INDEX]->toString() + "' shape='Mdiamond' style=\"filled\" color=\"#CCCCCC\"";
	graph_node.append_child(root_content.c_str());
    top_nodes(graph_node);
    std::string xml_name = filename + ".xml";
	doc.save_file(xml_name.c_str());
}

void CTW::node2XML(pugi::xml_node& node, KEY_TYPE parent, KEY_TYPE index, unsigned int child)
{
	std::string indexStr = boost::lexical_cast<std::string>(index);
	std::string node_content = "dotml:node id=\"" + indexStr + "\" label=\"" + tree[index]->toString() + "\" URL=\"node.html\" style=\"filled\" color=\"#CCCCCC\"";
	node.append_child(node_content.c_str());
	std::string edge_content = "dotml:edge from=\"" + boost::lexical_cast<std::string>(parent) + "\" to=\"" + indexStr + "\" URL=\"edge.html\" label=\""
					+ boost::lexical_cast<std::string>(child) +"\" fontname=\"Arial\"";
	node.append_child(edge_content.c_str());
}
