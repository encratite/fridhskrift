#pragma once

#include <string>
#include <vector>
#include <frith/scope.hpp>

namespace frith
{
	class interpreter
	{
	public:
		interpreter();
		bool execute(std::string const & data, std::string & error_message);
		bool execute_file(std::string const & path, std::string & error_message);

	private:
		bool running;

		module main_module;
		std::vector<module> modules;

		bool translate_data(std::string const & data, std::string & error_message);
	};
}
