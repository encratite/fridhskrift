#include <fridh/lexer.hpp>
#include <ail/string.hpp>

namespace fridh
{	
	lexeme::lexeme():
		type(lexeme_type::uninitialised)
	{
	}

	lexeme::lexeme(lexeme const & other)
	{
		copy(other);
	}

	lexeme::lexeme(lexeme_type::type type):
		type(type)
	{
	}

	lexeme::lexeme(types::boolean boolean):
		type(lexeme_type::boolean),
		boolean(boolean)
	{
	}

	lexeme::lexeme(types::signed_integer signed_integer):
		type(lexeme_type::signed_integer),
		signed_integer(signed_integer)
	{
	}

	lexeme::lexeme(types::unsigned_integer unsigned_integer):
		type(lexeme_type::unsigned_integer),
		unsigned_integer(unsigned_integer)
	{
	}

	lexeme::lexeme(types::floating_point_value floating_point_value):
		type(lexeme_type::floating_point_value),
		floating_point_value(floating_point_value)
	{
	}

	lexeme::lexeme(std::string const & string):
		type(lexeme_type::string),
		string(new std::string(string))
	{
	}

	lexeme::~lexeme()
	{
		destroy();
	}

	std::string lexeme::to_string() const
	{
		switch(type)
		{
			case lexeme_type::name:
				return "name: " + *string;

			case lexeme_type::nil:
				return "nil";

			case lexeme_type::boolean:
				return "boolean: " + ail::bool_to_string(boolean);

			case lexeme_type::signed_integer:
				return "integer: " + ail::number_to_string(signed_integer);

			case lexeme_type::unsigned_integer:
				return "unsigned-integer: " + ail::number_to_string(unsigned_integer);

			case lexeme_type::floating_point_value:
				return "float: " + ail::number_to_string(floating_point_value);

			case lexeme_type::string:
				return "string: " + ail::replace_string(*string, "\n", "\\n");

			case lexeme_type::addition:
				return "+";

			case lexeme_type::subtraction:
				return "-";

			case lexeme_type::multiplication:
				return "*";

			case lexeme_type::division:
				return "/";

			case lexeme_type::modulo:
				return "%";

			case lexeme_type::assignment:
				return "=";

			case lexeme_type::addition_assignment:
				return "+=";

			case lexeme_type::subtraction_assignment:
				return "-=";

			case lexeme_type::multiplication_assignment:
				return "*=";

			case lexeme_type::division_assignment:
				return "/=";

			case lexeme_type::modulo_assignment:
				return "%=";

			case lexeme_type::exponentiation_assignment:
				return "**=";

			case lexeme_type::increment:
				return "++";

			case lexeme_type::decrement:
				return "--";

			case lexeme_type::exponentiation:
				return "**";

			case lexeme_type::less_than:
				return "<";

			case lexeme_type::less_than_or_equal:
				return "<=";

			case lexeme_type::greater_than:
				return ">";

			case lexeme_type::greater_than_or_equal:
				return ">=";

			case lexeme_type::not_equal:
				return "!=";

			case lexeme_type::equal:
				return "==";

			case lexeme_type::logical_not:
				return "!";

			case lexeme_type::logical_and:
				return "&";

			case lexeme_type::logical_or:
				return "|";

			case lexeme_type::shift_left:
				return "<<";

			case lexeme_type::shift_right:
				return ">>";

			case lexeme_type::binary_and:
				return "&&";

			case lexeme_type::binary_or:
				return "||";

			case lexeme_type::binary_xor:
				return "^";

			case lexeme_type::binary_not:
				return "~";

			case lexeme_type::bracket_start:
				return "bracket: start";

			case lexeme_type::bracket_end:
				return "bracket: end";

			case lexeme_type::array_start:
				return "array: start";

			case lexeme_type::array_end:
				return "array: end";

			/*
			case lexeme_type::scope_start:
				return "scope: start";

			case lexeme_type::scope_end:
				return "scope: end";
			*/

			case lexeme_type::iteration:
				return "iteration";

			case lexeme_type::iterator:
				return "iterator";

			case lexeme_type::while_operator:
				return "while";

			case lexeme_type::function_declaration:
				return "function";

			case lexeme_type::anonymous_function_declaration:
				return "anonymous function";

			case lexeme_type::class_operator:
				return "class operator";

			case lexeme_type::dot:
				return ".";

			case lexeme_type::call_operator:
				return ",";

			case lexeme_type::scope_operator:
				return ":";

			default:
				return "unknown";
		}
	}

	lexeme & lexeme::operator=(lexeme const & other)
	{
		destroy();
		copy(other);
		return *this;
	}

	void lexeme::copy(lexeme const & other)
	{
		type = other.type;
		if(is_string())
			string = new std::string(*other.string);
	}

	void lexeme::destroy()
	{
		if(is_string())
			delete string;

		type = lexeme_type::uninitialised;
	}

	bool lexeme::is_string() const
	{
		return
			type == lexeme_type::name ||
			type == lexeme_type::string
		;
	}
}
