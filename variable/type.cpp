#include <frith/symbol.hpp>

namespace frith
{
	variable_type variable::get_type() const
	{
		return type;
	}

	bool variable::is_floating_point_operation(binary_argument & argument) const
	{
		return type == variable_type_identifier::floating_point_value || argument.other.type == variable_type_identifier::floating_point_value;
	}

	bool variable::is_integer_type() const
	{
		return
			type == variable_type_identifier::signed_integer ||
			type == variable_type_identifier::unsigned_integer;
	}

	bool variable::is_numeric_type() const
	{
		return
			type == variable_type_identifier::signed_integer ||
			type == variable_type_identifier::unsigned_integer ||
			type == variable_type_identifier::floating_point_value;
	}
}

