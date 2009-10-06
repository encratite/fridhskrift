#include <ail/file.hpp>
#include <ail/string.hpp>
#include <frith/intermediary.hpp>
#include <frith/lexer.hpp>

namespace frith
{
	intermediary_translator::intermediary_translator():
		running(false)
	{
	}

	bool intermediary_translator::load_module(std::string const & path, std::string const & name, std::string & error_message);
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

	bool intermediary_translator::name_is_used(std::string const & name)
	{
		return current_node->exists(name);
	}

	std::string const & intermediary_translator::get_declaration_name()
	{
		return *lines[line_offset][1].string;
	}

	bool intermediary_translator::name_collision_check()
	{
		std::string const & name = get_declaration_name();
		bool output = name_is_used(name);
		if(output)
			error("Name \"" + name + "\" has already been used by another function or class in the current scope");
		return output;
	}

	symbol_tree_node & intermediary_translator::add_name(symbol::type symbol_type)
	{
		std::string const & name = get_declaration_name();
		symbol_tree_node & new_node = current_node->children[name];
		new_node = symbol_tree_node(symbol_type);
		new_node.parent = current_node;
		current_node = &new_node;
		return new_node;
	}

	match_result::type intermediary_translator::process_body(function * current_function)
	{
		indentation++;

		bool is_class = (current_function == 0);

		if(is_class)
			nested_class_level++;

		while(true)
		{
			process_line_result::type result;
			result = process_line(current_function);
			if(result == match_result::error)
				return process_line_result::error;
			else if(result == process_line_result::end_of_block)
			{
				if(indentation > 0)
					indentation--;
				if(is_class)
					nested_class_level--;
				current_node = current_node->parent;
				return match_result::match;
			}
		}
	}

	match_result::type intermediary_translator::process_class()
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() == 2 && lexemes[0].type == lexeme_type::class_operator && lexemes[1].type == lexeme_type::name))
			return match_result::no_match;

		if(name_collision_check())
			return match_result::error;

		add_name(symbol::class_symbol);

		return process_body(true);
	}

	match_result::type intermediary_translator::process_function()
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() >= 2 && lexemes[0].type == lexeme_type::function_declaration))
			return match_result::no_match;

		for(std::size_t i = 1, end = lexemes.size(); i < end; i++)
		{
			if(lexemes[i].type != lexeme_type::name)
			{
				error("Encountered an invalid lexeme type in a function declaration - at this point only names are permitted");
				return match_result::error;
			}
		}

		if(name_collision_check())
			return match_result::error;

		function & current_function = *add_name(symbol::class_symbol).function_pointer;
		for(std::size_t i = 2, end = lexemes.size(); i < end; i++)
			current_function.arguments.push_back(*lexemes[i].string);

		return process_body(false);
	}

	bool intermediary_translator::parse_statement(lexeme_container & lexemes, std::size_t offset, std::size_t end, symbol_tree_node & output)
	{
		bool got_parenthesis = false;
		for(std::size_t i = offset; i < end; i++)
		{
			lexeme & current_lexeme = lexemes[i];
			if(current_lexeme.type != lexeme_type::bracket_end)
			{
			}
			if(current_lexeme.type != lexeme_type::bracket_start)
				continue;

		}
	}

	bool intermediary_translator::process_statement(function & current_function)
	{
	}

	process_line_result::type intermediary_translator::process_line(function * active_function)
	{
		line_of_code & current_line = lines[line_offset];
		if(current_line.indentation_level > indentation)
		{
			error("Unexpected increase in the indentation level");
			return process_line_result::error;
		}

		match_result::type result = process_class();
		if(result == match_result::error)
			return process_line_result::error;
		else if(result == match_result::no_match)
		{
			if(active_function)
			{
				result = process_function();
				if(result == match_result::error)
					return process_line_result::error;
				else if(result == match_result::no_match)
				{
					function & current_function = *active_function;
					if(!process_statement(current_function))
						return process_line_result::error;
				}
			}
			else
			{
				error("Regular statements and assignments need to be placed within functions");
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

	bool intermediary_translator::translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output)
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

	void intermediary_translator::error(std::string const & message)
	{
		error_message = "Line " + ail::number_to_string(lines[line_offset].line) + ": " + message;
	}
}
