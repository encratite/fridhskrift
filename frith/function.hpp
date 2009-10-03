#pragma once

#include <vector>
#include <string>

namespace frith
{
	namespace executable_unit
	{
		enum type
		{
			statement,
			assignment,
			if_statement,
			for_statement,
			while_statement,
		};
	}

	struct executable_unit
	{
		executable_unit_type type;
	};

	struct function
	{
		std::vector<std::string> arguments;
		std::vector<executable_unit> units;
	};
}
