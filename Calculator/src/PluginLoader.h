#pragma once
#include<string>
#include<vector>
#include<windows.h>
#include"FunctionRegistry.h"

class PluginLoader
{
public:
	explicit PluginLoader(FunctionRegistry& registry);

	void loadAll(const std::string& folder); //загружает все dll из указанной папки
	bool loadSingle(const std::string& path); //загружает конкретный dll по пути
	void unloadAll(); //выгружает все загруженные dll

private:
	FunctionRegistry& registry_;
	std::vector<HMODULE> handles_;
};