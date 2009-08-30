#pragma once

namespace frith
{
	enum executable_unit_type
	{
		executable_unit_type_statement,
		executable_unit_type_if,
		executable_unit_type_for,
		executable_unit_type_while
	};

	struct executable_unit
	{
		line_of_code line;
	};
}
