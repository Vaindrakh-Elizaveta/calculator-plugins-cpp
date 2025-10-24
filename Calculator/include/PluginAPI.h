#pragma once
#include<cstddef>

// --- Экспорт/импорт символов (один заголовок для exe и dll) ---
#if defined(_WIN32)
#ifdef PLUGIN_BUILD
#define PLUGIN_API  extern "C" __declspec(dllexport)
#else
#define PLUGIN_API  extern "C" __declspec(dllimport)
#endif
#define PLUGIN_CALL __cdecl
#else
#define PLUGIN_API  extern "C"
#define PLUGIN_CALL
#endif


using Calcfn = double(_cdecl*) (const double* args, int argc);

struct PluginFunction {
	const char* name; //имя функции в выражении (sin, pow и тд)
	int arity; //число аргументов
	Calcfn fn; //реализация
};

PLUGIN_API const PluginFunction* PLUGIN_CALL plugin_descriptor();

//extern "C" __declspec(dllimport)
//const PluginFunction * __cdecl plugin_descriptor();