#include <iostream>
#include <string>
#include <vector>
#include <frith/lexer.hpp>

int main()
{
	std::string code = "variable | 1 false\nvariable 'string'\nvariable [ 2.5 ]\n\tvariable {variable2}\n= a b";
	std::vector<frith::line_of_code> lines;
	std::string error;
	if(!frith::parse_lexemes(code, lines, error))
	{
		std::cout << "Error: " << error << std::endl;
		return 1;
	}

	std::cout << frith::visualise_lexemes(lines) << std::endl;

	return 0;
}
