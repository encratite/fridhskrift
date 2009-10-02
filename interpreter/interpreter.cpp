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

		module module_output;
		bool success = translate_data(module_output, content, error_message);
		if(!success)
			return false;

		return true;
	}

	bool interpreter::parse_class(std::string & error_message)
	{
		bool class_error(std::string const & message)
		{
			error_message = error(error_prefix + message, error_message);
			return false;
		}

		std::string const error_prefix = "Class operator error: ";
		line_of_code & current_line = lines[line_offset];
		if(current_line.lexemes.size() != 2)
		{
			error_message = error(error_prefix + "Invalid token count for a class operator", error_message);
			return false;
		}
		lexeme & name_lexeme = current_line.lexemes[1];
		if(name_lexeme.type != lexeme_type_name)
		{
			error_message = error(error_prefix + "The second lexeme must be a name", error_message);
			return false;
		}
		std::string const & class_name = *name_lexeme.string;
		frith::symbol_tree_node * node;
		frith::symbol_tree_entity * entity;
		if(current_node->find_entity(class_name, node, entity))
		{
		}
	}

	bool interpreter::translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message)
	{
		lines = std::vector<line_of_code>();
		lexer current_lexer(data, lines, error_message);
		if(!current_lexer.parse())
			return false;

		current_node = 0;

		uword indentation_level = 0;
		bool expected_indentation = false;

		for(line_offset = 0, line_end = lines.size(); line_offset < line_end;)
		{
			line_of_code & current_line = lines[line_offset];
			word difference = current_line.indentation_level - indentation_level;
			if(difference > 1)
			{
				error("Invalid jump in the indentation level (difference is " + ail::number_to_string(difference) + ")", error_message);
				return false;
			}
			else if(expected_indentation && difference != 1)
			{
				error("Expected an indentation of 1", error_message);
				return false;
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

			line_offset++;
		}

		return true;
	}

	void interpreter::error(std::string const & message, std::string & output)
	{
		output = "Line " + ail::number_to_string(lines[line_offset].line) + ": " + message;
	}
}
