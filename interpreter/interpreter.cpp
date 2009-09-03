#include <ail/file.hpp>
#include <frith/interpreter.hpp>
#include <frith/lexer.hpp>

namespace frith
{
	interpreter::interpreter():
		running(false)
	{
	}

	bool interpreter::execute(std::string const & data, std::string & error_message)
	{
		
	}

	bool interpreter::execute_file(std::string const & path, std::string & error_message)
	{
		std::string content;
		if(!ail::read_file(path, content))
		{
			error_message = "Unable to read file \"" + path + "\"";
			return false;
		}

		return execute(content, error_message);
	}

	bool translate_data(std::string const & data, std::string & error_message)
	{
	}
}
