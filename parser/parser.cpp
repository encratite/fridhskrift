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
		symbol_tree_node * & new_node_pointer = current_node->children[name];
		new_node_pointer = new symbol_tree_node(symbol_type);
		symbol_tree_node & new_node = *new_node_pointer;
		std::cout << "Its parent is " << (void *)current_node << std::endl;
		new_node.parent = current_node;
		current_node = new_node_pointer;
		std::cout << "current_node = " << (void *)new_node_pointer << std::endl;
		return new_node;
	}

	void parser::process_body(executable_units * output, bool increment)
	{
		if(increment)
		{
			line_offset++;
			indentation_level++;

			std::cout << "process_body" << std::endl;
		}

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
				end = process_line(&new_unit);
				output->push_back(new_unit);
			}
			if(end)
			{
				if(indentation_level == 0 && line_offset != line_end - 1)
					error("Internal error: Invalid indentation level calculated");

				indentation_level--;

				if(is_class)
					nested_class_level--;
				
				break;
			}
		}
	}

	bool parser::process_class()
	{
		lexeme_container & lexemes = get_lexemes();
		if(!(lexemes.size() == 2 && lexemes[0].type == lexeme_type::class_operator && lexemes[1].type == lexeme_type::name))
			return false;

		name_collision_check();

		add_name(symbol::class_symbol);

		process_body(0);

		scope_up();

		return true;
	}

	bool parser::process_function(function * output)
	{
		lexeme_container & lexemes = get_lexemes();
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
			current_function = add_name(symbol::function).function_pointer;

		for(std::size_t i = 2, end = lexemes.size(); i < end; i++)
			current_function->arguments.push_back(*lexemes[i].string);

		process_body(&current_function->body);

		scope_up();

		return true;
	}

	void parser::process_offset_atomic_statement(parse_tree_node & output, std::size_t offset)
	{
		lexeme_container & lexemes = get_lexemes();
		parse_tree_nodes nodes;
		process_atomic_statement(lexemes, offset, nodes);
		output = nodes[0];

		line_offset++;
		std::cout << "process_offset_atomic_statement" << std::endl;
	}

	void parser::process_composite_term(parse_tree_node & output)
	{
		process_offset_atomic_statement(output, 1);
	}

	bool parser::process_line(executable_unit * output)
	{
		line_of_code & current_line = lines[line_offset];
		std::cout << "Line " << current_line.line << ": " << current_line.indentation_level << std::endl;

		if(current_line.indentation_level > indentation_level)
			error("Unexpected increase in the indentation level (" + ail::number_to_string(indentation_level) + " to " + ail::number_to_string(current_line.indentation_level) + ")");

		if(!process_class() && !process_function())
		{
			if(output)
				process_statement(*output);
			else
				error("Regular statements need to be placed within functions");
		}

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
			line_end = lines.size();

			process_body(&target_module.entry_function.body, false);

			return true;
		}
		catch(ail::exception & exception)
		{
			error_message_output = module_name + ": " + exception.get_message();
			return false;
		}
	}

	lexeme_container & parser::get_lexemes()
	{
		return lines[line_offset].lexemes;
	}

	void parser::error(std::string const & message)
	{
		throw ail::exception("Line " + ail::number_to_string(lines[line_offset].line) + ": " + message);
	}	

	void parser::single_lexeme_error(std::string const & message, std::size_t offset)
	{
		lexeme_container & lexemes = get_lexemes();
		error(message + " (\"" + lexemes[offset].to_string() + "\")");
	}

	void parser::double_lexeme_error(std::string const & message, std::size_t offset)
	{
		lexeme_container & lexemes = get_lexemes();
		error(message + " (\"" + lexemes[offset - 1].to_string() + "\", \"" + lexemes[offset].to_string() + "\")");
	}

	void parser::scope_up()
	{
		std::cout << "current_node: " << (void *)current_node << " -> " << current_node->parent << std::endl;
		current_node = current_node->parent;
	}
}
