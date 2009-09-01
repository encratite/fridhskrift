#include <frith/variable.hpp>

namespace frith
{
#define NUMERIC_COMPARISON(operator) \
		if(is_numeric_type() && argument.other.is_numeric_type()) \
		{ \
			if(is_floating_point_operation(argument)) \
				argument.output.new_boolean(get_floating_point_value() operator argument.other.get_floating_point_value()); \
			else if(type == variable_type_identifier::unsigned_integer && argument.other.type == variable_type_identifier::signed_integer) \
				argument.output.new_boolean(unsigned_integer operator argument.other.unsigned_integer); \
			else \
				argument.output.new_boolean(signed_integer operator argument.other.signed_integer); \
		} \
		else \
		{ \
			argument.error_message = get_binary_argument_type_error(name_of_operation, type, argument.other.type); \
			return false; \
		} \
		return true;

	bool variable::less_than(binary_argument & argument) const
	{
		std::string const name_of_operation = "Less than";
		NUMERIC_COMPARISON(<=)
	}

	bool variable::less_than_or_equal(binary_argument & argument) const
	{
		std::string const name_of_operation = "Less than or equal";
		NUMERIC_COMPARISON(<=)
	}

	bool variable::greater_than(binary_argument & argument) const
	{
		std::string const name_of_operation = "Greater than";
		NUMERIC_COMPARISON(<=)
	}

	bool variable::greater_than_or_equal(binary_argument & argument) const
	{
		std::string const name_of_operation = "Greater than or equal";
		NUMERIC_COMPARISON(<=)
	}

	bool variable::unequal(binary_argument & argument) const
	{
		argument.output.new_boolean(operator!=(argument.other));
		return true;
	}

	bool variable::equal(binary_argument & argument) const
	{
		argument.output.new_boolean(operator==(argument.other));
		return true;
	}

	bool variable::logical_not(unary_argument & argument) const
	{
		bool value;
		if(get_boolean_value(value))
		{
			argument.output.new_boolean(!value);
			return true;
		}
		else
		{
			argument.error_message = get_unary_argument_type_error("Logical not", type);
			return false;
		}
	}
}
