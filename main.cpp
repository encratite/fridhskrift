#include <iostream>
#include <string>
#include <vector>

#include <fridh/lexer.hpp>
#include <fridh/parser.hpp>
#include <fridh/interpreter.hpp>

#include <ail/file.hpp>

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

bool perform_parser_test(std::string const & input, std::string const & output)
{
	std::string code;
	if(!ail::read_file(input, code))
	{
		std::cout << "Unable to read input" << std::endl;
		return false;
	}

	fridh::module module;
	fridh::parser parser;

	std::string error;
	if(!parser.process_module(input, "test", module, error))
	{
		std::cout << "Error: " << error << std::endl;
		return false;
	}

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

	std::string
		command = argv[1],
		input = argv[2],
		output = argv[3];

	if(command == "lexer")
		perform_lexer_test(input, output);
	else if(command == "parser")
		perform_parser_test(input, output);
	else
	{
		std::cout << "Unknown command" << std::endl;
		std::cin.get();
		return 1;
	}

	std::cin.get();
	return 0;
}
