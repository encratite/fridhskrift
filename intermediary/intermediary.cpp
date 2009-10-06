#include <stack>
#include <ail/file.hpp>
#include <ail/string.hpp>
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

	bool process_brackets(lexeme_container & lexemes, std::size_t & i, std::size_t end, parse_tree_nodes & arguments)
	{
		std::size_t bracket_start = i;

		word bracket_level = 0;

		for(std::size_t i = offset; i < end; i++)
		{
			switch(current_lexeme.type)
			{
				case lexeme_type::bracket_start:
					if(bracket_level == 0)
						bracket_start = i;
					bracket_level++;
					break;

				case lexeme_type::bracket_start_call:
					bracket_level++;
					break;

				case lexeme_type::bracket_end:
					bracket_level--;
					if(bracket_level == 0)
					{
						parse_tree_node new_argument;
						if(!parse_statement(lexemes, bracket_start, i, new_argument, false))
							return false;
						arguments.push_back(new_argument);
						return true;
					}
					break;
			}
		}

		if(bracket_level > 0)
			return error("Unmatched opening bracket");

		return true;
	}

	void operator_resolution(parse_tree_nodes & input, parse_tree_node & output)
	{
		if(input.size() != 1)
		{
			output = input[0];
			return;
		}

		bool got_an_operator = false;
		word minimum;
		std::size_t minimum_offset;

		for(std::size_t i = 0, end = input.size(); i < end; i++)
		{
			word precedence;
			if(get_parse_tree_node_precedence(input[i], precedence))
			{
				//<= instead < so unary operators and the selection operator will be parsed from right to left
				if(!got_an_operator || precedence <= minimum)
				{
					got_an_operator = true;
					minimum = precedence;
					minimum_offset = i;
				}
			}
		}

		if(!got_an_operator)
			throw ail::exception("Failed to perform operator resolution");

		parse_tree_node & operator_node = input[minimum_offset];
		std::size_t next_offset = minimum_offset + 1;
		if(operator_node.type == parse_tree_node_type::unary_operator_node)
		{
			std::size_t argument_offset = next_offset;
			parse_tree_unary_operator_node & unary_operator_node = *operator_node.unary_operator_pointer;
			unary_operator_node.argument = input.at(argument_offset);
			input.erase(input.begin() + argument_offset);
		}
		else
		{
			parse_tree_binary_operator_node & binary_operator_node = *operator_node.binary_operator_pointer;

			parse_tree_nodes
				left_side,
				right_side;

			std::copy(input.begin(), input.begin() + minimum_offset, left_side.begin());
			std::copy(input.begin() + next_offset, input.end(), right_side.begin());

			operator_resolution(left_side, binary_operator_node.left_argument);
			operator_resolution(right_side, binary_operator_node.right_argument);

			output = operator_node;
		}
	}

	bool intermediary_translator::parse_statement(lexeme_container & lexemes, std::size_t offset, std::size_t end, parse_tree_nodes & output, bool allow_multi_statements)
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

		if(offset == end)
			return error("Empty statement in line");

		for(std::size_t i = offset; i < end; i++)
		{
			lexeme & current_lexeme = lexemes[i];

			switch(current_lexeme.type)
			{
				case lexeme_type::bracket_start:
					if(!process_brackets(lexemes, i, end, arguments))
						return false;
					set_last_group(lexeme_group::argument);
					break;

				case lexeme_type::bracket_start_call:
					//process the call
					set_last_group(lexeme_group::argument);
					break;

				case lexeme_type::bracket_end:
					return error("Unmatched closing bracket");
			}

			lexeme_group::type group;
			if(!get_lexeme_group(current_lexeme.type, group))
				return error("Invalid lexeme type in statement");

			switch(group)
			{
				case lexeme_group::argument:
				{
					if(!allow_multi_statements && !got_last_group && last_group == lexeme_group::argument)
						return error("Encountered two arguments without an operator between them");
					
					parse_tree_node argument_node;
					lexeme_to_argument_node(current_lexeme, argument_node);
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
						return error("Encountered an argument followed by an unary operator without a binary operator between them");
					add_unary_node(current_lexeme);
					break;

				case lexeme_group::binary_operator:
					if(got_last_group)
					{
						switch(last_group)
						{
							case lexeme_group::unary_operator:
								return error("Encountered a unary operator followed by a binary operator");

							case lexeme_group::binary_operator:
								if(current_lexeme.type == lexeme_type::subtraction)
									add_negation_lexeme();
								else
									return error("Encountered two sequential binary operators");
								break;
						}
					}
					else
					{
						if(current_lexeme.type == lexeme_type::subtraction)
							add_negation_lexeme();
						else
							return error("Encountered a binary operator in the beginning of a statement");
						break;
					}
					parse_tree_node binary_operator_node;
					lexeme_to_binary_operator_node(current_lexeme, binary_operator_node);
					arguments.push_back(binary_operator_node);
					break;
			}

			set_last_group(group);
		}

		if(last_group != lexeme_group::argument)
			return error("An operator is missing an argument");

		process_node_group();

		return true;
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

	bool intermediary_translator::error(std::string const & message)
	{
		error_message = "Line " + ail::number_to_string(lines[line_offset].line) + ": " + message;
		return false;
	}
}
