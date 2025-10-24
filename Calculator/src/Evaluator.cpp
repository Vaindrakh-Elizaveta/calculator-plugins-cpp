#include"Evaluator.h"
#include<stack>
#include<stdexcept>
#include<cmath>

//снятие значения со стека
static double pop(std::stack<double>& st)
{
	if (st.empty()) throw std::runtime_error("Пустой стек");
	double v = st.top();
	st.pop();
	
	return v;
}

double Evaluator::eval(const std::vector<RpnItem>& rpn, const FunctionRegistry& reg)
{
	std::stack<double> st;

	for (const auto& it : rpn)
	{
		if (std::holds_alternative<double>(it))
		{
			st.push(std::get<double>(it));
		}
		else if (std::holds_alternative<TokKind>(it))
		{
			//оператор
			TokKind op = std::get<TokKind>(it);
			double b = pop(st);
			double a = pop(st);
			switch (op)
			{
			case TokKind::Plus: st.push(a + b); break;
			case TokKind::Minus: st.push(a - b); break;
			case TokKind::Star: st.push(a * b); break;
			case TokKind::Slash: st.push(a / b); break;
			case TokKind::Caret: st.push(std::pow(a, b)); break;
			default: throw std::runtime_error("Неизвестный оператор");
			}
		}
		else
		{
			//std::string - имя функции
			const std::string& fname = std::get<std::string>(it);

			double dargc = pop(st);
			int argc = static_cast<int>(dargc);
			if (argc < 0) throw std::runtime_error("argc < 0");

			std::vector<double> args(argc);
			for (int i = argc - 1; i >= 0; --i) args[i] = pop(st); //восстановить порядок
			double res = reg.call(fname, args);
			st.push(res);
		}
	}
	if (st.size() != 1) throw std::runtime_error("Выражение некорректно");

	return st.top();
}