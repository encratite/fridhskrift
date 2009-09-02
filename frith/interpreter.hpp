#pragma once

#include <frith/scope.hpp>

namespace frith
{
	class interpreter
	{
	public:
		interpreter();
		bool execute(std::string const & data);
		bool execute_file(std::string const & path);

	private:
	};
}
