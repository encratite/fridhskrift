#pragma once

#include <string>
#include <vector>
#include <ail/types.hpp>
#include <fridh/symbol.hpp>
#include <boost/thread/mutex.hpp>

namespace fridh
{
	namespace lexeme_type
	{
		enum type
		{
			non_terminating_placeholder,

			name,

			boolean,
			signed_integer,
			unsigned_integer,
			floating_point_value,
			string,

			addition,
			subtraction,
			multiplication,
			division,
			modulo,
			exponentiation,

			negation,

			assignment,
			addition_assignment,
			subtraction_assignment,
			multiplication_assignment,
			division_assignment,
			modulo_assignment,
			exponentiation_assignment,

			increment,
			decrement,

			less_than,
			less_than_or_equal,
			greater_than,
			greater_than_or_equal,
			unequal,
			equal,

			logical_not,

			logical_and,
			logical_or,

			shift_left,
			shift_right,

			binary_and,
			binary_or,
			binary_xor,

			binary_not,

			bracket_start,
			bracket_end,

			array_start,
			array_end,

			scope_start,
			scope_end,

			iteration,
			iterator,

			function_declaration,
			anonymous_function_declaration,

			scope_operator,
			class_operator,

			selection_operator,

			call_operator,
		};
	}

	namespace lexeme_group
	{
		enum type
		{
			argument,
			unary_operator,
			binary_operator
		};
	}

	struct lexeme;

	typedef std::vector<lexeme> lexeme_container;

	struct lexeme
	{
		lexeme_type::type type;
		union
		{
			bool boolean;
			types::signed_integer signed_integer;
			types::unsigned_integer unsigned_integer;
			types::floating_point_value floating_point_value;
			std::string * string;
		};

		lexeme();
		lexeme(lexeme_type::type type);
		explicit lexeme(types::boolean boolean);
		explicit lexeme(types::signed_integer signed_integer);
		explicit lexeme(types::unsigned_integer unsigned_integer);
		explicit lexeme(types::floating_point_value floating_point_value);
		explicit lexeme(lexeme_type::type type, std::string const & string);
		std::string to_string() const;
	};

	struct line_of_code
	{
		uword line;
		uword indentation_level;
		lexeme_container lexemes;

		line_of_code();
	};

	struct operator_lexeme
	{
		lexeme_type::type lexeme;
		std::string string;

		operator_lexeme(lexeme_type::type lexeme, std::string const & string);
		bool operator<(operator_lexeme const & other) const;
	};

	class lexer
	{
	public:
		lexer(std::string const & input, std::vector<line_of_code> & lines, std::string & error);

		bool parse();

	private:
		std::string const & input;
		std::vector<line_of_code> & lines;
		std::string & error;

		uword line;
		std::size_t
			i,
			end,
			line_offset;
		line_of_code current_line;

		bool parse_operator(line_of_code & output);
		bool parse_string(line_of_code & output, std::string & error_message, std::string error_prefix = "");
		bool parse_number(line_of_code & output, bool & error_occured);
		void parse_name(line_of_code & output);
		bool parse_comment(std::string & error_message);

		std::string lexer_error(std::string const & message, uword error_line = 0);
		std::string number_parsing_error(std::string const & message, bool & error_occured);

		bool is_name_char(char input);
		bool string_match(std::string const & target);
		void process_newline();
	};

	std::string visualise_lexemes(std::vector<line_of_code> & lines);
	
	void initialise_tables();

	bool get_lexeme_group(lexeme_type::type input, lexeme_group::type & output);

	extern boost::mutex table_mutex;
	extern std::vector<operator_lexeme> operator_lexeme_data;
}
