#pragma once

#include <ail/types.hpp>
#include <string>
#include <vector>
#include <fridh/symbol.hpp>

namespace fridh
{
	namespace operator_precedence
	{
		enum type
		{
			selection,

			call,

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

	namespace statement_processing
	{
		enum type
		{
			unrestricted,
			atomic_only
		};
	}

	struct sorted_binary_operator_entry
	{

		std::size_t index;
	};

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

		uword nested_class_level;

		lines_of_code lines;

		symbol_tree_node * current_node;

		bool name_is_used(std::string const & name);
		std::string const & get_declaration_name();
		void name_collision_check();
		symbol_tree_node & add_name(symbol::type symbol_type);

		void process_body(executable_units * output = 0);

		void process_atomic_statement(lexeme_container & lexemes, std::size_t & offset, parse_tree_node & output, bool allow_multi_statements = false, lexeme_type::type terminator = lexeme_type::non_terminating_placeholder);

		bool process_class();
		bool process_function();
		void process_statement(executable_units & output);
		bool process_line(executable_unit * output = 0);

		bool translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output);

		void error(std::string const & message);
	};

	void lexeme_to_argument_node(lexeme & input, parse_tree_node & output);
	void lexeme_to_unary_operator_node(lexeme & input, parse_tree_node & output);
	void lexeme_to_binary_operator_node(lexeme & input, parse_tree_node & output);

	word get_unary_operator_precedence(unary_operator_type::type input);
	word get_binary_operator_precedence(binary_operator_type::type input);
	bool get_parse_tree_node_precedence(parse_tree_node & input, word & output);

	bool is_right_to_left_operator(parse_tree_node & input);
}
