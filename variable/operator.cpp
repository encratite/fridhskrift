#include <fridh/symbol.hpp>

namespace fridh
{
	bool variable::array_addition(variable const & argument, variable & output) const
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

			return true;
		}
		else
			return false;
	}

	bool variable::string_addition(variable const & argument, variable & output) const
	{
		bool left_is_string = type == variable_type_identifier::string;
		bool right_is_string = argument.other.type == variable_type_identifier::string;

		if(left_is_string || right_is_string)
		{
			output.type = variable_type_identifier::string;
			output.string = new std::string;
			*output.string = get_string_representation() + argument.get_string_representation();
			return true;
		}
		else
			return false;
	}

#define ARITHMETIC_OPERATION(description, operator) \
		if(is_numeric_type() && argument.is_numeric_type()) \
		{ \
			if(is_floating_point_operation(argument)) \
				output.new_floating_point_value(get_floating_point_value() operator argument.get_floating_point_value()); \
			else if(type == variable_type_identifier::unsigned_integer && argument.type == variable_type_identifier::unsigned_integer) \
				output.new_unsigned_integer(unsigned_integer operator argument.unsigned_integer); \
			else \
				output.new_signed_integer(signed_integer operator argument.signed_integer); \
		} \
		else \
			binary_argument_type_error(description, type, argument.type);

	void variable::addition(variable const & argument, variable & output) const
	{
		if(array_addition(argument, output))
			return;

		if(string_addition(argument, output))
			return;

		ARITHMETIC_OPERATION("Addition", +)
	}

	void variable::subtraction(variable const & argument, variable & output) const
	{
		ARITHMETIC_OPERATION("Subtraction", -)
	}

	void variable::multiplication(variable const & argument, variable & output) const
	{
		ARITHMETIC_OPERATION("Multiplication", *)
	}

	void variable::addition(variable const & argument, variable & output) const
	{
		if(argument.is_zero())
			throw ail::exception(zero_division_error_message);
		ARITHMETIC_OPERATION("Division", /)
	}

	void variable::addition(variable const & argument, variable & output) const
	{
		if(argument.is_zero())
			throw ail::exception(zero_division_error_message);
		else if(is_integer_type() && argument.is_integer_type())
		{
			if(type == variable_type_identifier::unsigned_integer && argument.type == variable_type_identifier::unsigned_integer)
				output.new_unsigned_integer(unsigned_integer % argument.unsigned_integer);
			else
				output.new_signed_integer(signed_integer % argument.signed_integer);
		}
		else
			binary_argument_type_error("Modulo", type, argument.type);
	}

	void variable::negation(variable & output) const
	{
		output.type = type;
		switch(type)
		{
		case variable_type_identifier::signed_integer:
			output.signed_integer = - signed_integer;
			break;

		case variable_type_identifier::unsigned_integer:
			output.type = variable_type_identifier::signed_integer;
			output.signed_integer = - static_cast<types::signed_integer>(unsigned_integer);
			break;

		case variable_type_identifier::floating_point_value:
			output.floating_point_value = - floating_point_value;
			break;

		default:
			throw ail::exception("Cannot use unary minus on type " + get_type_string(type));
		}
	}
}
