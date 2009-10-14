#include <fridh/lexer.hpp>
#include <ail/string.hpp>

namespace fridh
{
	void lexer::parse_string(line_of_code & output)
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
						lexer_error("Backslash at the end of the input");

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
							lexer_error("Incomplete hex number escape sequence at the end of the input");

						if(!ail::is_hex_digit(input[i + 1]))
							lexer_error("Invalid hex number escape sequence");

						std::string hex_string = input.substr(i, 2);
						i++;
						char new_byte = ail::string_to_number<char>(hex_string, std::ios_base::hex);
						string.push_back(new_byte);
					}
					else
						lexer_error("Invalid escape sequence: " + ail::hex_string_8(static_cast<uchar>(next_byte)));
					break;
				}

				case '\n':
					lexer_error("Detected a newline in a string");
					break;

				case '\'':
				case '"':
					if(byte == string_character)
					{
						output.lexemes.push_back(lexeme(lexeme_type::string, string));
						i++;
						return;
					}
					string.push_back(byte);
					break;

				default:
					string.push_back(byte);
					break;
			}
		}
		lexer_error("String lacks terminator");
	}
}
