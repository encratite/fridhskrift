#pragma once

#include <string>
#include <vector>
#include <frith/symbol.hpp>

namespace frith
{
	class interpreter
	{
	public:
		interpreter();
		bool load_module(std::string const & path, std::string const & name, std::string & error_message);

	private:
		bool running;

		module main_module;
		std::vector<module> modules;

		std::size_t
			line_offset,
			line_end;

		std::size_t
			lexeme_offset,
			lexeme_end;

		std::vector<line_of_code> lines;

		symbol_tree_node * current_node;

		bool parse_class(std::string & error_message);

		bool translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message);

		void error(std::string const & message, std::string & output);
	};
}
