// Calculator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include<string>
#include<vector>
#include<locale>
#include<iomanip>

#include"include/PluginAPI.h"
#include"src/FunctionRegistry.h"
#include"src/PluginLoader.h"

#include"src/Lexer.h"
#include"src/Parser.h"
#include"src/Evaluator.h"


int main()
{
    setlocale(LC_ALL, "Russian");
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try {
        //создаем реестр функций и загрузчик
        FunctionRegistry registry;
        PluginLoader loader(registry);

        //загружаем все dll из ./plugins
        const std::string pluginsDir = "./plugins";
        try {
            loader.loadAll(pluginsDir);
        }
        catch (const std::exception& e) {
            std::cerr << "[error] " << e.what() << "\n";
            std::cerr << "Запуск без плагинов невозможен.Завершаюсь.\n";
            return 4;
        }

        //Приветствие и помощь
        std::cout << "Калькулятор с плагинами. Команды:\n"
            << "  :functions  — показать доступные функции\n"
            << "  :help       — подсказка\n"
            << "  :quit       — выход\n"
            << "Примеры: 2+3*4,  -5+2,  pow(2,3),  pow(2, sin(1+1))\n\n";

        //читаем строку - токенизируем - парсим - считаем
        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line))
        {
            if (line == ":quit") break;
            if (line == ":help")
            {
                std::cout << "Поддерживаются операторы: + - * / ^, унарные +/-, скобки (),\n"
                    << "и вызовы функций из плагинов: name(arg1, arg2, ...)\n"
                    << "Примеры: 2+3*4, -5+2, pow(2,3), pow(2, sin(1+1))\n";
                continue;
            }
            if (line == ":functions")
            {
                auto names = registry.list();
                if (names.empty())
                {
                    std::cout << "(нет загруженных функций)\n";
                }
                else
                {
                    std::cout << "Загружены функции:\n";
                    for (const auto& n : names) std::cout << " - " << n << "\n";
                }
                continue;
            }

            try
            {
                //Лексер -> токены
                Lexer lex(line);
                auto toks = lex.run();

                //Парсер -> RPN
                Parser parser(toks, registry);
                auto rpn = parser.toRpn();

                //Вычислитель -> число
                double val = Evaluator::eval(rpn, registry);

                std::cout << std::setprecision(15) << "= " << val << "\n";
            }
            catch (const std::exception& e) {
                std::cerr << "[error] " << e.what() << "\n";
            }
        }

        ////печать списка функций
        //auto names = registry.list();
        //if (names.empty())
        //{
        //    std::cerr << "Плагинов не найдено. Положите .dll в" << pluginsDir << "\n";
        //    return 4;
        //}
        //std::cout << "Загружены функции: ";
        //for (size_t i = 0; i < names.size(); ++i)
        //{
        //    std::cout << names[i] << (i + 1 < names.size() ? ", " : "");
        //}
        //std::cout << "\n";

        ////простой REPL (пока без парсера)
        //std::cout << "Команды: :functions, :quit\n";
        //std::string line;
        //while (std::cout << "> " && std::getline(std::cin, line)) {
        //    if (line == ":quit") break;
        //    if (line == ":functions") {
        //        for (auto& n : registry.list()) std::cout << " - " << n << "\n";
        //        continue;
        //    }
        //    std::cout << "(парсер ещё не реализован)\n";
        //}

        //выгружаем dll перед выходом
        loader.unloadAll();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[fatal] " << e.what() << "\n";
        return 1;
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
