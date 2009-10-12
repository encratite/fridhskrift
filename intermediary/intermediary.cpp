#include <ail/file.hpp>
#include <ail/string.hpp>
#include <fridh/intermediary.hpp>
#include <fridh/lexer.hpp>

namespace fridh
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

	void intermediary_translator::name_collision_check()
	{
		std::string const & name = get_declaration_name();
		if(name_is_used(name))
			error("Name \"" + name + "\" has already been used by another function or class in the current scope");
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

	void intermediary_translator::process_body(executable_units * output)
	{
		line_offset++;
		indentation++;

		bool is_class = (output == 0);

		if(is_class)
			nested_class_level++;

		while(line_offset < line_end)
		{
			if(process_line(output))
			{
				if(indentation > 0)
					indentation--;
				if(is_class)
					nested_class_level--;
				current_node = current_node->parent;
				break;
			}
		}
	}

	bool intermediary_translator::process_class()
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() == 2 && lexemes[0].type == lexeme_type::class_operator && lexemes[1].type == lexeme_type::name))
			return false;

		name_collision_check();

		add_name(symbol::class_symbol);

		process_body(0);
		return true;
	}

	bool intermediary_translator::process_function()
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() >= 2 && lexemes[0].type == lexeme_type::function_declaration))
			return false;

		for(std::size_t i = 1, end = lexemes.size(); i < end; i++)
			if(lexemes[i].type != lexeme_type::name)
				error("Encountered an invalid lexeme type in a function declaration - at this point only names are permitted");

		name_collision_check();

		function & current_function = *add_name(symbol::class_symbol).function_pointer;
		for(std::size_t i = 2, end = lexemes.size(); i < end; i++)
			current_function.arguments.push_back(*lexemes[i].string);

		process_body(&current_function.body);
		return true;
	}

	void intermediary_translator::process_offset_atomic_statement(parse_tree_node & output, std::size_t offset)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		parse_tree_nodes nodes;
		process_atomic_statement(lexemes, offset, nodes);
		output = nodes[0];
		line_offset++;
	}

	void intermediary_translator::process_composite_term(parse_tree_node & output)
	{
		process_offset_atomic_statement(output, 1);
	}

	bool intermediary_translator::process_line(executable_unit * output)
	{
		line_of_code & current_line = lines[line_offset];
		if(current_line.indentation_level > indentation)
			error("Unexpected increase in the indentation level");

		if(!process_class())
		{
			if(output)
			{
				if(!process_function())
					process_statement(*output);
			}
			else
				error("Regular statements and assignments need to be placed within functions");
		}

		line_offset++;

		if(line_offset == line_end)
		{
			//end of file -> end of the module entry function block
			return true;
		}

		line_of_code & next_line = lines[line_offset];
		
		//end of block?
		return next_line.indentation_level < indentation;
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
		throw ail::exception("Line " + ail::number_to_string(lines[line_offset].line) + ": " + message);
	}
}
