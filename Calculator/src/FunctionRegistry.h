#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<stdexcept>
#include"../include/PluginAPI.h"

class FunctionRegistry
{
public:
	bool registerFunction(const PluginFunction& f); //добавляет новую функцию в реестр
	bool has(const std::string& name) const; //проверяет, зарегистрирована ли функция по имени
	const PluginFunction& get(const std::string& name) const; //возвращает ссылку на структуру PluginFunction по имени
	double call(const std::string& name, const std::vector<double>& args) const; //вызывает зарегистрированную функцию с переданными вргументами
	std::vector<std::string> list() const;
	
private:
	std::unordered_map<std::string, PluginFunction> map_;
};