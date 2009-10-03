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

	match_result::type interpreter::read_class()
	{
	}

	match_result::type interpreter::read_function(function & current_function)
	{
	}

	bool interpreter::read_statement(function & current_function)
	{
	}

	process_line_result::type interpreter::process_line(function & active_function)
	{
		line_of_code & current_line = lines[line_offset];
		if(current_line.indentation_level > indentation)
		{
			error = error("Unexpected increase in the indentation level");
			return process_line_result::error;
		}

		match_result::type result = read_class();
		if(result == match_result::error)
			return process_line_result::error;
		else if(result == match_result::no_match)
		{
			result = read_function(current_function);
			if(result == match_result::error)
				return process_line_result::error;
			else
			{
				if(!read_statement(active_function))
					return process_line_result::error;
			}
		}

		line_offset++;

		if(line_offset == line_end)
		{
			//end of file -> end of the module entry function block
			return process_line_result::end_of_block;
		}

		line_of_code & next_line = lines[line_offset];
		
		if(next_line.indentation_level < indentation)
		{
			//end of block
			return process_line_result::end_of_block;
		}
		else
			return process_line_result::ok;
	}

	bool interpreter::translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output)
	{
		lines = std::vector<line_of_code>();
		lexer current_lexer(data, lines, error_message);
		if(!current_lexer.parse())
			return false;

		current_node = 0;
		indentation_level = 0;
		in_a_class = false;

		while(line_offset < line_end)
		{
		}

		return true;
	}

	void interpreter::error(std::string const & message)
	{
		error_message = "Line " + ail::number_to_string(lines[line_offset].line) + ": " + message;
	}
}
