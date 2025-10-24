#include"Lexer.h"
#include<cctype>
#include<stdexcept>

Lexer::Lexer(std::string input) : s_(std::move(input)) {}

char Lexer::peek() const {
	return (i_ < s_.size()) ? s_[i_] : '\0';
}

char Lexer::get()
{
	return (i_ < s_.size()) ? s_[i_++] : '\0';
}

void Lexer::skipSpaces() {
	while (std::isspace(static_cast<unsigned char>(peek()))) get();
}

Token Lexer::number()
{
	//Поддерживаем: 123, 3.14, .5, 10.
	size_t start = i_;
	bool sawDigit = false;
	bool sawDot = false;

	//возможная ведущая точка
	if (peek() == '.') {
		sawDot = true;
		get();
	}

	while (true) {
		char c = peek();
		if (std::isdigit(static_cast<unsigned char>(c))) {
			sawDigit = true;
			get();
		}
		else if (c == '.' && !sawDot) {
			sawDot = true;
			get();
		} else{
			break;
		}
	}

	if (!sawDigit) {
		//строка вида "." - не число
		throw std::runtime_error("Ожидалось число после '.'");
	}

	return { TokKind::Number, s_.substr(start, i_ - start) };
}

Token Lexer::ident() {
	size_t start = i_;
	get(); //Первый символ уже буква или '_'

	while (true) {
		char c = peek();
		if (std::isalnum(static_cast<unsigned char>(c)) || c == '_')
			get();
		else
			break;
	}

	return { TokKind::Identifier, s_.substr(start, i_ - start) };
}

std::vector<Token> Lexer::run() {
	std::vector<Token> out;
	skipSpaces();

	while (char c = peek()) {
		if (std::isdigit(static_cast<unsigned char>(c))) {
			out.push_back(number());
		}
		else if (c == '.' && std::isdigit(static_cast<unsigned char>(s_[i_ + 1]))) {
			//случай .5
			out.push_back(number());
		}
		else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
			out.push_back(ident());
		}
		else {
			switch (c) {
			case '+': out.push_back({ TokKind::Plus, "+" }); get(); break;
			case '-': out.push_back({ TokKind::Minus, "-" }); get(); break;
			case '*': out.push_back({ TokKind::Star, "*" }); get(); break;
			case '/': out.push_back({ TokKind::Slash, "/" });  get(); break;
			case '^': out.push_back({ TokKind::Caret, "^" });  get(); break;
			case '(': out.push_back({ TokKind::LParen,"(" });  get(); break;
			case ')': out.push_back({ TokKind::RParen,")" });  get(); break;
			case ',': out.push_back({ TokKind::Comma, "," });  get(); break;
			default:
				throw std::runtime_error(std::string("Неожиданный символ: '") + c + "'");
			}
		}
		skipSpaces();
	}

	out.push_back({ TokKind::End, "" });
	return out;
}