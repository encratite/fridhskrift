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
			return_statement,
			if_statement,
			for_statement,
			while_statement,
		};
	}

	namespace assignment_type
	{
		enum type
		{
			assignment,
			addition,
			subtraction,
			multiplication,
			division,
			modulo,
			exponentiation,
		};
	}

	namespace operator_type
	{
		enum type
		{

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
