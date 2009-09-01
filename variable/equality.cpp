#include <ail/exception.hpp>
#include <frith/variable.hpp>

namespace frith
{
	bool variable::array_equality(variable const & other) const
	{
		types::vector
			& vector = *array,
			& other_vector = *other.array;

		std::size_t size = vector.size();
		if(size != vector.size())
			return false;

		for(std::size_t i = 0; i < size; i++)
		{
			if(vector[i] != other_vector[i])
				return false;
		}

		return true;
	}

	bool variable::map_equality(variable const & other) const
	{
		types::map
			& this_map = *map,
			& other_map = *other.map;

		if(this_map.size() != other_map.size())
			return false;

		for(types::map::const_iterator i = this_map.begin(), end = this_map.end(); i != end; i++)
		{
			types::map::const_iterator iterator = other_map.find(i->first);
			if(iterator == other_map.end())
				return false;

			if(i->second != iterator->second)
				return false;
		}

		return true;
	}

	bool variable::operator==(variable const & other) const
	{
		if(is_numeric_type() && other.is_numeric_type())
		{
			if(type == variable_type_identifier::floating_point_value || other.type == variable_type_identifier::floating_point_value)
				return get_floating_point_value() == other.get_floating_point_value();
			else
				return unsigned_integer == other.unsigned_integer;
		}
		else
		{
			switch(type)
			{
			case variable_type_identifier::nil:
				return type == other.type;

			case variable_type_identifier::boolean:
				return type == other.type && boolean == other.boolean;

			case variable_type_identifier::string:
				return type == other.type && *string == *other.string;

			case variable_type_identifier::array:
				return array_equality(other);

			case variable_type_identifier::map:
				return map_equality(other);

			case variable_type_identifier::function:
				throw ail::exception("Comparison of functions has not been implemented yet");

			case variable_type_identifier::signed_integer:
			case variable_type_identifier::unsigned_integer:
			case variable_type_identifier::floating_point_value:
			default:
				return false;
			}
		}
	}

	bool variable::operator!=(variable const & other) const
	{
		return !operator==(other);
	}
}
