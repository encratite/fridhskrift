#include <string>
#include <fridh/function.hpp>

namespace fridh
{
	struct module
	{
		std::string path;
		symbol_tree_node symbols;
		function entry_function;
	};
}
