#include <iostream>
#include <map>
#include <fridh/symbol.hpp>
#include <ail/string.hpp>

namespace fridh
{
	parse_tree_node::parse_tree_node():
		type(parse_tree_node_type::uninitialised)
	{
	}

	parse_tree_node::parse_tree_node(parse_tree_node const & other)
	{

		//std::cout << "Copying " << other.to_string() << std::endl;

#define COPY_MEMBER(type, member_type, member) \
		case parse_tree_node_type::type: \
			member = new member_type(*other.member); \
			break;

		type = other.type;

		switch(other.type)
		{
			COPY_MEMBER(variable, variable, variable_pointer)
			COPY_MEMBER(symbol, parse_tree_symbol, symbol_pointer)
			COPY_MEMBER(unary_operator_node, parse_tree_unary_operator_node, unary_operator_pointer)
			COPY_MEMBER(binary_operator_node, parse_tree_binary_operator_node, binary_operator_pointer)
			COPY_MEMBER(call, parse_tree_call, call_pointer)
			COPY_MEMBER(array, parse_tree_array, array_pointer)
		}

#undef COPY_MEMBER

		//std::cout << "Result: " << to_string() << std::endl;

	}

	parse_tree_node::~parse_tree_node()
	{

#define DELETE_MEMBER(type, member) \
			case parse_tree_node_type::type: \
				delete member; \
				break;

		switch(type)
		{
			DELETE_MEMBER(variable, variable_pointer)
			DELETE_MEMBER(symbol, symbol_pointer)
			DELETE_MEMBER(unary_operator_node, unary_operator_pointer)
			DELETE_MEMBER(binary_operator_node, binary_operator_pointer)
			DELETE_MEMBER(call, call_pointer)
			DELETE_MEMBER(array, array_pointer)
		}

#undef DELETE_MEMBER

	}

	parse_tree_node::parse_tree_node(parse_tree_node_type::type type):
		type(type)
	{
	}

	parse_tree_node::parse_tree_node(variable * variable_pointer):
		type(parse_tree_node_type::variable),
		variable_pointer(variable_pointer)
	{
	}

	parse_tree_node::parse_tree_node(unary_operator_type::type unary_operator):
		type(parse_tree_node_type::unary_operator_node)
	{
		unary_operator_pointer = new parse_tree_unary_operator_node;
		unary_operator_pointer->type = unary_operator;
	}

	parse_tree_node::parse_tree_node(binary_operator_type::type binary_operator):
		type(parse_tree_node_type::binary_operator_node)
	{
		binary_operator_pointer = new parse_tree_binary_operator_node;
		binary_operator_pointer->type = binary_operator;
	}

	parse_tree_node::parse_tree_node(parse_tree_nodes & elements):
		type(parse_tree_node_type::array)
	{
		array_pointer = new parse_tree_array;
		array_pointer->elements = elements;
	}

	void parse_tree_node::is_call()
	{
		type = parse_tree_node_type::call;
		call_pointer = new parse_tree_call;
	}

	bool parse_tree_node::is_post_fix() const
	{
		if(type != parse_tree_node_type::unary_operator_node)
			throw ail::exception("Tried to retrieve post fix node properties of a non-post-fix node");

		unary_operator_type::type unary_type = unary_operator_pointer->type;
		return
			unary_type == unary_operator_type::increment ||
			unary_type == unary_operator_type::decrement
		;
	}

	std::string parse_tree_node::to_string() const
	{
		switch(type)
		{
			case parse_tree_node_type::uninitialised:
				return "uninitialised";

			case parse_tree_node_type::variable:
				return "variable";

			case parse_tree_node_type::symbol:
				return "symbol: " + symbol_pointer->name;

			case parse_tree_node_type::unary_operator_node:
				return "unary operator";

			case parse_tree_node_type::binary_operator_node:
				return "binary operator";

			case parse_tree_node_type::call:
				return "call";

			case parse_tree_node_type::array:
				return "array";

			case parse_tree_node_type::call_operator:
				return "call operator";

			case parse_tree_node_type::spaced_call_operator:
				return "spaced call operator";

			case parse_tree_node_type::iterator:
				return "iterator";

			default:
				return "unknown (" + ail::number_to_string(static_cast<int>(type)) + ")";
		}
	}

	parse_tree_symbol::parse_tree_symbol():
		type(symbol_prefix::none)
	{
	}
}
