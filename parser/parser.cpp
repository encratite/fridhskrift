#include <ail/file.hpp>
#include <ail/string.hpp>
#include <fridh/parser.hpp>
#include <fridh/lexer.hpp>

namespace fridh
{
	parser::parser():
		running(false)
	{
	}

	bool parser::process_module(std::string const & path, std::string const & name, module & output, std::string & error_message)
	{
		std::string content;
		if(!ail::read_file(path, content))
		{
			error_message = "Unable to read file \"" + path + "\"";
			return false;
		}

		module module_output;
		bool success = translate_data(module_output, content, name, error_message);
		if(!success)
			return false;

		return true;
	}

	bool parser::name_is_used(std::string const & name)
	{
		return current_node->exists(name);
	}

	std::string const & parser::get_declaration_name()
	{
		return *lines[line_offset].lexemes[1].string;
	}

	void parser::name_collision_check()
	{
		std::string const & name = get_declaration_name();
		if(name_is_used(name))
			error("Name \"" + name + "\" has already been used by another function or class in the current scope");
	}

	symbol_tree_node & parser::add_name(symbol::type symbol_type)
	{
		std::string const & name = get_declaration_name();
		symbol_tree_node & new_node = *current_node->children[name];
		new_node = symbol_tree_node(symbol_type);
		new_node.parent = current_node;
		current_node = &new_node;
		return new_node;
	}

	void parser::process_body(executable_units * output)
	{
		line_offset++;
		indentation_level++;

		bool is_class = (output == 0);

		if(is_class)
			nested_class_level++;

		while(line_offset < line_end)
		{
			bool end;
			if(is_class)
				end = process_line(0);
			else
			{
				executable_unit new_unit;
				process_line(&new_unit);
				output->push_back(new_unit);
			}
			if(end)
			{
				if(indentation_level > 0)
					indentation_level--;
				if(is_class)
					nested_class_level--;
				current_node = current_node->parent;
				break;
			}
		}
	}

	bool parser::process_class()
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() == 2 && lexemes[0].type == lexeme_type::class_operator && lexemes[1].type == lexeme_type::name))
			return false;

		name_collision_check();

		add_name(symbol::class_symbol);

		process_body(0);
		return true;
	}

	bool parser::process_function(function * output)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(!(lexemes.size() >= 2 && lexemes[0].type == lexeme_type::function_declaration))
			return false;

		for(std::size_t i = 1, end = lexemes.size(); i < end; i++)
			if(lexemes[i].type != lexeme_type::name)
				error("Encountered an invalid lexeme type in a function declaration - at this point only names are permitted");

		name_collision_check();

		function * current_function;
		if(output)
			current_function = output;
		else
			current_function = add_name(symbol::class_symbol).function_pointer;

		for(std::size_t i = 2, end = lexemes.size(); i < end; i++)
			current_function->arguments.push_back(*lexemes[i].string);

		process_body(&current_function->body);
		return true;
	}

	void parser::process_offset_atomic_statement(parse_tree_node & output, std::size_t offset)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		parse_tree_nodes nodes;
		process_atomic_statement(lexemes, offset, nodes);
		output = nodes[0];
		line_offset++;
	}

	void parser::process_composite_term(parse_tree_node & output)
	{
		process_offset_atomic_statement(output, 1);
	}

	bool parser::process_line(executable_unit * output)
	{
		line_of_code & current_line = lines[line_offset];
		if(current_line.indentation_level > indentation_level)
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
		return next_line.indentation_level < indentation_level;
	}

	bool parser::translate_data(module & target_module, std::string const & data, std::string const & module_name, std::string & error_message_output)
	{
		try
		{
			lines = lines_of_code();
			lexer current_lexer(data, lines);

			if(!current_lexer.parse(error_message_output))
				return false;

			current_node = &target_module.symbols;
			indentation_level = 0;
			nested_class_level = 0;
			line_offset = 0;

			process_function(&target_module.entry_function);

			return true;
		}
		catch(ail::exception & exception)
		{
			error_message_output = exception.get_message();
			return false;
		}
	}

	void parser::error(std::string const & message)
	{
		throw ail::exception("Line " + ail::number_to_string(lines[line_offset].line) + ": " + message);
	}
}
