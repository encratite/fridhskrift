#include <ail/exception.hpp>
#include <frith/symbol.hpp>
#include <fnv.hpp>

namespace frith
{
	uword variable::array_hash(uword previous_hash) const
	{
		uword hash = previous_hash;
		for(types::vector::iterator i = array->begin(), end = array->end(); i != end; i++)
			hash = i->hash(hash);
		return hash;
	}

	uword variable::map_hash(uword previous_hash) const
	{
		uword hash = previous_hash;
		for(types::map::iterator i = map->begin(), end = map->end(); i != end; i++)
		{
			hash = i->first.hash(hash);
			hash = i->second.hash(hash);
		}
		return hash;
	}

	uword string_hash(std::string const & value, uword previous_hash)
	{
		return fnv1a_hash(value.c_str(), value.size(), previous_hash);
	}

	uword variable::hash(uword previous_hash) const
	{
		switch(type)
		{
		case variable_type_identifier::undefined:
			throw ail::exception("Attempted to calculate the hash of an undefined variable");

		case variable_type_identifier::nil:
			return string_hash("nil", previous_hash);

		case variable_type_identifier::none:
			return string_hash("none", previous_hash);

		case variable_type_identifier::boolean:
			return fnv1a_hash(&hash_pointer, sizeof(types::signed_integer), previous_hash);

		case variable_type_identifier::signed_integer:
			return fnv1a_hash(&hash_pointer, sizeof(types::signed_integer), previous_hash);

		case variable_type_identifier::unsigned_integer:
			return fnv1a_hash(&hash_pointer, sizeof(types::unsigned_integer), previous_hash);

		case variable_type_identifier::floating_point_value:
			return fnv1a_hash(&hash_pointer, sizeof(types::floating_point_value), previous_hash);

		case variable_type_identifier::string:
			return fnv1a_hash(string->c_str(), string->size(), previous_hash);

		case variable_type_identifier::array:
			return array_hash(previous_hash);

		case variable_type_identifier::map:
			return map_hash(previous_hash);

		case variable_type_identifier::function:
			return fnv1a_hash(&hash_pointer, sizeof(hash_pointer), previous_hash);

		case variable_type_identifier::object:
			throw ail::exception("Hashing for objects has not been implemented yet");
		}

		throw ail::exception("Tried to hash an object of an unknown type");
	}
}
