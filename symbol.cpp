#include <fridh/symbol.hpp>

namespace fridh
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
			case symbol::variable:
				variable_pointer = new variable;
				break;

			case symbol::function:
				function_pointer = new function;
				break;

			case symbol::class_symbol:
				class_pointer = new class_type;
				break;
		}
	}

	symbol_tree_node::~symbol_tree_node()
	{
		for(node_children::iterator i = children.begin(), end = children.end(); i != end; i++)
			delete i->second;

		switch(type)
		{
			case symbol::variable:
				delete variable_pointer;
				break;

			case symbol::function:
				delete function_pointer;
				break;

			case symbol::class_symbol:
				delete class_pointer;
				break;

			case symbol::module:
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

		output = iterator->second;
		return true;
	}
}
