#include <string>
#include <frith/function.hpp>

namespace frith
{
	struct module
	{
		std::string path;
		symbol_tree_node symbols;
		function entry_function;
	};
}
