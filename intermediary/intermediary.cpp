#include <stack>
#include <ail/file.hpp>
#include <ail/string.hpp>
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

	void operator_resolution(parse_tree_nodes & input, parse_tree_node & output)
	{
		void call_check(std::size_t extremum_offset)
		{
			if(extremum_offset != 1)
				throw ail::exception("Invalid call offset encountered during operator resolution");
		}

		if(input.size() != 1)
		{
			output = input[0];
			return;
		}

		bool got_an_operator = false;
		word extremum;
		std::size_t extremum_offset;

		for(std::size_t i = 0, end = input.size(); i < end; i++)
		{
			word precedence;
			parse_tree_node & current_node = input[i];
			if(get_parse_tree_node_precedence(current_node, precedence))
			{
				if
				(
					!got_an_operator ||
					precedence > extremum ||
					(is_right_to_left_operator(current_node) && precedence == extremum)
				)
				{
					got_an_operator = true;
					extremum = precedence;
					extremum_offset = i;
				}
			}
		}

		if(!got_an_operator)
			throw ail::exception("Failed to perform operator resolution");

		parse_tree_node & operator_node = input[extremum_offset];
		std::size_t next_offset = extremum_offset + 1;
		switch(operator_node.type)
		{
			case parse_tree_node_type::unary_operator_node:
				std::size_t argument_offset = next_offset;
				parse_tree_unary_operator_node & unary_operator_node = *operator_node.unary_operator_pointer;
				unary_operator_node.argument = input.at(argument_offset);
				input.erase(input.begin() + argument_offset);
				break;
		
			case parse_tree_node_type::binary_operator_node:
				parse_tree_binary_operator_node & binary_operator_node = *operator_node.binary_operator_pointer;

				parse_tree_nodes
					left_side,
					right_side;

				std::copy(input.begin(), input.begin() + extremum_offset, left_side.begin());
				std::copy(input.begin() + next_offset, input.end(), right_side.begin());

				operator_resolution(left_side, binary_operator_node.left_argument);
				operator_resolution(right_side, binary_operator_node.right_argument);

				output = operator_node;
				break;

			case parse_tree_node_type::call:
				//this is questionable
				call_check(extremum_offset);
				operator_node.call_pointer->function = input[0];
				input.erase(input.begin());
				break;

			case parse_tree_node_type::call_operator:
			case parse_tree_node_type::spaced_call_operator:
				call_check(extremum_offset);
				operator_node.is_call();
				operator_node.call_pointer->function = input[0];
				input.erase(input.begin());
				if(operator_node.type != parse_tree_node_type::spaced_call_operator && next_offset != input.size())
				{
					//it's an unary call
					operator_node.call_pointer->arguments.push_back(input[next_offset]);
					input.erase(input.end() - 1);
				}
				break;

			default:
				throw ail::exception("Invalid operator node type encountered during operator resolution");
		}
	}

	void intermediary_translator::process_atomic_statement(lexeme_container & lexemes, std::size_t & offset, parse_tree_nodes & output, bool allow_multi_statements, lexeme_type::type terminator)
	{
		bool got_last_group = false;
		lexeme_group::type last_group;

		parse_tree_nodes arguments;

		void set_last_group(lexeme_group::type new_last_group)
		{
			last_group = new_last_group;
			got_last_group = true;
		}

		void add_unary_node(lexeme & current_lexeme)
		{
			parse_tree_node unary_operator_node;
			lexeme_to_unary_operator_node(current_lexeme, unary_operator_node);
			arguments.push_back(unary_operator_node);
		}

		void add_negation_lexeme()
		{
			add_unary_node(lexeme(lexeme_type::negation));
		}

		void process_node_group()
		{
			parse_tree_node new_node;
			operator_resolution(arguments, new_node);
			output.push_back(new_node);
		}

		symbol_prefix::type prefix = symbol_prefix::none;

		for(std::size_t & i = offset, end = lexemes.size(); i < end; i++)
		{
			lexeme & current_lexeme = lexemes[i];

			if(current_lexeme.type == terminator)
			{
				i++;
				break;
			}
			else if(prefix != symbol_prefix::none && current_lexeme.type == lexeme_type::name)
				error("Invalid use of a symbol prefix");

			switch(current_lexeme.type)
			{
				case lexeme_type::scope_operator:
					prefix = symbol_prefix::scope_operator;
					continue;

				case lexeme_type::class_operator:
					prefix = symbol_prefix::class_operator;
					continue;

				case lexeme_type::bracket_start:
				{
					parse_tree_nodes content;
					if(got_last_group && last_group == lexeme_group::argument)
					{
						process_atomic_statement(lexemes, offset, content, true, lexeme_type::bracket_end);
						parse_tree_node call;
						call.is_call();
						call.call_pointer->arguments = content;
						arguments.push_back(call);
					}
					else
					{
						process_atomic_statement(lexemes, offset, content, false, lexeme_type::bracket_end);
						arguments.push_back(content[0]);
					}
					set_last_group(lexeme_group::argument);
					break;
				}

				case lexeme_type::bracket_end:
					error("Unmatched closing bracket");

				case lexeme_type::array_start:
				{
					parse_tree_nodes elements;
					process_atomic_statement(lexemes, offset, content, true, lexeme_type::array_end);
					arguments.push_back(parse_tree_node(elements));
					set_last_group(lexeme_group::argument);
					break;
				}

				case lexeme_type::array_end:
					error("Unmatched curled brace");

				case lexeme_type::call_operator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::call_operator));
					set_last_group(lexeme_group::call_operator);
					continue;

				case lexeme_type::spaced_call_operator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::spaced_call_operator));
					set_last_group(lexeme_group::call_operator);
					continue;

				case lexeme_type::iterator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::iterator));
					set_last_group(lexeme_group::argument);
					continue;
			}

			lexeme_group::type group;
			if(!get_lexeme_group(current_lexeme.type, group))
				error("Invalid lexeme type in statement");

			switch(group)
			{
				case lexeme_group::argument:
				{
					if(!allow_multi_statements && !got_last_group && last_group == lexeme_group::argument)
						error("Encountered two arguments without an operator between them");
					
					parse_tree_node argument_node;
					lexeme_to_argument_node(current_lexeme, argument_node);
					if(current_lexeme.type == lexeme_type::name)
					{
						argument_node.symbol_pointer->type = prefix;
						if(prefix != symbol_prefix::none)
						{
							if
							(
								argument_node.type == parse_tree_node_type::binary_operator_node &&
								argument_node.binary_operator_pointer->type == binary_operator_type::selection
							)
								error("Encountered a symbol prefix after a selection operator");
							prefix = symbol_prefix::none;
						}
					}
					arguments.push_back(argument_node);

					if(allow_multi_statements)
					{
						process_node_group();
						arguments.clear();
						got_last_group = false;
						continue;
					}
					break;
				}

				case lexeme_group::unary_operator:
					if(got_last_group && last_group == lexeme_group::argument)
						error("Encountered an argument followed by an unary operator without a binary operator between them");
					add_unary_node(current_lexeme);
					break;

				case lexeme_group::binary_operator:
					if(got_last_group)
					{
						switch(last_group)
						{
							case lexeme_group::unary_operator:
								error("Encountered a unary operator followed by a binary operator");

							case lexeme_group::binary_operator:
								if(current_lexeme.type == lexeme_type::subtraction)
									add_negation_lexeme();
								else
									error("Encountered two sequential binary operators");
								break;
						}
					}
					else
					{
						if(current_lexeme.type == lexeme_type::subtraction)
							add_negation_lexeme();
						else
							error("Encountered a binary operator in the beginning of a statement");
						break;
					}
					parse_tree_node binary_operator_node;
					lexeme_to_binary_operator_node(current_lexeme, binary_operator_node);
					arguments.push_back(binary_operator_node);
					break;
			}

			set_last_group(group);
		}

		if(!got_last_group)
			error("Empty statement");

		if(last_group != lexeme_group::argument)
			error("An operator is missing an argument");

		process_node_group();
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

	bool intermediary_translator::is_if_statement()
	{
		return lines[line_offset].lexemes[0].type == lexeme_type::division;
	}

	bool intermediary_translator::process_if(executable_unit & output)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(is_if_statement())
			return false;

		parse_tree_node conditional;
		process_composite_term(conditional);

		executable_units if_body;
		process_body(&if_body);

		bool is_if_else = false;

		if(line_offset < line_end)
		{
			lexeme_container & lexemes = lines[line_offset].lexemes;
			if(is_if_statement() && lexemes.size() == 1)
			{
				is_if_else = true;

				executable_units else_body;
				process_body(&else_body);

				output.type = executable_unit_type::if_else_statement;
				if_else_statement * & if_else_pointer = output.if_else_pointer;
				if_else_pointer = new if_else_statement;
				if_else_pointer->conditional_term = conditional;
				if_else_pointer->if_body = if_body;
				if_else_pointer->else_body = else_body;
			}
		}

		if(!is_if_else)
		{
			output.type = executable_unit_type::if_statement;
			if_statement * & if_pointer = output.if_pointer;
			if_pointer = new if_statement;
			if_pointer->conditional_term = conditional;
			if_pointer->body = if_body;
		}

		return true;
	}

	bool intermediary_translator::process_while(executable_unit & output)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(lexemes[0].type != lexeme_type::while_statement)
			return false;

		parse_tree_node conditional;
		process_composite_term(conditional);

		output.type = executable_unit_type::while_statement;
		while_statement * & while_pointer = output.while_pointer;
		while_pointer = new while_statement;
		while_pointer->conditional_term = conditional;

		process_body(&while_pointer->body);

		return true;
	}

	bool intermediary_translator::process_for(executable_unit & output)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(lexemes[0].type != lexeme_type::iteration)
			return false;

		if(lexemes.size() == 1)
		{
			//three part for

			if(lines.size() - line_offset < 4)
				throw ail::exception("Incomplete for statement");

			line_offset++;

			for(std::size_t i = line_offset, end = i + 3; i < end; i++)
			{
				if(lines[i].indentation_level != indentation_level)
					throw ail::exception("Invalid indentation level in a for statement");
			}

			output.type = executable_unit_type::for_statement;
			for_statement * & for_pointer = output.for_pointer;
			for_pointer = new for_statement;
			process_offset_atomic_statement(for_pointer->initialisation);
			process_offset_atomic_statement(for_pointer->conditional);
			process_offset_atomic_statement(for_pointer->iteration);
		}
		else
		{
			//for each statement

			output.type = executable_unit_type::for_each_statement;
			for_each_statement * & for_each_pointer = output.for_each_pointer;
			for_each_pointer = new for_each_statement;
			process_composite_term(for_each_pointer->conditional_term);

			process_body(&for_each_pointer->body);
		}

		return true;
	}

	bool intermediary_translator::process_return(executable_unit & output)
	{
		lexeme_container & lexemes = lines[line_offset].lexemes;
		if(lexemes[0].type != lexeme_type::selection_operator)
			return false;

		output.type = executable_unit_type::return_statement;
		parse_tree_node * & statement_pointer = output.statement_pointer;
		statement_pointer = new parse_tree_node;
		process_composite_term(*statement_pointer);

		return true;
	}

	void intermediary_translator::process_statement(executable_units & output)
	{
	}

	bool intermediary_translator::process_line(executable_units * output)
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
		{
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
