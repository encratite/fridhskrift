#include <string>

namespace frith
{
	struct module
	{
		std::string path;
		symbol_tree_node module_scope;
	};
}
