#include <fridh/symbol.hpp>

namespace fridh
{
	symbol_tree_node::symbol_tree_node():
		type(symbol::uninitialised),
		parent(0)
	{
	}

	symbol_tree_node::symbol_tree_node(symbol_tree_node const & other)
	{
		copy(other);
	}

	symbol_tree_node::symbol_tree_node(symbol::type type):
		type(type),
		parent(0)
	{
		switch(type)
		{
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
		destroy();
	}

	symbol_tree_node & symbol_tree_node::operator=(symbol_tree_node const & other)
	{
		destroy();
		copy(other);
		return *this;
	}

	void symbol_tree_node::copy(symbol_tree_node const & other)
	{
		type = other.type;
		parent = other.parent;

		for(node_children::const_iterator i = other.children.begin(), end = other.children.end(); i != end; i++)
		{
			symbol_tree_node * & node_pointer = children[i->first];
			node_pointer = new symbol_tree_node(*i->second);
			node_pointer->parent = this;
		}

#define COPY_MEMBER(type, member_type, member) \
		case symbol::type: \
			member = new member_type(*other.member); \
			break;

		switch(type)
		{
			COPY_MEMBER(function, function, function_pointer);
			COPY_MEMBER(class_symbol, class_type, class_pointer);
			COPY_MEMBER(module, module, module_pointer);
		}

#undef COPY_MEMBER

	}

	void symbol_tree_node::destroy()
	{
		for(node_children::iterator i = children.begin(), end = children.end(); i != end; i++)
			delete i->second;

#define DELETE_MEMBER(type, member) \
			case symbol::type: \
				delete member; \
				break;

		switch(type)
		{
			DELETE_MEMBER(function, function_pointer);
			DELETE_MEMBER(class_symbol, class_pointer);
			DELETE_MEMBER(module, module_pointer);
		}

		type = symbol::uninitialised;

#undef DELETE_MEMBER

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
