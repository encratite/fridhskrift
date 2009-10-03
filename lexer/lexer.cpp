#include <frith/lexer.hpp>
#include <ail/array.hpp>
#include <ail/string.hpp>
#include <boost/foreach.hpp>

namespace frith
{
	boost::mutex table_mutex;

	line_of_code::line_of_code():
		indentation_level(0)
	{
	}

	operator_lexeme_type::operator_lexeme(lexeme_type::type lexeme, std::string const & string):
		lexeme(lexeme),
		string(string)
	{
	}

	bool operator_lexeme_type::operator<(operator_lexeme const & other) const
	{
		return string.length() > other.string.length();
	}

	lexer::lexer(std::string const & input, std::vector<line_of_code> & lines, std::string & error):
		input(input),
		lines(lines),
		error(error)
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
				output.lexemes.push_back(lexeme(current_lexeme.lexeme));
				i += operator_length;
				return true;
			}
		}
		return false;
	}

	std::string lexer::lexer_error(std::string const & message, uword error_line)
	{
		if(error_line == 0)
			error_line = line;
		return "Line " + ail::number_to_string<uword>(error_line) + ": " + message;
	}
	
	std::string lexer::number_parsing_error(std::string const & message, bool & error_occured)
	{
		error_occured = true;
		return lexer_error(message);
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
			current_lexeme = lexeme(name, name);

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

	bool lexer::parse()
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
					{
						error = lexer_error("Tabs are only permitted at the beginning of a line (offset " + ail::number_to_string(i - line_offset + 1) + ")");
						return false;
					}
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
					if(!parse_string(current_line, error))
						return false;
					continue;
				}

				case ';':
					if(!parse_comment(error))
						return false;
					continue;
			}

			bool error_occured;
			if(parse_number(current_line, error_occured))
				continue;

			if(error_occured)
				return false;

			parse_name(current_line);
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
}
