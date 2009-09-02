#pragma once

#include <map>
#include <string>
#include <frith/variable.hpp>
#include <frith/function.hpp>
#include <frith/class.hpp>

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

	class scope;

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

	class scope
	{
		scope_entities entities;
	};
}
