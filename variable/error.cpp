#include <ail/exception.hpp>
#include <fridh/symbol.hpp>

namespace fridh
{
	void unary_argument_type_error(std::string const & operation, variable_type type)
	{
		throw ail::exception(operation + ": Invalid argument type \"" + get_type_string(type) + "\"");
	}

	void binary_argument_type_error(std::string const & operation, variable_type left, variable_type right)
	{
		throw ail::exception(operation + ": Invalid argument types \"" + get_type_string(left) + "\", \"" + get_type_string(right));
	}
}
