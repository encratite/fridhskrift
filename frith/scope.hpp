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
			class_scope
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
		};
	};

	typedef std::map<std::string, scope_entity> scope_entities;

	struct scope
	{
		scope_entities entities;
		scope * parent;
	};
}

#include <frith/class.hpp>
