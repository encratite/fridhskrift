#include <frith/scope.hpp>

namespace frith
{
	scope::scope():
		parent(0)
	{
	}

	bool scope::find_entity(std::string const & name, scope * & entity_scope_output, scope_entity * & entity_output) const
	{
		scope_entities::const_iterator iterator = entities.find(name);
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
