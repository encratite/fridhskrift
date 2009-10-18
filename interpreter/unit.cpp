#include <fridh/symbol.hpp>

namespace fridh
{
	executable_unit::executable_unit():
		type(executable_unit_type::uninitialised)
	{
	}

	executable_unit::executable_unit(executable_unit const & other)
	{
		copy(other);
	}

	executable_unit::~executable_unit()
	{
		destroy();
	}

	executable_unit & executable_unit::operator=(executable_unit const & other)
	{
		destroy();
		copy(other);
		return *this;
	}

	void executable_unit::copy(executable_unit const & other)
	{
#define COPY_MEMBER(type, member_type, member) \
		case executable_unit_type::type: \
			member = new member_type(*other.member); \
			break;

		type = other.type;

		switch(other.type)
		{
			COPY_MEMBER(statement, parse_tree_node, statement_pointer)
			COPY_MEMBER(return_statement, parse_tree_node, statement_pointer)
			COPY_MEMBER(if_statement, if_statement, if_pointer)
			COPY_MEMBER(if_else_statement, if_else_statement, if_else_pointer)
			COPY_MEMBER(for_each_statement, for_each_statement, for_each_pointer)
			COPY_MEMBER(for_statement, for_statement, for_pointer)
			COPY_MEMBER(while_statement, while_statement, while_pointer)
		}

#undef COPY_MEMBER

	}

	void executable_unit::destroy()
	{

#define DELETE_MEMBER(type, member) \
			case executable_unit_type::type: \
				delete member; \
				break;

		switch(type)
		{
			DELETE_MEMBER(statement, statement_pointer)
			DELETE_MEMBER(return_statement, statement_pointer)
			DELETE_MEMBER(if_statement, if_pointer)
			DELETE_MEMBER(if_else_statement, if_else_pointer)
			DELETE_MEMBER(for_each_statement, for_each_pointer)
			DELETE_MEMBER(for_statement, for_pointer)
			DELETE_MEMBER(while_statement, while_pointer)
		}

#undef DELETE_MEMBER

	}
}
