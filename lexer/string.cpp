#include <frith/lexer.hpp>
#include <ail/string.hpp>

namespace frith
{
	bool lexer::parse_string(line_of_code & output, std::string & error_message, std::string error_prefix)
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
						error_message = lexer_error(error_prefix + "Backslash at the end of the input");
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
							error_message = lexer_error(error_prefix + "Incomplete hex number escape sequence at the end of the input");
							return false;
						}
						if(!ail::is_hex_digit(input[i + 1]))
						{
							error_message = lexer_error(error_prefix + "Invalid hex number escape sequence");
							return false;
						}
						std::string hex_string = input.substr(i, 2);
						i++;
						char new_byte = ail::string_to_number<char>(hex_string, std::ios_base::hex);
						string.push_back(new_byte);
					}
					else
					{
						error_message = lexer_error(error_prefix + "Invalid escape sequence: " + ail::hex_string_8(static_cast<uchar>(next_byte)));
						return false;
					}
					break;
				}

				case '\n':
					error_message = lexer_error(error_prefix + "Detected a newline in a string");
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
		error_message = lexer_error(error_prefix + "String lacks terminator");
		return false;
	}
}
