#include <iostream>
#include <map>
#include <fridh/symbol.hpp>
#include <ail/string.hpp>

namespace fridh
{
	parse_tree_symbol::parse_tree_symbol():
		type(symbol_prefix::none)
	{
		std::cout << "parse_tree_symbol " << (void *)this << std::endl;
	}

	parse_tree_symbol::parse_tree_symbol(parse_tree_symbol const & other):
		name(other.name),
		type(other.type)
	{
		std::cout << "parse_tree_symbol " << (void *)this << " from " << (void *)&other << std::endl;
	}

	parse_tree_symbol::~parse_tree_symbol()
	{
		std::cout << "~parse_tree_symbol " << (void *)this << std::endl;
	}
}
