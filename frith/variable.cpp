#include <ail/string.hpp>
#include <frith/variable.hpp>

namespace frith
{
	namespace
	{
		std::string const zero_division_error_message = "Zero division error";
	}

	unary_argument::unary_argument(variable & output, std::string & error_message):
		output(output),
		error_message(error_message)
	{
	}

	binary_argument::binary_argument(variable const & other, variable & output, std::string & error_message)
		other(other),
		output(output),
		error_message(error_message)
	{
	}

	variable::variable():
		type(variable_type::undefined)
	{
	}

	variable::variable(variable const & other):
		type(other.type)
	{
		switch(type)
		{
		case variable_type::boolean:
			boolean = other.boolean;
			break;

		case variable_type::signed_integer:
			signed_integer = other.signed_integer;
			break;

		case variable_type::unsigned_integer:
			unsigned_integer = other.unsigned_integer;
			break;

		case variable_type::floating_point_value:
			floating_point_value = other.floating_point_value;
			break;

		case variable_type::string:
			string = new string(*other.string);
			break;

		case variable_type::array:
			array = new variable_vector(*other.array);
			break;

		case variable_type::map:
			map = new variable_map(*other.map);
			break;
		}
	}

	variable::~variable()
	{
		switch(type)
		{
		case variable_type::string:
			delete string;
			break;

		case variable_type::array:
			delete array;
			break;

		case variable_type::map:
			delete map;
			break;
		}
	}

	variable_type variable::get_type() const
	{
		return type;
	}

	void variable::nil()
	{
		type = variable_type::nil;
	}

	void variable::new_boolean(types::boolean new_boolean)
	{
		type = variable_type::boolean;
		boolean = new_boolean;
	}

	void variable::new_signed_integer(types::signed_integer new_signed_integer)
	{
		type = variable_type::signed_integer;
		signed_integer = new_signed_integer;
	}

	void variable::new_unsigned_integer(types::unsigned_integer new_unsigned_integer)
	{
		type = variable_type::unsigned_integer;
		unsigned_integer = new_unsigned_integer;
	}

	void variable::new_floating_point_value(types::floating_point_value new_floating_point_value)
	{
		type = variable_type::floating_point_value;
		floating_point_value = new_floating_point_value;
	}

	void variable::new_string(types::string const & new_string)
	{
		type = variable_type::string;
		string = new std::string(new_string);
	}

	void variable::new_array()
	{
		type = variable_type::array;
		array = new variable_vector();
	}

	void variable::new_map()
	{
		type = variable_type::map;
		map = new variable_map();
	}

	bool variable::array_addition(binary_argument & argument) const
	{
		bool left_is_array = type == variable_type::array;
		bool right_is_array = argument.other.type == variable_type::array;

		if(left_is_array || right_is_array)
		{
			argument.output.type = variable_type::array;
			argument.output.array = new types::variable_vector;
			types::variable_vector & vector = *argument.output.array;

			if(left_is_array && right_is_array)
			{
				vector = *array;
				vector.append(*argument.other.array);
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

	bool variable::perform_string_conversion(std::string & output, bool & error)
	{
		if(!get_string_representation(output_string))
		{
			error = true;
			return false;
		}
		else
			return true;
	}

	bool variable::string_addition(binary_argument & argument, bool & error) const
	{
		bool left_is_string = type == variable_type::string;
		bool right_is_string = argument.other.type == variable_type::string;

		error = false;

		if(left_is_string || right_is_string)
		{
			argument.output.type = variable_type::string;
			argument.output.string = new std::string;
			std::string & output_string = *argument.output.string;
			if(left_is_string && right_is_string)
				output_string = *string + *argument.other.string;
			else if(left_is_string && !right_is_string)
				return other.argument.perform_string_conversion(output_string, error);
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
			else \
				unsigned_integer = unsigned_integer operator argument.other.unsigned_integer; \
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
		ARITHMETIC_OPERATION(%)
	}

	bool variable::negative(unary_argument & argument) const
	{
	}

	bool variable::less_than(binary_argument & argument) const
	{
	}

	bool variable::less_than_or_equal(binary_argument & argument) const
	{
	}

	bool variable::greater_than(binary_argument & argument) const
	{
	}

	bool variable::greater_than_or_equal(binary_argument & argument) const
	{
	}

	bool variable::unequal(binary_argument & argument) const
	{
	}

	bool variable::equal(binary_argument & argument) const
	{
	}

	bool variable::logical_not(unary_argument & argument) const
	{
	}

	bool variable::logical_and(binary_argument & argument) const
	{
	}

	bool variable::logical_or(binary_argument & argument) const
	{
	}

	bool variable::shift_left(binary_argument & argument) const
	{
	}

	bool variable::shift_right(binary_argument & argument) const
	{
	}

	bool variable::binary_and(binary_argument & argument) const
	{
	}

	bool variable::binary_or(binary_argument & argument) const
	{
	}

	bool variable::binary_xor(binary_argument & argument) const
	{
	}

	bool variable::binary_not(unary_argument & argument) const
	{
	}

	bool variabe::is_floating_point_operation(binary_argument & argument) const
	{
		return type == variable_type::floating_point_value || argument.other.type == variable_type::floating_point_value;
	}

	bool variabe::is_numeric_type(binary_argument & argument) const
	{
		return
			type == variable_type::signed_integer ||
			type == variable_type::unsigned_integer ||
			type == variable_type::floating_point_value;
	}

	types::floating_point_value variable::get_floating_point_value() const
	{
		switch(type)
		{
		case variable_type::signed_integer:
			return signed_integer;

		case variable_type::unsigned_integer:
			return unsigned_integer;

		case variable_type::floating_point_value:
			return floating_point_value;
		}

		throw ail::exception("Failed to retrieve floating point value");
	}

	bool variable::get_string_representation(std::string & output) const
	{
		switch(type)
		{
		case variable_type::boolean:
			output = ail::bool_to_string(boolean);
			break;

		case variable_type::signed_integer:
			output = ail::number_to_string<types::signed_integer>(signed_integer);
			break;

		case variable_type::unsigned_integer:
			output = ail::number_to_string<types::unsigned_integer>(unsigned_integer);
			break;

		case variable_type::floating_point_value:
			output = ail::number_to_string<types::floating_point_value>(floating_point_value);
			break;

		default:
			return false;
		}

		return true;
	}

	bool variable::is_zero() const
	{
		switch(type)
		{
		case variable_type::signed_integer:
			return signed_integer == 0;

		case variable_type::unsigned_integer:
			return unsigned_integer == 0;

		case variable_type::floating_point_value:
			return floating_point_value == 0.0;
		}

		throw exception("Unable to check if variable is zero");		
	}

	std::string get_type_string(variable_type type)
	{
		switch(type)
		{
		case variable_type::undefined:
			return "undefined";

		case variable_type::nil:
			return "nil";

		case variable_type::boolean:
			return "boolean";

		case variable_type::signed_integer:
			return "integer";

		case variable_type::unsigned_integer:
			return "unsigned-integer";

		case variable_type::floating_point_value:
			return "float";

		case variable_type::string:
			return "string";

		case variable_type::array:
			return "array";

		case variable_type::map:
			return "map";
		}

		return "unknown";
	}

	std::string get_binary_argument_type_error(std::string const & operation, variable_type left, variable_type right)
	{
		return operation + ": Invalid argument types \"" + get_type_string(left) + "\", \"" + get_type_string(right);
	}
}
