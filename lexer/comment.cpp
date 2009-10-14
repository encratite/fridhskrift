#include <fridh/lexer.hpp>

namespace fridh
{
	void lexer::parse_comment()
	{
		std::string const
			multi_line_comment = ";;",
			nested_comment_start = ";.",
			nested_comment_end = ".;",

			multiline_comment_prefix = "In multi-line comment: ",
			nested_comment_prefix = "In nested comment: ";

		uword start_of_comment = line;

		if(string_match(multi_line_comment))
		{
			bool got_end = false;
			for(i += multi_line_comment.size(); !got_end && i < end;)
			{
				char byte = input[i];
				switch(byte)
				{
					case '\n':
						process_newline();
						continue;

					case ';':
						if(string_match(multi_line_comment))
						{
							got_end = true;
							i++;
						}
						break;
				}
				i++;
			}
			if(!got_end)
				lexer_error("Unable to find the end of a multi-line comment", start_of_comment);
		}
		else if(string_match(nested_comment_start))
		{
			uword comment_depth = 1;
			for(i += nested_comment_start.size(); comment_depth > 0 && i < end;)
			{
				char byte = input[i];
				if(byte == '\n')
				{
					process_newline();
					continue;
				}
				else if(string_match(nested_comment_start))
				{
					comment_depth++;
					i += nested_comment_start.size();
					continue;
				}
				else if(string_match(nested_comment_end))
				{
					comment_depth--;
					i += nested_comment_end.size();
					continue;
				}
				i++;
			}

			if(comment_depth != 0)
				lexer_error("Unable to find the end of a nested comment", start_of_comment);
		}
		else
		{
			std::size_t offset = input.find('\n', i);
			if(offset == std::string::npos)
				lexer_error("Unable to find the end of a multi-line comment", start_of_comment);
			i = offset;
			process_newline();
		}
	}
}
