#include "pch.h"
#include <cmath>
#include <stdexcept>
#include "PluginAPI.h"

static double ln_impl(const double* args, int argc)
{
    if (argc != 1) throw std::runtime_error("ln: ожидается ровно 1 аргумент");
    const double x = args[0];

    // область определения: x > 0
    if (!(x > 0.0)) {
        throw std::domain_error("ln: область определения нарушена, x должен быть > 0");
    }

    return std::log(x);
}

static const char NAME[] = "ln";
static const PluginFunction DESC{ NAME, 1, &ln_impl };

PLUGIN_API const PluginFunction* PLUGIN_CALL plugin_descriptor() {
    return &DESC;
}