#pragma once
#include<string>
#include<variant>
#include<vector>

enum class TokKind {
	Number, //число
	Identifier, //имя функции (sin, pow)
	Plus, Minus, Star, Slash, Caret, //+ - * / ^
	LParen, RParen, Comma, // ( ) ,
	End //маркер конца ввода
};

struct Token {
	TokKind kind{};
	std::string text; //для чисел, идентификаторов
};

using RpnItem = std::variant<double, std::string, TokKind>;