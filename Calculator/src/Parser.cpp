#include"Parser.h"
#include<stack>
#include<stdexcept>
#include<cstdlib>

Parser::Parser(const std::vector<Token>& toks, const FunctionRegistry& reg)
	: t_(toks), reg_(reg) {}

const Token& Parser::at(size_t k) const
{
	if (k >= t_.size()) return t_.back();

	return t_[k];
}

int Parser::precedence(TokKind op)
{
	switch (op) {
	case TokKind::Caret: return 4;
	case TokKind::Star:
	case TokKind::Slash: return 3;
	case TokKind::Plus:
	case TokKind::Minus: return 2;
	default: return 0;
	}
}

bool Parser::rightAssoc(TokKind op)
{
	return op == TokKind::Caret;
}

bool Parser::isOperator(TokKind k)
{
	return k == TokKind::Plus || k == TokKind::Minus || k == TokKind::Star || k == TokKind::Slash || k == TokKind::Caret;
}

bool Parser::isUnaryContext(TokKind prev)
{
	return prev == TokKind::End ||
		prev == TokKind::LParen || prev == TokKind::Comma ||
		prev == TokKind::Plus || prev == TokKind::Minus ||
		prev == TokKind::Star || prev == TokKind::Slash ||
		prev == TokKind::Caret;
}

//Алгоритм шантинг-ярд
std::vector<RpnItem> Parser::toRpn()
{
	std::vector<RpnItem> out;  //сюда собираем RPN
	std::stack<TokKind> ops; //стек операторов и скобок
	std::stack<std::string> funs; //стек имен функций (ожидаем закрючающуюся скобку)
	std::stack<int> argcStack; //стек счётчиков аргументов для текущей функции

	TokKind prev = TokKind::End;

	for (size_t i = 0; ; ++i)
	{
		const Token& tk = at(i);

		if (tk.kind == TokKind::Number)
		{
			//число сразу уходит в выходной поток (как double)
			out.emplace_back(std::strtod(tk.text.c_str(), nullptr));
			prev = tk.kind;
		}
		else if (tk.kind == TokKind::Identifier)
		{
			funs.push(tk.text);
			argcStack.push(0); //начальное число запятых = 0
			prev = tk.kind;
		}
		else if (tk.kind == TokKind::LParen)
		{
			//скобка всегда попадает в стек операторов как маркер
			ops.push(TokKind::LParen);
			prev = tk.kind;
		}
		else if (tk.kind == TokKind::Comma)
		{
			//выталкиваем операторы до ближайшей (
			while (!ops.empty() && ops.top() != TokKind::LParen)
			{
				out.emplace_back(ops.top());
				ops.pop();
			}
			if (ops.empty())
				throw std::runtime_error("Лишняя запятая или пропущенная '('");
			//увеличиваем счётчик аргументов текущей функции
			if (argcStack.empty())
				throw std::runtime_error("Запятая вне вызова функции");
			argcStack.top()++;
			prev = tk.kind;
		}
		else if (tk.kind == TokKind::RParen)
		{
			//выталктваем до (
			while (!ops.empty() && ops.top() != TokKind::LParen)
			{
				out.emplace_back(ops.top());
				ops.pop();
			}
			if (ops.empty())
				throw std::runtime_error("Пропущена '('");
			ops.pop(); //убрать '('

			//если на вершине есть функция - завершаем её вызов
			if (!funs.empty())
			{
				int commas = argcStack.top();
				argcStack.pop();
				//если перед ) шёл сразу ( -> аргументов нет
				int actualArgs = (prev == TokKind::LParen) ? 0 : (commas + 1);

				const std::string fname = funs.top();
				funs.pop();

				//проверка арности по реестру
				const auto& f = reg_.get(fname);
				if (f.arity != actualArgs)
				{
					throw std::runtime_error("Неверное число аргументов у функции '" + fname +
						"': ожидалось " + std::to_string(f.arity) +
						", получено " + std::to_string(actualArgs));
				}

				//В RPN кладём сначала количество аргументов (как число)
				//затем имя функции
				out.emplace_back(static_cast<double>(actualArgs));
				out.emplace_back(fname);
			}
			prev = tk.kind;
		}
		else if (isOperator(tk.kind))
		{
			if ((tk.kind == TokKind::Plus || tk.kind == TokKind::Minus) && isUnaryContext(prev))
			{
				out.emplace_back(0.0);
			}

			//правило приоритетов (шантинг-ярд)
			//выталкиваем с вершины стека все операторы выше приоритетом
			//а при равном приоритете - все левоассоциативные
			while (!ops.empty())
			{
				TokKind top = ops.top();
				if (top == TokKind::LParen) break;
				int pTop = precedence(top);
				int pCur = precedence(tk.kind);
				if (pTop > pCur || (pTop == pCur && !rightAssoc(tk.kind)))
				{
					out.emplace_back(top);
					ops.pop();
				}
				else break;
			}
			ops.push(tk.kind);
			prev = tk.kind;
		}
		else if (tk.kind == TokKind::End)
		{
			//выталктваем оставшиеся операторы
			while (!ops.empty())
			{
				if (ops.top() == TokKind::LParen)
					throw std::runtime_error("Пропущена ')'");
				out.emplace_back(ops.top());
				ops.pop();
			}
			if (!funs.empty())
				throw std::runtime_error("ропущена ')': незавершён вызов функции");
			break;
		}
		else
		{
		throw std::runtime_error("Неожиданный токен в парсере");
        }
	}

	return out;
}

