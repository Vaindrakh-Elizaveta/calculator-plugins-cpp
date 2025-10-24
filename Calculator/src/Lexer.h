#pragma once
#include<string>
#include<vector>
#include"Token.h"

class Lexer {
public:
	explicit Lexer(std::string input);
	std::vector<Token> run();

private:
	char peek() const; //текущий символ или '\0' (конец)
	char get(); //потребуить текущий символ и сдвинутьс€
	void skipSpaces(); //пропустить пробелы/табул€ции/переводы строк

	Token number();
	Token ident();

	std::string s_;
	size_t i_ = 0; //индекс текущей позиции
};