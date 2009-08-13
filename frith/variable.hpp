#pragma once

#include <string>
#include <vector>
#include <map>

namespace frith
{
	namespace variable_type
	{
		enum variable_type
		{
			undefined,
			nil,
			boolean,
			signed_integer,
			unsigned_integer,
			floating_point_value,
			string,
			array,
			map
		};
	
	}

	typedef variable_type::variable_type variable_type;

	struct unary_argument
	{
		variable & output;
		std::string & error_message;

		binary_argument(variable & output, std::string & error_message);
	};

	struct binary_argument
	{
		variable const & other;
		variable & output;
		std::string & error_message;

		binary_argument(variable const & other, variable & output, std::string & error_message);
	};

	class variable
	{
	public:
		variable();
		variable(variable const & other);
		~variable();

		void new_boolean(bool boolean);
		void new_signed_integer(long signed_integer);
		void new_unsigned_integer(unsigned long unsigned_integer);
		void new_floating_point_value(double floating_point_value);
		void new_string(std::string const & input);
		void new_array();
		void new_map();

		bool addition(binary_argument & argument);
		bool subtraction(binary_argument & argument);
		bool multiplication(binary_argument & argument);
		bool division(binary_argument & argument);
		bool modulo(binary_argument & argument);

		bool negative(unary_argument & argument);

		bool less_than(binary_argument & argument);
		bool less_than_or_equal(binary_argument & argument);
		bool greater_than(binary_argument & argument);
		bool greater_than_or_equal(binary_argument & argument);
		bool unequal(binary_argument & argument);
		bool equal(binary_argument & argument);

		bool logical_not(unary_argument & argument);

		bool logical_and(binary_argument & argument);
		bool logical_or(binary_argument & argument);

		bool shift_left(binary_argument & argument);
		bool shift_right(binary_argument & argument);

		bool binary_and(binary_argument & argument);
		bool binary_or(binary_argument & argument);
		bool binary_xor(binary_argument & argument);

		bool binary_not(unary_argument & argument);

	private:
		union
		{
			bool boolean;
			long signed_integer;
			unsigned long unsigned_integer;
			double floating_point_value;
			std::string * string;
			std::vector<variable> * array;
			std::map<variable, variable> * map;
		};

		variable_type type;
	};
}
