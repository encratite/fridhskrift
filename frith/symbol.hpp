#pragma once

#include <map>
#include <string>
#include <frith/variable.hpp>
#include <frith/function.hpp>

namespace frith
{
	namespace symbol
	{
		enum type
		{
			variable,
			function,
			class_scope,
			module
		};
	}

	struct symbol_tree_node;
	struct class_type;

	struct symbol_tree_entity
	{
		symbol::type type;
		union
		{
			variable * variable_pointer;
			function * function_pointer;
			class_type * class_pointer;
			module * module_pointer;
		};
	};

	typedef std::map<std::string, symbol_tree_entity> scope_entities;

	struct symbol_tree_node
	{
		scope_entities entities;
		symbol_tree_node * parent;

		symbol_tree_node();

		bool find_entity(std::string const & name, symbol_tree_node * & entity_scope_output, symbol_tree_entity * & entity_output);
	};
}

#include <frith/class.hpp>
#include <frith/module.hpp>
