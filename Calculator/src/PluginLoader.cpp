#include"PluginLoader.h"
#include<filesystem>
#include<iostream>

namespace fs = std::filesystem;

PluginLoader::PluginLoader(FunctionRegistry& registry) : registry_(registry) {}

void PluginLoader::loadAll(const std::string& folder)
{
	for (const auto& entry : fs::directory_iterator(folder))
	{
		if (entry.path().extension() == ".dll")
		{
			loadSingle(entry.path().string());
		}
	}
}

bool PluginLoader::loadSingle(const std::string& path)
{
	HMODULE lib = LoadLibraryA(path.c_str());

	if (!lib)
	{
		std::cerr << "Не удалось загрузить DLL: " << path << "\n";
		return false;
	}

	auto createFunc = reinterpret_cast<const PluginFunction * (_cdecl*)()>(
		GetProcAddress(lib, "plugin_descriptor")
		);
	if (!createFunc)
	{
		std::cerr << "В DLL нет plugin_descriptor: " << path << "\n";
		FreeLibrary(lib);
		return false;
	}

	const PluginFunction* desc = nullptr;
	try {
		desc = createFunc();
	}
	catch (...) {
		std::cerr << "plugin_descriptor() бросил исключение: " << path << "\n";
		FreeLibrary(lib);
		return false;
	}

	if (!desc || !desc->name || !*desc->name || desc->arity < 0 || !desc->fn)
	{
		std::cerr << "Неверный дескрипторв DLL: " << path << "\n";
		FreeLibrary(lib);
		return false;
	}

	if (!registry_.registerFunction(*desc))
	{
		std::cerr << "Функция '" << desc->name << "' уже зарегистрирована\n";
		FreeLibrary(lib);
		return false;
	}

	handles_.push_back(lib);
	std::cout << "[plugin] loaded " << path << " -> " << desc->name << "/" << desc->arity << "\n";

	return true;
}

void PluginLoader::unloadAll()
{
	for (HMODULE h : handles_)
	{
		FreeLibrary(h);
	}
	handles_.clear();
}
