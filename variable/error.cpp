#include <frith/variable.hpp>

namespace frith
{
	std::string get_unary_argument_type_error(std::string const & operation, variable_type type)
	{
		return operation + ": Invalid argument type \"" + get_type_string(type) + "\"";
	}

	std::string get_binary_argument_type_error(std::string const & operation, variable_type left, variable_type right)
	{
		return operation + ": Invalid argument types \"" + get_type_string(left) + "\", \"" + get_type_string(right);
	}
}
