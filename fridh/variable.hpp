#include <string>
#include <vector>
#include <map>
#include <ail/types.hpp>
#include <ail/exception.hpp>
#include <fridh/construction.hpp>

namespace fridh
{
	namespace variable_type_identifier
	{
		enum type
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
	struct function;

	typedef variable_type_identifier::type variable_type;

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

#define DECLARE_UNARY_OPERATOR(name) void name(variable & output) const;
#define DECLARE_BINARY_OPERATOR(name) void name(variable const & argument, variable & output) const;

		DECLARE_BINARY_OPERATOR(addition)
		DECLARE_BINARY_OPERATOR(subtraction)
		DECLARE_BINARY_OPERATOR(multiplication)
		DECLARE_BINARY_OPERATOR(division)
		DECLARE_BINARY_OPERATOR(modulo)

		DECLARE_BINARY_OPERATOR(negation)

		DECLARE_BINARY_OPERATOR(less_than)
		DECLARE_BINARY_OPERATOR(less_than_or_equal)
		DECLARE_BINARY_OPERATOR(greater_than)
		DECLARE_BINARY_OPERATOR(greater_than_or_equal)
		DECLARE_BINARY_OPERATOR(not_equal)
		DECLARE_BINARY_OPERATOR(equal)

		DECLARE_UNARY_OPERATOR(logical_not)

		DECLARE_BINARY_OPERATOR(logical_and)
		DECLARE_BINARY_OPERATOR(logical_or)

		DECLARE_BINARY_OPERATOR(shift_left)
		DECLARE_BINARY_OPERATOR(shift_right)

		DECLARE_BINARY_OPERATOR(binary_and)
		DECLARE_BINARY_OPERATOR(binary_or)
		DECLARE_BINARY_OPERATOR(binary_xor)

		DECLARE_UNARY_OPERATOR(binary_not)

		DECLARE_UNARY_OPERATOR(negation)

#undef DECLARE_UNARY_OPERATOR
#undef DECLARE_BINARY_OPERATOR

		bool operator==(variable const & other) const;
		bool operator!=(variable const & other) const;
		bool operator<(variable const & other) const;

		variable & operator=(variable const & other);

		void copy(variable const & other);
		void destroy();

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

		bool is_floating_point_operation(variable const & argument) const;
		bool is_integer_type() const;
		bool is_numeric_type() const;
		bool is_zero() const;

		types::floating_point_value get_floating_point_value() const;
		std::string get_string_representation() const;
		bool get_boolean_value() const;

		bool array_addition(variable const & argument, variable & output) const;
		bool string_addition(variable const & argument, variable & output) const;

		bool array_equality(variable const & other) const;
		bool map_equality(variable const & other) const;

		uword array_hash(uword previous_hash) const;
		uword map_hash(uword previous_hash) const;
		uword hash(uword previous_hash = 0) const;
	};

	std::string get_type_string(variable_type type);

	void unary_argument_type_error(std::string const & operation, variable_type type);
	void binary_argument_type_error(std::string const & operation, variable_type left, variable_type right);
}
