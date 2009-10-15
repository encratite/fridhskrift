#include <fridh/lexer.hpp>
#include <ail/array.hpp>
#include <ail/string.hpp>
#include <boost/foreach.hpp>

namespace fridh
{
	boost::mutex table_mutex;

	line_of_code::line_of_code():
		indentation_level(0)
	{
	}

	operator_lexeme::operator_lexeme(lexeme_type::type lexeme, std::string const & string):
		lexeme(lexeme),
		string(string)
	{
	}

	bool operator_lexeme::operator<(operator_lexeme const & other) const
	{
		return string.length() > other.string.length();
	}

	lexer::lexer(std::string const & input, lines_of_code & lines):
		input(input),
		lines(lines)
	{
	}

	bool lexer::parse_operator(line_of_code & output)
	{
		std::size_t remaining_characters = end - i;

		BOOST_FOREACH(operator_lexeme & current_lexeme, operator_lexeme_data)
		{
			std::size_t operator_length = current_lexeme.string.size();
			if(remaining_characters < operator_length)
				return false;

			std::string substring = input.substr(i, operator_length);

			if(substring == current_lexeme.string)
			{
				output.lexemes.push_back(current_lexeme.lexeme);
				i += operator_length;
				return true;
			}
		}
		return false;
	}

	void lexer::lexer_error(std::string const & message, uword error_line)
	{
		if(error_line == 0)
			error_line = line;
		throw ail::exception("Lexer error: Line " + ail::number_to_string<uword>(error_line) + ": " + message);
	}
	
	void lexer::number_parsing_error(std::string const & message)
	{
		lexer_error(message);
	}
	
	bool lexer::is_name_char(char input)
	{
		return ail::is_alpha(input) || ail::is_digit(input) || input == '_';
	}

	void lexer::parse_name(line_of_code & output)
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
		{
			current_lexeme = lexeme(name);
			current_lexeme.type = lexeme_type::name;
		}

		output.lexemes.push_back(current_lexeme);
	}

	bool lexer::string_match(std::string const & target)
	{
		if(end - i < target.size())
			return false;

		return input.substr(i, target.size()) == target;
	}

	void lexer::process_newline()
	{
		if(!current_line.lexemes.empty())
		{
			current_line.line = line;
			lines.push_back(current_line);
		}
		std::string line_string = input.substr(line_offset, i - line_offset);
		current_line = line_of_code();
		line++;
		i++;
		line_offset = i;
	}

	void lexer::parse_lexemes()
	{
		initialise_tables();

		line = 1;

		line_offset = 0;

		for(i = 0, end = input.size(); i < end;)
		{
			if(parse_operator(current_line))
				continue;

			char const tab = '\t';

			char byte = input[i];

			switch(byte)
			{
				case tab:
					if(current_line.indentation_level > 0)
						lexer_error("Tabs are only permitted in the beginning of a line (offset " + ail::number_to_string(i - line_offset + 1) + ")");
					for(i++, current_line.indentation_level = 1; i < end && input[i] == tab; i++, current_line.indentation_level++);
					continue;

				case ' ':
				case '\r':
					i++;
					continue;

				case '\n':
				{
					process_newline();
					continue;
				}

				case '\'':
				case '"':
				{
					std::string string;
					parse_string(current_line);
					continue;
				}

				case ';':
					parse_comment();
					continue;
			}

			if(parse_number(current_line))
				continue;

			parse_name(current_line);
		}

		if(!current_line.lexemes.empty())
		{
			current_line.line = line;
			lines.push_back(current_line);
		}
	}

	std::string visualise_lexemes(lines_of_code & lines)
	{
		std::string output;

		BOOST_FOREACH(line_of_code & current_line, lines)
		{
			std::string number_string = ail::number_to_string(current_line.line);
			for(word i = 0, end = 5 - number_string.size(); i < end; i++)
				output += " ";
			output += number_string;
			output += ": ";
			for(uword indentation = 0; indentation < current_line.indentation_level; indentation++)
				output += "    ";
			bool first = true;
			BOOST_FOREACH(lexeme & current_lexeme, current_line.lexemes)
			{
				if(first)
					first = false;
				else
					output += " ";
				output += "[" + current_lexeme.to_string() + "]";
			}
			output += "\n";
		}

		return output;
	}

	bool lexer::parse(std::string & error)
	{
		try
		{
			parse_lexemes();
			return true;
		}
		catch(ail::exception & exception)
		{
			error = exception.get_message();
			return false;
		}
	}
}
