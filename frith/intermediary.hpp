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

	namespace operator_precedence
	{
		enum type
		{
			selection,

			negation,
			logical_not,
			binary_not,

			exponentiation,

			multiplication,
			division,
			modulo,

			addition,
			subtraction,

			shift_left,
			shift_right,

			less_than,
			less_than_or_equal,

			greater_than,
			greater_than_or_equal,

			equal,
			not_equal,

			binary_and,
			binary_or,

			logical_and,
			logical_or,
		};
	}

	class intermediary_translator
	{
	public:
		intermediary_translator();
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

		uword nested_class_level;

		std::vector<line_of_code> lines;

		symbol_tree_node * current_node;

		bool name_is_used(std::string const & name);
		std::string const & get_declaration_name();
		bool name_collision_check();
		symbol_tree_node & add_name(symbol::type symbol_type);
		match_result::type process_body(function * current_function = 0);

		match_result::type process_class();
		match_result::type process_function();
		bool process_statement(function & current_function);
		process_line_result::type process_line(function * active_function = 0);

		bool translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output);

		void error(std::string const & message);
	};
}
