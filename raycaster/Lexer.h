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
	EVENT_MAP,
	EVENT_TIMER,

	COND_ANY,
	COND_NONE,
	COND_EQ,
	COND_NEQ,
	COND_ACTIVE,

	ACTION_SUB,
	ACTION_BREAKWALL,
	ACTION_SET,
	ACTION_FAILED,
	ACTION_VICTORY,
	ACTION_BUILD,
	ACTION_SHOWALL,
	ACTION_REMOVE,
	
	ARGUMENT
};

struct Token
{
	Token_Kind kind;
	std::string text;
	int line;
	std::size_t text_len;
	Token() :
		kind(Token_Kind::END), line(0), text(""), text_len(0)
	{
	}
};

class Lexer
{
private:
	std::string content;
	std::size_t content_len;
	std::size_t cursor; // arbitrary point in file content
	int line; 
	std::size_t anchor;
	std::size_t bol; // beginning of line to calculate indentation level

	const std::map<std::string, Token_Kind> token_kinds = {
		{"interaction", Token_Kind::EVENT_INTERACTION},
		{"map", Token_Kind::EVENT_MAP},
		{"timer", Token_Kind::EVENT_TIMER},
		{"any", Token_Kind::COND_ANY},
		{"none", Token_Kind::COND_NONE},
		{"eq", Token_Kind::COND_EQ},
		{"neq", Token_Kind::COND_NEQ},
		{"active", Token_Kind::COND_ACTIVE},
		{"sub", Token_Kind::ACTION_SUB},
		{"breakwall", Token_Kind::ACTION_BREAKWALL},
		{"set", Token_Kind::ACTION_SET},
		{"failed", Token_Kind::ACTION_FAILED},
		{"build", Token_Kind::ACTION_BUILD},
		{"showall", Token_Kind::ACTION_SHOWALL},
		{"remove", Token_Kind::ACTION_REMOVE},
		{"victory", Token_Kind::ACTION_VICTORY},

	};
public:
	Lexer();
	void init(std::string c, std::size_t len);
	char eat_char();
	void trim_left();
	bool is_symbol(char x);

	Token next_token();
};

