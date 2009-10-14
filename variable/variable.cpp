#include <ail/string.hpp>
#include <fridh/symbol.hpp>

namespace fridh
{
	variable::variable():
		type(variable_type_identifier::undefined)
	{
	}

	variable::variable(variable const & other):
		type(other.type)
	{
		switch(type)
		{
		case variable_type_identifier::boolean:
			boolean = other.boolean;
			break;

		case variable_type_identifier::signed_integer:
			signed_integer = other.signed_integer;
			break;

		case variable_type_identifier::unsigned_integer:
			unsigned_integer = other.unsigned_integer;
			break;

		case variable_type_identifier::floating_point_value:
			floating_point_value = other.floating_point_value;
			break;

		case variable_type_identifier::string:
			string = new std::string(*other.string);
			break;

		case variable_type_identifier::array:
			array = new types::vector(*other.array);
			break;

		case variable_type_identifier::map:
			map = new types::map(*other.map);
			break;
		}
	}

	variable::~variable()
	{
		switch(type)
		{
		case variable_type_identifier::string:
			delete string;
			break;

		case variable_type_identifier::array:
			delete array;
			break;

		case variable_type_identifier::map:
			delete map;
			break;
		}
	}

	types::floating_point_value variable::get_floating_point_value() const
	{
		switch(type)
		{
		case variable_type_identifier::signed_integer:
			return signed_integer;

		case variable_type_identifier::unsigned_integer:
			return unsigned_integer;

		case variable_type_identifier::floating_point_value:
			return floating_point_value;
		}

		throw ail::exception("Failed to retrieve floating point value");
	}

	std::string variable::get_string_representation() const
	{
		switch(type)
		{
		case variable_type_identifier::boolean:
			return ail::bool_to_string(boolean);

		case variable_type_identifier::signed_integer:
			return ail::number_to_string<types::signed_integer>(signed_integer);

		case variable_type_identifier::unsigned_integer:
			return ail::number_to_string<types::unsigned_integer>(unsigned_integer);

		case variable_type_identifier::floating_point_value:
			return ail::number_to_string<types::floating_point_value>(floating_point_value);

		case variable_type_identifier::string:
			return *string;
		}

		unary_argument_type_error("String representation", type);

		//should never get here, suppress warnings
		return "Error";
	}

	bool variable::get_boolean_value() const
	{
		switch(type)
		{
		case variable_type_identifier::boolean:
			return boolean;

		case variable_type_identifier::signed_integer:
			return signed_integer != 0;

		case variable_type_identifier::unsigned_integer:
			return unsigned_integer != 0;
		}

		unary_argument_type_error("Boolean representation", type);

		//should never get here, suppress warnings
		return false;
	}

	bool variable::is_zero() const
	{
		switch(type)
		{
		case variable_type_identifier::signed_integer:
			return signed_integer == 0;

		case variable_type_identifier::unsigned_integer:
			return unsigned_integer == 0;

		case variable_type_identifier::floating_point_value:
			return floating_point_value == 0.0;
		}

		throw ail::exception("Unable to check if variable is zero");		
	}

	bool variable::operator<(variable const & other) const
	{
		if(is_numeric_type() && other.is_numeric_type())
		{
			if
			(
				type == variable_type_identifier::floating_point_value ||
				other.type == variable_type_identifier::floating_point_value
			)
				return get_floating_point_value() < other.get_floating_point_value();
			else if
			(
				type == variable_type_identifier::unsigned_integer &&
				other.type == variable_type_identifier::unsigned_integer
			)
				return unsigned_integer < other.unsigned_integer;
			else
				return signed_integer < other.signed_integer;
		}
		else if(type != other.type)
			return static_cast<word>(type) < static_cast<word>(other.type);
		else
		{
			if(type == variable_type_identifier::string)
				return *string < *other.string;
			else
				return hash() < other.hash();
		}
	}

	std::string get_type_string(variable_type type)
	{
		switch(type)
		{
		case variable_type_identifier::undefined:
			return "undefined";

		case variable_type_identifier::nil:
			return "nil";

		case variable_type_identifier::none:
			return "none";

		case variable_type_identifier::boolean:
			return "boolean";

		case variable_type_identifier::signed_integer:
			return "integer";

		case variable_type_identifier::unsigned_integer:
			return "unsigned-integer";

		case variable_type_identifier::floating_point_value:
			return "float";

		case variable_type_identifier::string:
			return "string";

		case variable_type_identifier::array:
			return "array";

		case variable_type_identifier::map:
			return "map";

		case variable_type_identifier::function:
			return "function";

		case variable_type_identifier::object:
			return "object";
		}

		return "unknown";
	}
}
