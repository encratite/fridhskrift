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

	std::vector<fridh::line_of_code> lines;
	std::string error;
	fridh::lexer lexer(code, lines);
	if(!lexer.parse())
	{
		std::cout << "Error: " << error << std::endl;
		return false;
	}

	ail::write_file(output, fridh::visualise_lexemes(lines));
	return true;
}

int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		std::cout << argv[0] << " <input> <output>" << std::endl;
		return 1;
	}

	perform_lexer_test(argv[1], argv[2]);

	return 0;
}
