#pragma once

#include <vector>
#include <string>

#include <ail/types.hpp>

namespace frith
{
	namespace executable_unit_type
	{
		enum type
		{
			statement,
			assignment,
			return_statement,
			if_statement,
			if_else_statement,
			for_each_statement,
			for_statement,
			while_statement,
		};
	}

	namespace assignment_type
	{
		enum type
		{
			assignment,
			addition,
			subtraction,
			multiplication,
			division,
			modulo,
			exponentiation,

			increment,
			decrement,
		};
	}

	namespace unary_operator_type
	{
		enum type
		{
			negation,
			logical_not,
			binary_not,
		};
	}

	namespace binary_operator_type
	{
		enum type
		{
			addition,
			subtraction,
			multiplication,
			division,
			modulo,
			exponentiation,

			less_than,
			less_than_or_equal,
			greater_than,
			greater_than_or_equal,
			unequal,
			equal,

			logical_and,
			logical_or,

			shift_left,
			shift_right,

			binary_and,
			binary_or,
			binary_xor,
		};
	}

	namespace parse_tree_node_type
	{
		enum type
		{
			symbol,
			unary_operator_node,
			binary_operator_node,
			call,
			array,
		};
	}

	struct
		parse_tree_node,
		parse_tree_symbol,
		executable_units;

	typedef std::vector<parse_tree_node> parse_tree_nodes;
	typedef std::vector<parse_tree_symbol> parse_tree_symbols;
	typedef std::vector<executable_unit> executable_units;
	typedef std::vector<binary_operator_type::type> binary_operator_container;

	struct parse_tree_symbol
	{
		std::string name;
	};

	struct parse_tree_unary_operator_node
	{
		unary_operator_type type;
		parse_tree_node argument;
	};

	struct parse_tree_binary_operator_node
	{
		binary_operator_type type;
		parse_tree_node
			left_argument,
			right_argument;
	};

	struct parse_tree_call
	{
		parse_tree_node function;
		parse_tree_nodes arguments;
	};

	struct parse_tree_array
	{
		parse_tree_node elements;
	};

	struct parse_tree_node
	{
		parse_tree_node_type::type type;
		union
		{
			parse_tree_symbol * symbol_pointer;
			parse_tree_unary_operator_node * unary_operator_pointer;
			parse_tree_binary_operator_node * binary_operator_pointer;
			parse_tree_call * call_pointer;
			parse_tree_array * array_pointer;
		}
	};

	struct assignment_statement
	{
		assignment_type::type type;
		parse_tree_symbols symbols;
		parse_tree_node term;
	};

	struct if_statement
	{
		parse_tree_node conditional_term;
		parse_tree_nodes body;
	};

	struct if_else_statement
	{
		parse_tree_node conditional_term;
		parse_tree_nodes
			if_body,
			else_body;
	};

	struct for_each_statement
	{
		parse_tree_symbol container;
		parse_tree_nodes body;
	};

	struct for_statement
	{
		parse_tree_node
			initialisation,
			conditional,
			iteration;
		parse_tree_nodes body;
	};

	struct while_statement
	{
		parse_tree_node conditional_term;
		parse_tree_nodes body;
	};

	struct executable_unit
	{
		executable_unit_type::type type;

		union
		{
			parse_tree_node * statement_pointer;
			if_statement * if_pointer;
			if_else_statement * if_else_pointer;
			for_each_statement * for_each_pointer;
			for_statement * for_pointer;
			while_statement * while_pointer;
		}
	};

	struct function
	{
		string_vector arguments;
		executable_units units;
	};
}
