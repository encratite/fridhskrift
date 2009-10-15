#include <ail/file.hpp>
#include <ail/string.hpp>
#include <fridh/parser.hpp>
#include <fridh/lexer.hpp>

namespace fridh
{
	void set_last_group(lexeme_group::type new_last_group, lexeme_group::type & last_group, bool & got_last_group)
	{
		last_group = new_last_group;
		got_last_group = true;
	}

	void add_unary_node(lexeme & current_lexeme, parse_tree_nodes & arguments)
	{
		parse_tree_node unary_operator_node;
		lexeme_to_unary_operator_node(current_lexeme, unary_operator_node);
		arguments.push_back(unary_operator_node);
	}

	void add_negation_lexeme(parse_tree_nodes & arguments)
	{
		add_unary_node(lexeme(lexeme_type::negation), arguments);
	}

	void process_node_group(parse_tree_nodes & arguments, parse_tree_nodes & output)
	{
		parse_tree_node new_node;
		operator_resolution(arguments, new_node);
		output.push_back(new_node);
	}

	void parser::process_atomic_statement(lexeme_container & lexemes, std::size_t & offset, parse_tree_nodes & output, bool allow_multi_statements, lexeme_type::type terminator)
	{
		bool got_last_group = false;
		lexeme_group::type last_group;

		parse_tree_nodes arguments;

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
					set_last_group(lexeme_group::argument, last_group, got_last_group);
					break;
				}

				case lexeme_type::bracket_end:
					error("Unmatched closing bracket");

				case lexeme_type::array_start:
				{
					parse_tree_nodes elements;
					process_atomic_statement(lexemes, offset, elements, true, lexeme_type::array_end);
					arguments.push_back(parse_tree_node(elements));
					set_last_group(lexeme_group::argument, last_group, got_last_group);
					break;
				}

				case lexeme_type::array_end:
					error("Unmatched curled brace");

				case lexeme_type::call_operator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::call_operator));
					set_last_group(lexeme_group::call_operator, last_group, got_last_group);
					continue;

				case lexeme_type::spaced_call_operator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::spaced_call_operator));
					set_last_group(lexeme_group::call_operator, last_group, got_last_group);
					continue;

				case lexeme_type::iterator:
					arguments.push_back(parse_tree_node(parse_tree_node_type::iterator));
					set_last_group(lexeme_group::argument, last_group, got_last_group);
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
						process_node_group(arguments, output);
						arguments.clear();
						got_last_group = false;
						continue;
					}
					break;
				}

				case lexeme_group::unary_operator:
					if(got_last_group && last_group == lexeme_group::argument)
						error("Encountered an argument followed by an unary operator without a binary operator between them");
					add_unary_node(current_lexeme, arguments);
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
									add_negation_lexeme(arguments);
								else
									error("Encountered two sequential binary operators");
								break;
						}
					}
					else
					{
						if(current_lexeme.type == lexeme_type::subtraction)
							add_negation_lexeme(arguments);
						else
							error("Encountered a binary operator in the beginning of a statement");
						break;
					}
					parse_tree_node binary_operator_node;
					lexeme_to_binary_operator_node(current_lexeme, binary_operator_node);
					arguments.push_back(binary_operator_node);
					break;
			}

			set_last_group(group, last_group, got_last_group);
		}

		if(!got_last_group)
			error("Empty statement");

		if(last_group != lexeme_group::argument)
			error("An operator is missing an argument");

		process_node_group(arguments, output);
	}
}