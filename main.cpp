#include <iostream>
#include <string>
#include <vector>
#include <fridh/lexer.hpp>
#include <ail/file.hpp>
#include <fridh/interpreter.hpp>

bool perform_lexer_test(std::string const & input, std::string const & output)
{
	std::string code;
	if(!ail::read_file(input, code))
	{
		std::cout << "Unable to read input" << std::endl;
		return false;
	}

	fridh::lines_of_code lines;
	fridh::lexer lexer(code, lines);

	std::string error;
	if(!lexer.parse(error))
	{
		std::cout << "Error: " << error << std::endl;
		return false;
	}

	std::cout << "Processed " << lines.size() << " line(s) of code" << std::endl;

	std::string data = fridh::visualise_lexemes(lines);

	std::cout << "Output: " << data.size() << " byte(s)" << std::endl;

	ail::write_file(output, data);
	return true;
}

int main(int argc, char ** argv)
{
	if(argc != 4)
	{
		std::cout << argv[0] << " lexer <input> <output>" << std::endl;
		std::cout << argv[0] << " parser <input> <output>" << std::endl;
		return 1;
	}

	perform_lexer_test(argv[1], argv[2]);

	return 0;
}
