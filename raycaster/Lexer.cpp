#include "Lexer.h"

Lexer::Lexer()
	:  content(""), content_len(0), cursor(0), line(0), anchor(0), bol(0)
{

}

void Lexer::init(std::string c, std::size_t len)
{
	content = std::move(c);
	content_len = len;
}


char Lexer::eat_char()
{
	char x = content[cursor];
	cursor += 1;
	if (x == '\n') {
		line += 1;
		bol = cursor;
	} 
	return x;
}

void Lexer::trim_left()
{
	while (cursor < content_len && std::isspace(static_cast<unsigned char>(content[cursor]))) {
		eat_char();
	}
}

bool Lexer::is_symbol(char x)
{
	return std::isalnum(static_cast<unsigned char>(x)) || x == '.' || x == '_';
}

Token Lexer::next_token()
{
	Token token;

	trim_left();

	if (cursor >= content_len) return token;

	if (is_symbol(content[cursor])) {
		while (cursor < content_len && is_symbol(content[cursor])) {
			token.text += eat_char();
			token.text_len += 1;
		}

		auto kind = token_kinds.find(token.text);
		if (kind != token_kinds.end()) {
			token.kind = kind->second;
		} else {
			token.kind = Token_Kind::ARGUMENT;
		}

		return token;
	}

	token.kind = Token_Kind::UNKNOWN;
	return token;
}
