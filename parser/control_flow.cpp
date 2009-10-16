#include <ail/file.hpp>
#include <ail/string.hpp>
#include <fridh/parser.hpp>
#include <fridh/lexer.hpp>

namespace fridh
{
	bool parser::is_if_statement()
	{
		return lines[line_offset].lexemes[0].type == lexeme_type::division;
	}

	bool parser::process_if(executable_unit & output)
	{
		lexeme_container & lexemes = get_lexemes();
		if(!is_if_statement())
			return false;

		parse_tree_node conditional;
		process_composite_term(conditional);

		executable_units if_body;
		process_body(&if_body);

		bool is_if_else = false;

		if(line_offset < line_end)
		{
			lexeme_container & lexemes = get_lexemes();
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

	bool parser::process_while(executable_unit & output)
	{
		lexeme_container & lexemes = get_lexemes();
		if(lexemes[0].type != lexeme_type::while_operator)
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

	bool parser::process_for(executable_unit & output)
	{
		lexeme_container & lexemes = get_lexemes();
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
			process_composite_term(for_each_pointer->container);

			process_body(&for_each_pointer->body);
		}

		return true;
	}

	bool parser::process_return(executable_unit & output)
	{
		lexeme_container & lexemes = get_lexemes();
		if(lexemes[0].type != lexeme_type::selection_operator)
			return false;

		output.type = executable_unit_type::return_statement;
		parse_tree_node * & statement_pointer = output.statement_pointer;
		statement_pointer = new parse_tree_node;
		process_composite_term(*statement_pointer);

		return true;
	}

	void parser::process_statement(executable_unit & output)
	{
		if
		(
			!
			(
				process_if(output) ||
				process_while(output) ||
				process_for(output) ||
				process_return(output)
			)
		)
		{
			output.type = executable_unit_type::statement;
			parse_tree_node * & node = output.statement_pointer;
			node = new parse_tree_node;
			process_offset_atomic_statement(*node);
		}
	}
}
