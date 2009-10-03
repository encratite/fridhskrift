#include <frith/symbol_tree_node.hpp>
#include <boost/foreach.hpp>

namespace frith
{
	symbol_tree_node::symbol_tree_node():
		parent(0)
	{
	}

	symbol_tree_node::symbol_tree_node(symbol::type type):
		type(type),
		parent(0)
	{
		switch(type)
		{
			case variable::variable:
				variable_pointer = new variable;
				break;

			case variable::function:
				function_pointer = new function;
				break;

			case variable::class_symbol:
				class_pointer = new class_type;
				break;
		}
	}

	symbol_tree_node::~symbol_tree_node()
	{
		BOOST_FOREACH(symbol_tree_node * i, children)
			delete i;

		switch(type)
		{
			case variable::variable:
				delete variable_pointer;
				break;

			case variable::function:
				delete function_pointer;
				break;

			case variable::class_symbol:
				delete class_pointer;
				break;

			case variable::module:
				delete module_pointer;
				break;
		}
	}

	bool symbol_tree_node::exists(std::string const & name)
	{
		node_children::iterator iterator = children.find(name);
		return iterator != children.end();
	}

	bool symbol_tree_node::find_entity(std::string const & name, symbol_tree_node * & output)
	{
		node_children::iterator iterator = children.find(name);
		if(iterator == children.end())
		{
			if(parent == 0)
				return false;
			return parent->find_entity(name, output);
		}

		output = &iterator->second;
		return true;
	}
}
