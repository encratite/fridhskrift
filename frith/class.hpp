#include <vector>
#include <frith/function.hpp>

namespace frith
{
	struct class_type
	{
		std::string name;
		symbol_tree_node * class_scope;
		//std::vector<function> functions;
	};
}
