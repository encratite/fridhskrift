#pragma once

#include <string>
#include <vector>
#include <frith/symbol.hpp>

namespace frith
{
	namespace match_result
	{
		enum type
		{
			no_match,
			match,
			error,
		};
	}

	namespace process_line_result
	{
		enum type
		{
			ok,
			end_of_block,
			error,
		};
	}

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

		uword indentation_level;

		std::size_t
			lexeme_offset,
			lexeme_end;

		std::string error_message;

		bool in_a_class;

		std::vector<line_of_code> lines;

		symbol_tree_node * current_node;

		bool translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output);

		void error(std::string const & message);
	};
}
