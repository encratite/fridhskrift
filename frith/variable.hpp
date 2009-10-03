#include <string>
#include <vector>
#include <map>
#include <ail/types.hpp>

namespace frith
{
	namespace variable_type_identifier
	{
		enum variable_type
		{
			undefined,
			nil,
			none,
			boolean,
			signed_integer,
			unsigned_integer,
			floating_point_value,
			string,
			array,
			map,
			function,
			object
		};
	
	}

	class variable;

	typedef variable_type_identifier::variable_type variable_type;

	struct unary_argument
	{
		variable & output;
		std::string & error_message;

		unary_argument(variable & output, std::string & error_message);
	};

	struct binary_argument
	{
		variable const & other;
		variable & output;
		std::string & error_message;

		binary_argument(variable const & other, variable & output, std::string & error_message);
	};

	class variable;

	namespace types
	{
		typedef bool boolean;
		typedef word signed_integer;
		typedef uword unsigned_integer;
		typedef double floating_point_value;
		typedef std::string string;
		typedef std::vector<variable> vector;
		typedef std::map<variable, variable> map;
	}

	class variable
	{
	public:
		variable();
		variable(variable const & other);
		~variable();

		variable_type get_type() const;

		void nil();
		void none();
		void new_boolean(types::boolean new_boolean);
		void new_signed_integer(types::signed_integer new_signed_integer);
		void new_unsigned_integer(types::unsigned_integer new_unsigned_integer);
		void new_floating_point_value(types::floating_point_value new_floating_point_value);
		void new_string(types::string const & new_string);
		void new_array();
		void new_map();

		bool addition(binary_argument & argument) const;
		bool subtraction(binary_argument & argument) const;
		bool multiplication(binary_argument & argument) const;
		bool division(binary_argument & argument) const;
		bool modulo(binary_argument & argument) const;

		bool negative(unary_argument & argument) const;

		bool less_than(binary_argument & argument) const;
		bool less_than_or_equal(binary_argument & argument) const;
		bool greater_than(binary_argument & argument) const;
		bool greater_than_or_equal(binary_argument & argument) const;
		bool unequal(binary_argument & argument) const;
		bool equal(binary_argument & argument) const;

		bool logical_not(unary_argument & argument) const;

		bool logical_and(binary_argument & argument) const;
		bool logical_or(binary_argument & argument) const;

		bool shift_left(binary_argument & argument) const;
		bool shift_right(binary_argument & argument) const;

		bool binary_and(binary_argument & argument) const;
		bool binary_or(binary_argument & argument) const;
		bool binary_xor(binary_argument & argument) const;

		bool binary_not(unary_argument & argument) const;

		bool operator==(variable const & other) const;
		bool operator!=(variable const & other) const;
		bool operator<(variable const & other) const;

	private:
		union
		{
			types::boolean boolean;
			types::signed_integer signed_integer;
			types::unsigned_integer unsigned_integer;
			types::floating_point_value floating_point_value;
			types::string * string;
			types::vector * array;
			types::map * map;
			function * function_pointer;
			void * hash_pointer;
		};

		variable_type type;

		bool is_floating_point_operation(binary_argument & argument) const;
		bool is_integer_type() const;
		bool is_numeric_type() const;
		bool is_zero() const;

		types::floating_point_value get_floating_point_value() const;
		bool get_string_representation(std::string & output) const;
		bool get_boolean_value(bool & output) const;

		bool array_addition(binary_argument & argument) const;
		bool perform_string_conversion(std::string & output, bool & error) const;
		bool string_addition(binary_argument & argument, bool & error) const;

		bool array_equality(variable const & other) const;
		bool map_equality(variable const & other) const;

		uword array_hash(uword previous_hash) const;
		uword map_hash(uword previous_hash) const;
		uword hash(uword previous_hash = 0) const;
	};

	std::string get_type_string(variable_type type);
	std::string get_unary_argument_type_error(std::string const & operation, variable_type type);
	std::string get_binary_argument_type_error(std::string const & operation, variable_type left, variable_type right);
}
