#include "pch.h"
#include"PluginAPI.h"
#include<cmath>
#include<stdexcept>

static double sin_impl(const double* args, int argc)
{
	if (argc != 1) throw std::invalid_argument("pow: arity 1 expected");
	
	return std::sin(args[0]);
}

static const char NAME[] = "sin";
static const PluginFunction DESC{ NAME, 1, &sin_impl };

PLUGIN_API const PluginFunction* PLUGIN_CALL plugin_descriptor() {
	return &DESC;
}