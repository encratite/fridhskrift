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

	match_result::type interpreter::read_class()
	{
		std::vector<lexeme> lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() == 2 && lexemes[0].type == lexeme_type::lexeme_type_class_operator && lexemes[1].type == lexeme_type::lexeme_type_name))
			return match_result::no_match;

		std::string const & name = *lexemes[1].string;

		frith::symbol_tree_node * node;
		frith::symbol_tree_entity * entity;

		symbol_tree_node & class_parent_node = *current_node;

		if(class_parent_node.exists(name, node, entity))
		{
			error_message = error("Name \"" + name + "\" has already been used by another function or class in the current scope");
			return match_result::error;
		}

		symbol_tree_node & class_node = class_parent_node.entities[name];
		class_node = symbol_tree_node(symbol::class_symbol);

		indentation++;
		nested_class_level++;

		while(true)
		{
			process_line_result::type result = process_line();
			if(result == match_result::error)
				return process_line_result::error;
			else if(result == process_line_result::end_of_block)
			{
				if(indentation > 0)
					indentation--;
				nested_class_level--;
				return match_result::match;
			}
		}
	}

	match_result::type interpreter::read_function(function & current_function)
	{
	}

	bool interpreter::read_statement(function & current_function)
	{
	}

	process_line_result::type interpreter::process_line(function * active_function)
	{
		line_of_code & current_line = lines[line_offset];
		if(current_line.indentation_level > indentation)
		{
			error_message = error("Unexpected increase in the indentation level");
			return process_line_result::error;
		}

		match_result::type result = read_class();
		if(result == match_result::error)
			return process_line_result::error;
		else if(result == match_result::no_match)
		{
			if(active_function)
			{
				function & current_function = *active_function;
				result = read_function(current_function);
				if(result == match_result::error)
					return process_line_result::error;
				else if(result == match_result::no_match)
				{
					if(!read_statement(current_function))
						return process_line_result::error;
				}
			}
			else
			{
				error_message = error("Regular statements and assignments need to be placed within functions");
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

		current_node = &target_module.symbols;
		indentation_level = 0;
		nested_class_level = 0;

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
