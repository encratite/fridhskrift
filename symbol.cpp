#include <frith/symbol_tree_node.hpp>

namespace frith
{
	symbol_tree_node::symbol_tree_node():
		parent(0)
	{
	}

	bool symbol_tree_node::exists(std::string const & name)
	{
		scope_entities::iterator iterator = entities.find(name);
		return iterator != entities.end();
	}

	bool symbol_tree_node::find_entity(std::string const & name, symbol_tree_node * & entity_scope_output, symbol_tree_entity * & entity_output)
	{
		scope_entities::iterator iterator = entities.find(name);
		if(iterator == entities.end())
		{
			if(parent == 0)
				return false;
			return parent->find_entity(name, entitiy_scope_output, entity_output);
		}

		entity_scope_output = this;
		entity_output = &iterator->second;
		return true;
	}
}
