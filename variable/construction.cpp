#include <frith/variable.hpp>

namespace frith
{
	void variable::nil()
	{
		type = variable_type_identifier::nil;
	}

	void variable::new_boolean(types::boolean new_boolean)
	{
		type = variable_type_identifier::boolean;
		boolean = new_boolean;
	}

	void variable::new_signed_integer(types::signed_integer new_signed_integer)
	{
		type = variable_type_identifier::signed_integer;
		signed_integer = new_signed_integer;
	}

	void variable::new_unsigned_integer(types::unsigned_integer new_unsigned_integer)
	{
		type = variable_type_identifier::unsigned_integer;
		unsigned_integer = new_unsigned_integer;
	}

	void variable::new_floating_point_value(types::floating_point_value new_floating_point_value)
	{
		type = variable_type_identifier::floating_point_value;
		floating_point_value = new_floating_point_value;
	}

	void variable::new_string(types::string const & new_string)
	{
		type = variable_type_identifier::string;
		string = new std::string(new_string);
	}

	void variable::new_array()
	{
		type = variable_type_identifier::array;
		array = new types::vector();
	}

	void variable::new_map()
	{
		type = variable_type_identifier::map;
		map = new types::map();
	}
}
