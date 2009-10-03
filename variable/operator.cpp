#include <frith/symbol.hpp>

namespace frith
{
	bool variable::array_addition(binary_argument & argument) const
	{
		bool left_is_array = type == variable_type_identifier::array;
		bool right_is_array = argument.other.type == variable_type_identifier::array;

		if(left_is_array || right_is_array)
		{
			argument.output.type = variable_type_identifier::array;
			argument.output.array = new types::vector;
			types::vector & vector = *argument.output.array;

			if(left_is_array && right_is_array)
			{
				vector = *array;
				types::vector & right_vector = *argument.other.array;
				vector.insert(vector.end(), right_vector.begin(), right_vector.end());
			}
			else if(left_is_array && !right_is_array)
			{
				vector = *array;
				vector.push_back(argument.other);
			}
			else if(!left_is_array && right_is_array)
			{
				vector = *argument.other.array;
				vector.push_back(*this);
			}
		}
		else
			return false;

		return true;
	}

	bool variable::perform_string_conversion(std::string & output, bool & error) const
	{
		if(!get_string_representation(output))
		{
			error = true;
			return false;
		}
		else
			return true;
	}

	bool variable::string_addition(binary_argument & argument, bool & error) const
	{
		bool left_is_string = type == variable_type_identifier::string;
		bool right_is_string = argument.other.type == variable_type_identifier::string;

		error = false;

		if(left_is_string || right_is_string)
		{
			argument.output.type = variable_type_identifier::string;
			argument.output.string = new std::string;
			std::string & output_string = *argument.output.string;
			if(left_is_string && right_is_string)
				output_string = *string + *argument.other.string;
			else if(left_is_string && !right_is_string)
				return argument.other.perform_string_conversion(output_string, error);
			else if(!left_is_string && right_is_string)
				return perform_string_conversion(output_string, error);
		}
		else
			return false;

		return true;
	}

#define ARITHMETIC_OPERATION(operator) \
		if(is_numeric_type() && argument.other.is_numeric_type()) \
		{ \
			if(is_floating_point_operation(argument)) \
				argument.output.new_floating_point_value(get_floating_point_value() operator argument.other.get_floating_point_value()); \
			else if(type == variable_type_identifier::unsigned_integer && argument.other.type == variable_type_identifier::unsigned_integer) \
				argument.output.new_unsigned_integer(unsigned_integer operator argument.other.unsigned_integer); \
			else \
				argument.output.new_signed_integer(signed_integer operator argument.other.signed_integer); \
		} \
		else \
		{ \
			argument.error_message = get_binary_argument_type_error(name_of_operation, type, argument.other.type); \
			return false; \
		} \
		return true;

	bool variable::addition(binary_argument & argument) const
	{
		std::string const name_of_operation = "Addition";

		if(array_addition(argument))
			return true;

		bool string_error;
		if(string_addition(argument, string_error))
			return true;

		if(string_error)
		{
			argument.error_message = get_binary_argument_type_error(name_of_operation, type, argument.other.type);
			return false;
		}

		ARITHMETIC_OPERATION(+)
	}

	bool variable::subtraction(binary_argument & argument) const
	{
		std::string const name_of_operation = "Subtraction";
		ARITHMETIC_OPERATION(-)
	}

	bool variable::multiplication(binary_argument & argument) const
	{
		std::string const name_of_operation = "Multiplication";
		ARITHMETIC_OPERATION(*)
	}

	bool variable::division(binary_argument & argument) const
	{
		std::string const name_of_operation = "Division";
		if(argument.other.is_zero())
		{
			argument.error_message = zero_division_error_message;
			return false;
		}
		ARITHMETIC_OPERATION(/)
	}

	bool variable::modulo(binary_argument & argument) const
	{
		std::string const name_of_operation = "Modulo";
		if(argument.other.is_zero())
		{
			argument.error_message = zero_division_error_message;
			return false;
		}
		else if(is_integer_type() && argument.other.is_integer_type())
		{
			if(type == variable_type_identifier::unsigned_integer && argument.other.type == variable_type_identifier::unsigned_integer)
				argument.output.new_unsigned_integer(unsigned_integer % argument.other.unsigned_integer);
			else
				argument.output.new_signed_integer(signed_integer % argument.other.signed_integer);
		}
		else
		{
			argument.error_message = get_binary_argument_type_error(name_of_operation, type, argument.other.type);
			return false;
		}
		return true;
	}

	bool variable::negative(unary_argument & argument) const
	{
		argument.output.type = type;
		switch(type)
		{
		case variable_type_identifier::signed_integer:
			argument.output.signed_integer = - signed_integer;
			break;

		case variable_type_identifier::unsigned_integer:
			argument.output.type = variable_type_identifier::signed_integer;
			argument.output.signed_integer = - static_cast<types::signed_integer>(unsigned_integer);
			break;

		case variable_type_identifier::floating_point_value:
			argument.output.floating_point_value = - floating_point_value;
			break;

		default:
			argument.error_message = "Cannot use unary minus on type " + get_type_string(type);
			return false;
		}

		return true;
	}
}
