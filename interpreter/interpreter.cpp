#include <ail/file.hpp>
#include <ail/string.hpp>
#include <frith/interpreter.hpp>
#include <frith/lexer.hpp>

namespace frith
{
	interpreter::interpreter():
		running(false)
	{
	}

	bool interpreter::load_module(std::string const & path, std::string const & name, std::string & error_message);
	{
		std::string content;
		if(!ail::read_file(path, content))
		{
			error_message = "Unable to read file \"" + path + "\"";
			return false;
		}

		return translate_data(content, error_message);
	}

	bool interpreter::translate_data(std::string const & data, std::string const & module_name, std::string & error_message)
	{
		lines = std::vector<line_of_code>();
		lexer current_lexer(data, lines, error_message);
		if(!current_lexer.parse())
			return false;

		uword indentation_level = 0;

		for(line_offset = 0, line_end = lines.size(); line_offset < line_end; line_offset)
		{
			line_of_code & current_line = lines[line_offset];
			word difference = current_line.indentation_level - indentation_level;
			if(difference > 1)
			{
				error("Invalid jump in the indentation level (difference is " + ail::number_to_string(difference) + ")", error_message);
			}

			std::vector<lexeme> & current_lexemes = current_line.lexemes;

			for(lexeme_offset = 0, lexeme_end = current_lexemes.size(); lexeme_offset < lexeme_end;)
			{
				lexeme & current_lexeme = current_lexemes[lexeme_offset];
				switch(current_lexeme.type)
				{
					case lexeme_type_class_operator:
						continue;

					default:
						error("Unprocessed lexeme type encountered: " + current_lexeme.to_string(), error_message);
						return false;
				}

				lexeme_offset++;
			}
		}

		return true;
	}

	void interpreter::error(std::string const & message, std::string & output)
	{
		output = "Line " + ail::number_to_string(lines[line_offset].line) + ": " + message;
	}
}
