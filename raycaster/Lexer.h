#pragma once

#include <iostream>
#include <string>
#include <map>
#include <cctype>

enum class Token_Kind
{
	END,
	UNKNOWN,

	EVENT_INTERACTION,
	EVENT_OBJECT,
	EVENT_ONCE,
	EVENT_TIMER,
	EVENT_KEYBOARD,

	COND_ANY,
	COND_NONE,
	COND_EQ,

	ACTION_SUB,
	ACTION_BREAKWALL,
	ACTION_SET,
	ACTION_FAILED,

	ARGUMENT
};

struct Token
{
	Token_Kind kind;
	std::string text;
	std::size_t text_len;
	Token() :
		kind(Token_Kind::END), text(""), text_len(0)
	{
	}
};

class Lexer
{
private:
	std::string content;
	std::size_t content_len;
	std::size_t cursor; // arbitrary point in file content
	std::size_t line; 
	std::size_t anchor;
	std::size_t bol; // beginning of line to calculate indentaion level

	const std::map<std::string, Token_Kind> token_kinds = {
		{"interaction", Token_Kind::EVENT_INTERACTION},
		{"object", Token_Kind::EVENT_OBJECT},
		{"once", Token_Kind::EVENT_ONCE},
		{"timer", Token_Kind::EVENT_TIMER},
		{"keyboard", Token_Kind::EVENT_KEYBOARD},
		{"any", Token_Kind::COND_ANY},
		{"none", Token_Kind::COND_NONE},
		{"eq", Token_Kind::COND_EQ},
		{"sub", Token_Kind::ACTION_SUB},
		{"breakwall", Token_Kind::ACTION_BREAKWALL},
		{"set", Token_Kind::ACTION_SET},
		{"failed", Token_Kind::ACTION_FAILED}
	};
public:
	Lexer();
	void init(std::string c, std::size_t len);
	char eat_char();
	void trim_left();
	bool is_symbol(char x);

	Token next_token();
};

