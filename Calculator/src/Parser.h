#pragma once
#include"Token.h"
#include"FunctionRegistry.h"
#include<vector>
#include<string>

//преобразует поток токенов от лексера в RPN (обратную польскую запись)
class Parser {
public:
	Parser(const std::vector<Token>& toks, const FunctionRegistry& reg);

	std::vector<RpnItem> toRpn();

private:
	const Token& at(size_t k) const; //безопасный доступ к токену
	static int precedence(TokKind op); //приоритет оператора
	static bool rightAssoc(TokKind op); //правая ассоциативность?
	static bool isOperator(TokKind k); //это один из + - * / ^ ?
	static bool isUnaryContext(TokKind prev); //контекст для унарного + -

	const std::vector<Token>& t_;
	const FunctionRegistry reg_;
};