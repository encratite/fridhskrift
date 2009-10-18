#include <ail/string.hpp>
#include <fridh/symbol.hpp>

namespace fridh
{
	variable::variable():
		type(variable_type_identifier::undefined)
	{
	}

	variable::variable(variable const & other)
	{
		copy(other);
	}

	variable::~variable()
	{
		destroy();
	}

	variable & variable::operator=(variable const & other)
	{
		destroy();
		copy(other);
		return *this;
	}

	void variable::copy(variable const & other_pattern)
	{
		variable const & other = dynamic_cast<variable const &>(other_pattern);

		type = other.type;

#define COPY_MEMBER(type) \
			case variable_type_identifier::type: \
				type = other.type; \
				break;

#define COPY_MEMBER_POINTER(type, member_type, member) \
			case variable_type_identifier::type: \
				member = new member_type(*other.member); \
				break;

		switch(type)
		{
			COPY_MEMBER(boolean)
			COPY_MEMBER(signed_integer)
			COPY_MEMBER(unsigned_integer)
			COPY_MEMBER(floating_point_value)

			COPY_MEMBER_POINTER(string, std::string, string)
			COPY_MEMBER_POINTER(array, types::vector, array)
			COPY_MEMBER_POINTER(map, types::map, map)
		}

#undef COPY_MEMBER_POINTER
#undef COPY_MEMBER

	}

	void variable::destroy()
	{

#define DELETE_MEMBER(type) \
			case variable_type_identifier::type: \
				delete type; \
				break;

		switch(type)
		{
			DELETE_MEMBER(string)
			DELETE_MEMBER(array)
			DELETE_MEMBER(map)
		}

		type = variable_type_identifier::undefined;

#undef DELETE_MEMBER

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
