#include <fridh/lexer.hpp>
#include <ail/string.hpp>

namespace fridh
{
	bool lexer::parse_number(line_of_code & output, bool & error_occured)
	{
		std::size_t start = i;
		char byte = input[i];
		error_occured = false;

		if(ail::is_digit(byte))
		{
			i++;
			if(byte == '0')
			{
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
							error = number_parsing_error("Incomplete hex number at the end of the input", error_occured);
							return false;
						}

						std::size_t hex_start = i;

						for(; i < end && ail::is_hex_digit(input[i]); i++);
						
						std::size_t hex_length = i - hex_start;
						if(hex_length == 0)
						{
							error_occured = true;
							error = lexer_error("Incomplete hex number");
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
			char last_byte = byte;
			for(; i < end; i++)
			{
				byte = input[i];
				if(byte == dot)
				{
					if(got_dot)
					{
						error = number_parsing_error("Encountered a floating point value containing multiple dots", error_occured);
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
				error = number_parsing_error("Encountered a floating point value ending with a dot", error_occured);
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
}
