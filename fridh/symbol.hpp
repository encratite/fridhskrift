#pragma once

#include <map>
#include <string>

namespace fridh
{
	namespace symbol
	{
		enum type
		{
			variable,
			function,
			class_symbol,
			module
		};
	}

	struct symbol_tree_node;
	struct function;
	struct class_type;
	struct module;

	class variable;

	typedef std::map<std::string, symbol_tree_node *> node_children;

	struct symbol_tree_node
	{
		symbol::type type;
		union
		{
			variable * variable_pointer;
			function * function_pointer;
			class_type * class_pointer;
			module * module_pointer;
		};

		node_children children;
		symbol_tree_node * parent;

		symbol_tree_node();
		symbol_tree_node(symbol::type type);
		~symbol_tree_node();

		bool exists(std::string const & name);
		bool find_entity(std::string const & name, symbol_tree_node * & output);
	};
}

#include <fridh/variable.hpp>
#include <fridh/class.hpp>
#include <fridh/function.hpp>
#include <fridh/module.hpp>
