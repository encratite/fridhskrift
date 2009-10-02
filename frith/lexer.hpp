#pragma once

#include <string>
#include <vector>
#include <ail/types.hpp>
#include <frith/variable.hpp>
#include <boost/thread/mutex.hpp>

namespace frith
{
	enum lexeme_type
	{
		lexeme_type_name,

		lexeme_type_boolean,
		lexeme_type_signed_integer,
		lexeme_type_unsigned_integer,
		lexeme_type_floating_point_value,
		lexeme_type_string,

		lexeme_type_addition,
		lexeme_type_subtraction,
		lexeme_type_multiplication,
		lexeme_type_division,
		lexeme_type_modulo,
		lexeme_type_exponentiation,

		lexeme_type_increment,
		lexeme_type_decrement,

		lexeme_type_less_than,
		lexeme_type_less_than_or_equal,
		lexeme_type_greater_than,
		lexeme_type_greater_than_or_equal,
		lexeme_type_unequal,
		lexeme_type_equal,

		lexeme_type_logical_not,

		lexeme_type_logical_and,
		lexeme_type_logical_or,

		lexeme_type_shift_left,
		lexeme_type_shift_right,

		lexeme_type_binary_and,
		lexeme_type_binary_or,
		lexeme_type_binary_xor,

		lexeme_type_binary_not,

		lexeme_type_bracket_start,
		lexeme_type_bracket_end,

		lexeme_type_array_start,
		lexeme_type_array_end,

		lexeme_type_scope_start,
		lexeme_type_scope_end,

		lexeme_type_iteration,
		lexeme_type_iterator,
		lexeme_type_function_declaration,
		lexeme_type_anonymous_function_declaration,
		lexeme_type_class_operator,
		lexeme_type_dot,
		lexeme_type_scope_operator,
	};

	struct lexeme
	{
		lexeme_type type;
		union
		{
			bool boolean;
			types::signed_integer signed_integer;
			types::unsigned_integer unsigned_integer;
			types::floating_point_value floating_point_value;
			std::string * string;
		};

		lexeme();
		lexeme(lexeme_type type);
		explicit lexeme(types::boolean boolean);
		explicit lexeme(types::signed_integer signed_integer);
		explicit lexeme(types::unsigned_integer unsigned_integer);
		explicit lexeme(types::floating_point_value floating_point_value);
		explicit lexeme(lexeme_type type, std::string const & string);
		std::string to_string() const;
	};

	struct line_of_code
	{
		uword line;
		uword indentation_level;
		std::vector<lexeme> lexemes;

		line_of_code();
	};

	struct operator_lexeme
	{
		lexeme_type lexeme;
		std::string string;

		operator_lexeme(lexeme_type lexeme, std::string const & string);
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

	extern boost::mutex table_mutex;
	extern std::vector<operator_lexeme> operator_lexeme_data;
}
