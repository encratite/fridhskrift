#include <iostream>
#include <algorithm>
#include <map>
#include <frith/lexer.hpp>
#include <ail/array.hpp>
#include <ail/string.hpp>
#include <boost/foreach.hpp>

namespace frith
{

	lexeme::lexeme()
	{
	}

	lexeme::lexeme(lexeme_type type):
		type(type)
	{
	}

	lexeme::lexeme(types::boolean boolean):
		type(lexeme_type_boolean),
		boolean(boolean)
	{
	}

	lexeme::lexeme(types::signed_integer signed_integer):
		type(lexeme_type_signed_integer),
		signed_integer(signed_integer)
	{
	}

	lexeme::lexeme(types::unsigned_integer unsigned_integer):
		type(lexeme_type_unsigned_integer),
		unsigned_integer(unsigned_integer)
	{
	}

	lexeme::lexeme(types::floating_point_value floating_point_value):
		type(lexeme_type_floating_point_value),
		floating_point_value(floating_point_value)
	{
	}

	lexeme::lexeme(lexeme_type type, std::string const & string):
		type(type),
		string(new std::string(string))
	{
	}

	std::string lexeme::to_string() const
	{
		switch(type)
		{
			case lexeme_type_name:
				return "name: " + *string;

			case lexeme_type_boolean:
				return "boolean: " + ail::bool_to_string(boolean);

			case lexeme_type_signed_integer:
				return "integer: " + ail::number_to_string(signed_integer);

			case lexeme_type_unsigned_integer:
				return "unsigned-integer: " + ail::number_to_string(unsigned_integer);

			case lexeme_type_floating_point_value:
				return "float: " + ail::number_to_string(floating_point_value);

			case lexeme_type_string:
				return "string: " + *string;

			case lexeme_type_addition:
				return "+";

			case lexeme_type_subtraction:
				return "-";

			case lexeme_type_multiplication:
				return "*";

			case lexeme_type_division:
				return "/";

			case lexeme_type_modulo:
				return "%";

			case lexeme_type_increment:
				return "++";

			case lexeme_type_decrement:
				return "--";

			case lexeme_type_exponentiation:
				return "**";

			case lexeme_type_less_than:
				return "<";

			case lexeme_type_less_than_or_equal:
				return "<=";

			case lexeme_type_greater_than:
				return ">";

			case lexeme_type_greater_than_or_equal:
				return ">=";

			case lexeme_type_unequal:
				return "!=";

			case lexeme_type_equal:
				return "=";

			case lexeme_type_logical_not:
				return "!";

			case lexeme_type_logical_and:
				return "&";

			case lexeme_type_logical_or:
				return "|";

			case lexeme_type_shift_left:
				return "<<";

			case lexeme_type_shift_right:
				return ">>";

			case lexeme_type_binary_and:
				return "&&";

			case lexeme_type_binary_or:
				return "||";

			case lexeme_type_binary_xor:
				return "^";

			case lexeme_type_binary_not:
				return "~";

			case lexeme_type_bracket_start:
				return "bracket: start";

			case lexeme_type_bracket_end:
				return "bracket: end";

			case lexeme_type_array_start:
				return "array: start";

			case lexeme_type_array_end:
				return "array: end";

			case lexeme_type_scope_start:
				return "scope: start";

			case lexeme_type_scope_end:
				return "scope: end";

			case lexeme_type_iteration:
				return "iteration";

			case lexeme_type_function_declaration:
				return "function";

			case lexeme_type_anonymous_function_declaration:
				return "anonymous function";

			case lexeme_type_class_declaration:
				return "class";

			case lexeme_type_dot:
				return ".";

			default:
				return "unknown";
		}
	}

	line_of_code::line_of_code():
		indentation_level(0)
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
			operator_lexeme(lexeme_type_exponentiation, "**"),

			operator_lexeme(lexeme_type_increment, "++"),
			operator_lexeme(lexeme_type_decrement, "--"),

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

			operator_lexeme(lexeme_type_bracket_start, "["),
			operator_lexeme(lexeme_type_bracket_end, "]"),

			operator_lexeme(lexeme_type_array_start, "{"),
			operator_lexeme(lexeme_type_array_end, "}"),

			operator_lexeme(lexeme_type_scope_start, "("),
			operator_lexeme(lexeme_type_scope_end, ")"),

			operator_lexeme(lexeme_type_iteration, "\\"),
			operator_lexeme(lexeme_type_function_declaration, "@"),
			operator_lexeme(lexeme_type_anonymous_function_declaration, "@@"),
			operator_lexeme(lexeme_type_class_declaration, "$"),
			operator_lexeme(lexeme_type_dot, "."),
		};
	}

	void initialise_tables()
	{
		if(tables_initialised)
			return;

		tables_initialised = true;

		std::sort(operators, operators + ail::countof(operators));
	}

	bool parse_operator(std::string const & input, std::size_t & offset, line_of_code & output)
	{
		for(std::size_t i = 0, end = ail::countof(operators); i < end; i++)
		{
			operator_lexeme & current_lexeme = operators[i];
			std::size_t remaining_characters = end - i;
			std::size_t operator_length = current_lexeme.string.size();
			if(remaining_characters < operator_length)
				return false;

			std::string substring = input.substr(offset, operator_length);

			//std::cout << "Comparing \"" << substring << "\" to \"" << current_lexeme.string << "\"" << std::endl;

			if(substring == current_lexeme.string)
			{
				bool has_next_character = remaining_characters >= operator_length + 1;
				if(has_next_character)
				{
					char next_character = input[offset + operator_length];
					if(!ail::is_whitespace(next_character))
						return false;
				}
				output.lexemes.push_back(lexeme(current_lexeme.lexeme));
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

	bool parse_string(std::string const & input, std::size_t & i, std::size_t end, uword line, line_of_code & output, std::string & error_message, std::string error_prefix = "")
	{
		std::string string;
		char string_character = input[i];
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
						error_message = lexer_error(error_prefix + "Backslash at the end of the input", line);
						return false;
					}

					i++;

					char next_byte = input[i];
					
					switch(next_byte)
					{
						case 'r':
							string.push_back('\r');
							continue;

						case 'n':
							string.push_back('\n');
							continue;
					}

					if(ail::is_hex_digit(next_byte))
					{
						if(end - i < 2)
						{
							error_message = lexer_error(error_prefix + "Incomplete hex number escape sequence at the end of the input", line);
							return false;
						}
						if(!ail::is_hex_digit(input[i + 1]))
						{
							error_message = lexer_error(error_prefix + "Invalid hex number escape sequence", line);
							return false;
						}
						std::string hex_string = input.substr(i, 2);
						i++;
						char new_byte = ail::string_to_number<char>(hex_string, std::ios_base::hex);
						string.push_back(new_byte);
					}
					else
					{
						error_message = lexer_error(error_prefix + "Invalid escape sequence: " + ail::hex_string_8(static_cast<uchar>(next_byte)), line);
						return false;
					}
					break;
				}

				case '\n':
					error_message = lexer_error(error_prefix + "Detected a newline in a string", line);
					return false;

				case '\'':
				case '"':
					if(byte == string_character)
					{
						output.lexemes.push_back(lexeme(lexeme_type_string, string));
						i++;
						return true;
					}
					string.push_back(byte);
					break;

				default:
					string.push_back(byte);
					break;
			}
		}
		error_message = lexer_error(error_prefix + "String lacks terminator", line);
		return false;
	}

	std::string number_parsing_error(std::string const & message, bool & error_occured, uword line)
	{
		error_occured = true;
		return lexer_error(message, line);
	}

	bool parse_number(std::string const & input, std::size_t & i, std::size_t end, uword line, line_of_code & output, bool & error_occured, std::string & error_message)
	{
		std::size_t start = i;
		char byte = input[i];
		error_occured = false;

		if(ail::is_digit(byte))
		{
			if(byte == '0')
			{
				i++;
				std::size_t remaining_bytes = end - i;
				if(remaining_bytes > 1)
				{
					char next_byte = input[i + 1];
					if(next_byte == 'x')
					{
						i++;
						remaining_bytes = end - i;
						if(remaining_bytes == 0)
						{
							error_message = number_parsing_error("Incomplete hex number at the end of the input", error_occured, line);
							return false;
						}

						std::size_t hex_start = i;

						for(; i < end && ail::is_hex_digit(input[i]); i++);
						
						std::size_t hex_length = i - hex_start;
						if(hex_length == 0)
						{
							error_occured = true;
							error_message = lexer_error("Incomplete hex number", line);
							return false;
						}

						std::string hex_string = input.substr(hex_start, i - end);
						types::unsigned_integer value = ail::string_to_number<types::unsigned_integer>(hex_string, std::ios_base::hex);
						output.lexemes.push_back(lexeme(value));
						return true;
					}
				}
			}

			char const dot = '.';

			bool got_dot = false;
			std::size_t start = i;
			char last_byte = byte;
			for(i++; i < end; i++)
			{
				byte = input[i];
				if(byte == dot)
				{
					if(got_dot)
					{
						error_message = number_parsing_error("Encountered a floating point value containing multiple dots", error_occured, line);
						return false;
					}
					else
						got_dot = true;
				}
				else if(!ail::is_digit(byte))
					break;

				last_byte = byte;
			}

			if(last_byte == dot)
			{
				error_message = number_parsing_error("Encountered a floating point value ending with a dot", error_occured, line);
				return false;
			}

			std::string number_string = input.substr(start, i - start);
			lexeme current_lexeme;
			if(got_dot)
				current_lexeme = lexeme(ail::string_to_number<types::floating_point_value>(number_string));
			else
				current_lexeme = lexeme(ail::string_to_number<types::signed_integer>(number_string));
			output.lexemes.push_back(current_lexeme);

			return true;
		}
		else
			return false;
	}

	bool is_name_char(char input)
	{
		return !ail::is_whitespace(input);
	}

	void parse_name(std::string const & input, std::size_t & i, std::size_t end, line_of_code & output)
	{
		std::size_t start = i;
		for(i++; i < end && is_name_char(input[i]); i++);
		std::string name = input.substr(start, i - start);

		lexeme current_lexeme;
		if(name == "true")
			current_lexeme = lexeme(true);
		else if(name == "false")
			current_lexeme = lexeme(false);
		else
			current_lexeme = lexeme(lexeme_type_name, name);

		output.lexemes.push_back(current_lexeme);
	}

	bool string_match(std::string const & input, std::size_t i, std::size_t end, std::string const & target)
	{
		if(end - i < target.size())
			return false;

		return input.substr(i, target.size()) == target;
	}

	bool parse_comment(std::string const & input, std::size_t & i, std::size_t end, uword & line, std::string & error_message)
	{
		std::string const
			multi_line_comment = ";;",
			nested_comment_start = ";.",
			nested_command_end = ".;",

			multiline_comment_prefix = "In multi-line comment: ",
			nested_comment_prefix = "In nested comment: ";

		if(string_match(input, i, end, multi_line_comment))
		{
			bool got_end = false;
			for(i += multi_line_comment.size(); !got_end && i < end; i++)
			{
				char byte = input[i];
				switch(byte)
				{
					case '\'':
					case '"':
					{
						line_of_code unused;
						if(!parse_string(input, i, end, line, unused, error_message, multiline_comment_prefix))
							return false;
						break;
					}

					case '\n':
						line++;
						break;

					case ';':
						if(string_match(input, i, end, multi_line_comment))
							got_end = true;
						break;
				}
			}
			if(!got_end)
			{
				error_message = lexer_error("Unable to find the end of a multi-line comment", line);
				return false;
			}
		}
		else if(string_match(input, i, end, nested_comment_start))
		{
			uword comment_depth = 1;
			for(i += recursive_comment_start.size(); comment_depth > 0 && i < end; i++)
			{
				if(string_match(input, i, end, nested_comment_start))
				{
					comment_depth++;
					i += nested_comment_start.size();
				}
				else if(string_match(input, i, end, nested_comment_start))
				{
					comment_depth--;
					i += nested_comment_end.size();
				}
			}
			if(comment_depth != 0)
			{
				error_message = lexer_error("Unable to find the end of a nested comment", line);
				return false;
			}
		}
		else
		{
			std::size_t offset = input.find('\n', i);
			if(offset == std::string::npos)
			{
				error_message = lexer_error("Unable to find the end of a multi-line comment", line);
				return false;
			}
			line++;
			i = offset + 1;
		}
		return true;
	}

	bool parse_lexemes(std::string const & input, std::vector<line_of_code> & lines, std::string & error)
	{
		initialise_tables();

		line_of_code current_line;
		uword line = 1;

		for(std::size_t i = 0, end = input.size(); i < end;)
		{
			if(parse_operator(input, i, current_line))
				continue;

			char const tab = '\t';

			char byte = input[i];
			switch(byte)
			{
				case tab:
					if(current_line.indentation_level > 0)
					{
						error = lexer_error("Tabs are only permitted at the beginning of a line", line);
						return false;
					}
					for(i++, current_line.indentation_level = 1; i < end && input[i] == tab; i++, current_line.indentation_level++);
					continue;

				case ' ':
				case '\r':
					i++;
					continue;

				case '\n':
					if(!current_line.lexemes.empty())
					{
						current_line.line = line;
						lines.push_back(current_line);
					}
					current_line = line_of_code();
					line++;
					i++;
					continue;

				case '\'':
				case '"':
				{
					std::string string;
					if(!parse_string(input, i, end, line, current_line, error))
						return false;
					continue;
				}
			}

			bool error_occured;
			if(parse_number(input, i, end, line, current_line, error_occured, error))
				continue;

			if(error_occured)
				return false;

			parse_name(input, i, end, current_line);
		}

		if(!current_line.lexemes.empty())
		{
			current_line.line = line;
			lines.push_back(current_line);
		}

		return true;
	}

	std::string visualise_lexemes(std::vector<line_of_code> & lines)
	{
		uword line = 1;

		std::string output;

		BOOST_FOREACH(line_of_code & current_line, lines)
		{
			output += ail::number_to_string(line) + ": ";
			for(uword indentation = 0; indentation < current_line.indentation_level; indentation++)
				output += "  ";
			bool first = true;
			BOOST_FOREACH(lexeme & current_lexeme, current_line.lexemes)
			{
				if(first)
					first = false;
				else
					output += " ";
				output += "[" + current_lexeme.to_string() + "]";
			}
			line++;
			output += "\n";
		}

		return output;
	}
}
