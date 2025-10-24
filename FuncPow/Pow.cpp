#include "pch.h"
#include"PluginAPI.h"
#include<cmath>
#include<stdexcept>

static double pow_impl(const double* args, int argc)
{
	if (argc != 2) throw std::invalid_argument("pow: arity 2 expected");
	const double a = args[0];
	const double b = args[1];

	return std::pow(a, b);
}

static const char NAME[] = "pow";
static const PluginFunction DESC{ NAME, 2, &pow_impl };

PLUGIN_API const PluginFunction* PLUGIN_CALL plugin_descriptor() {
	return &DESC;
}