#pragma once
#include"Token.h"
#include"FunctionRegistry.h"
#include<vector>

class Evaluator {
public:
	static double eval(const std::vector<RpnItem>& rpn, const FunctionRegistry& reg);
};