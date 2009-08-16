#include <algorithm>
#include <map>
#include <lexer.hpp>
#include <ail/array.hpp>
#include <ail/string.hpp>

lexeme::lexeme()
{
}

lexeme::lexeme(lexeme_type type):
	type(type)
{
}

explicit lexeme::lexeme(types::signed_integer signed_integer):
	type(lexeme_type_signed_integer),
	signed_integer(signed_integer)
{
}

explicit lexeme::lexeme(types::unsigned_integer unsigned_integer):
	type(lexeme_type_umsigned_integer),
	signed_integer(unsigned_integer)
{
}

explicit lexeme::lexeme(types::floating_point_value floating_point_value)
	type(lexeme_type_floating_point_value),
	signed_integer(floating_point_value)
{
}

line_of_code::line_of_code():
	indentation_level(0)
{
}

explicit lexeme::lexeme(lexeme_type type, std::string const & string)
	type(type),
	string(new std::string(string))
{
}

operator_lexeme::operator_lexeme(lexeme_type lexeme, std::string const & string):
	lexeme(lexeme),
	string(string)
{
}

bool operator_lexeme::operator<(operator_lexeme const & other) const
{
	return string.length() > other.string.length();
}

namespace
{
	bool tables_initialised = false;

	operator_lexeme operators[] =
	{
		operator_lexeme(lexeme_type_addition, "+"),
		operator_lexeme(lexeme_type_subtraction, "-"),
		operator_lexeme(lexeme_type_multiplication, "*"),
		operator_lexeme(lexeme_type_division, "/"),
		operator_lexeme(lexeme_type_modulo, "%"),

		operator_lexeme(lexeme_type_less_than, "<"),
		operator_lexeme(lexeme_type_less_than_or_equal, "<="),
		operator_lexeme(lexeme_type_greater_than, ">"),
		operator_lexeme(lexeme_type_greater_than_or_equal, ">="),
		operator_lexeme(lexeme_type_unequal, "!="),
		operator_lexeme(lexeme_type_equal, "="),

		operator_lexeme(lexeme_type_logical_not, "!"),

		operator_lexeme(lexeme_type_logical_and, "&"),
		operator_lexeme(lexeme_type_logical_or, "|"),

		operator_lexeme(lexeme_type_shift_left, "<<"),
		operator_lexeme(lexeme_type_shift_right, ">>"),

		operator_lexeme(lexeme_type_binary_and, "&&"),
		operator_lexeme(lexeme_type_binary_or, "||"),
		operator_lexeme(lexeme_type_binary_xor, "^"),

		operator_lexeme(lexeme_type_binary_not, "~"),

		operator_lexeme(lexeme_type_bracket_left, "["),
		operator_lexeme(lexeme_type_bracket_right, "]"),

		operator_lexeme(lexeme_type_array_left, "{"),
		operator_lexeme(lexeme_type_array_right, "}"),
	};
}

void initialise_tables()
{
	if(tables_initialised)
		return;

	tables_initialised = true;

	std::sort(operators, operators + ail::countof(operators));
}

bool parse_operator(std::string const & input, std::size_t & offset, lexeme & output)
{
	for(std::size_t i = 0, end = ail::countof(operators); i < end; i++)
	{
		operator_lexeme & current_lexeme = operators[i];
		std::size_t remaining_characters = end - i;
		std::size_t operator_length = current_lexeme.string.size();
		if(remaining_characters < operator_length)
			return false;

		if(input.substr(i, operator_length) == current_lexeme.string)
		{
			output = lexeme(current_lexeme.lexeme);
			offset += operator_length;
			return true;
		}
	}
	return false;
}

std::string lexer_error(std::string const & message, uword line)
{
	return "Line " + ail::number_to_string<uword>(line) + ": " + message;
}

bool parse_string(std::string const & input, std::size_t & i, std::size_t end, uword line, std::string & output, std::string & error_message)
{
	i++;
	std::size_t start = i;
	for(; i < end; i++)
	{
		char byte = input[i];
		switch(byte)
		{
			case '\\':
			{
				if(end - i < 2)
				{
					error_message = lexer_error("Backslash at the end of the input", line);
					return false;
				}

				i++;

				char next_byte = input[i];
				
				switch(next_byte)
				{
					case 'r':
						output.push_back('\r');
						continue;

					case 'n':
						output.push_back('\n');
						continue;
				}

				if(ail::is_hex_digit(next_byte))
				{
					if(end - i < 2)
					{
						error_message = lexer_error("Incomplete hex number escape sequence at the end of the input", line);
						return false;
					}
					if(!ail::is_hex_digit(input[i + 1]))
					{
						error_message = lexer_error("Invalid hex number escape sequence", line);
						return false;
					}
					std::string hex_string = input.substr(i, 2);
					i++;
					char new_byte = ail::string_to_number<char>(hex_string, std::ios_base::hex);
					output.push_back(new_byte);
				}
				else
				{
					error_message = lexer_error("Invalid escape sequence: " + ail::hex_string_8(static_cast<uchar>(next_byte)), line);
					return false;
				}
				break;
			}

			case '\n':
				error_message = lexer_error("Detected a newline in a string", line);
				return false;

			case '\'':
				i++;
				return true;

			default:
				output.push_back(byte);
				break;
		}
	}
}

bool parse_lexemes(std::string const & input, std::vector<line_of_code> & lines, std::string & error)
{
	initialise_tables();

	line_of_code current_line;
	uword line = 1;

	for(std::size_t i = 0, end = input.size(); i < end;)
	{
		lexeme current_operator;
		if(parse_operator(input, i, current_operator))
		{
			current_line.lexemes.push_back(current_operator);
			continue;
		}

		char const tab = '\t';

		char byte = input[i];
		switch(byte)
		{
			case tab:
				if(current_line.indentation_level > 0)
				{
					error = lexer_error("Tabs are only permitted at the beginning of a line", current_line);
					return false;
				}
				for(i++, current_line.indentation_level = 1; i < end && input[i] == tab; i++, current_line.indentation_level++);
				continue;

			case '\n':
				current_line.line = line;
				lines.push_back(current_line);
				current_line = line_of_code();
				line++;
				break;

			case '\'':
			{
				std::string string;
				if(!parse_string(input, i, end, line, string, error))
					return false;
				continue;
			}
		}



		i++;
	}
}
