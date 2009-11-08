#include <ail/file.hpp>
#include <ail/string.hpp>
#include <fridh/parser.hpp>
#include <fridh/lexer.hpp>

namespace fridh
{
	void call_check(std::size_t extremum_offset)
	{
		if(extremum_offset != 1)
			throw ail::exception("Invalid call offset encountered during operator resolution");
	}

	void parser::operator_resolution(parse_tree_nodes & input, parse_tree_node & output)
	{
		std::cout << "Performing operator resolution on " << input.size() << " node(s)" << std::endl;
		if(input.size() == 1)
		{
			output = input[0];
			return;
		}

		bool got_an_operator = false;
		word extremum;
		std::size_t extremum_offset;

		for(std::size_t i = 0, end = input.size(); i < end; i++)
		{
			parse_tree_node & current_node = input[i];
			word precedence;

			bool is_initialised_unary_node =
				current_node.type == parse_tree_node_type::unary_operator_node &&
				current_node.unary_operator_pointer->argument.type != parse_tree_node_type::uninitialised
			;

			bool is_initialised_call_node =
				current_node.type == parse_tree_node_type::call &&
				i == 0;

			if
			(
				is_initialised_unary_node ||
				is_initialised_call_node
			)
				continue;

			try
			{
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
			catch(ail::exception & exception)
			{
				error(exception.get_message());
			}
		}

		if(!got_an_operator)
			error("Failed to perform operator resolution");

		parse_tree_node & operator_node = input[extremum_offset];
		std::size_t next_offset = extremum_offset + 1;
		switch(operator_node.type)
		{
			case parse_tree_node_type::unary_operator_node:
			{
				std::size_t argument_offset;
				if(operator_node.is_post_fix())
					argument_offset = extremum_offset - 1;
				else
					argument_offset = next_offset;

				if(argument_offset >= input.size())
					error("Missing operator for unary argument");

				parse_tree_unary_operator_node & unary_operator_node = *operator_node.unary_operator_pointer;
				unary_operator_node.argument = input[argument_offset];
				input.erase(input.begin() + argument_offset);
				break;
			}
		
			case parse_tree_node_type::binary_operator_node:
			{
				parse_tree_binary_operator_node & binary_operator_node = *operator_node.binary_operator_pointer;

				if(extremum_offset == 0)
					error("Encountered a binary operator which lacks a left hand argument");
				else if(next_offset >= input.size())
					error("Encountered a binary operator which lacks a right hand argument");

				parse_tree_nodes
					left_side(input.begin(), input.begin() + extremum_offset),
					right_side(input.begin() + next_offset, input.end());

				operator_resolution(left_side, binary_operator_node.left_argument);
				operator_resolution(right_side, binary_operator_node.right_argument);

				output = operator_node;

				return;
			}

			case parse_tree_node_type::call:
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
					//it's a unary call
					operator_node.call_pointer->arguments.push_back(input[next_offset]);
					input.erase(input.end() - 1);
				}
				break;

			default:
				error("Invalid operator node type encountered during operator resolution");
		}

		operator_resolution(input, output);
	}
}
