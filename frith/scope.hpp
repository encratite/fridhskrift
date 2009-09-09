#pragma once

#include <map>
#include <string>
#include <frith/variable.hpp>
#include <frith/function.hpp>

namespace frith
{
	namespace scope_entry
	{
		enum type
		{
			variable,
			function,
			class_scope,
			module
		};
	}

	struct scope;
	struct class_type;

	struct scope_entity
	{
		scope_entry::type type;
		union
		{
			variable * variable_pointer;
			function * function_pointer;
			class_type * class_pointer;
			module * module_pointer;
		};
	};

	typedef std::map<std::string, scope_entity> scope_entities;

	struct scope
	{
		scope_entities entities;
		scope * parent;

		scope();

		bool find_entity(std::string const & name, scope * & entity_scope_output, scope_entity * & entity_output) const;
	};
}

#include <frith/class.hpp>
#include <frith/module.hpp>
