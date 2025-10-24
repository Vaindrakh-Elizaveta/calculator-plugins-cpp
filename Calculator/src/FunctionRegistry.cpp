#include"FunctionRegistry.h"

bool FunctionRegistry::registerFunction(const PluginFunction& f)
{
	if (!f.name || !*f.name || f.arity < 0 || !f.fn) return false;

	return map_.emplace(std::string(f.name), f).second;
}

bool FunctionRegistry::has(const std::string& name) const
{
	return map_.find(name) != map_.end();
}

const PluginFunction& FunctionRegistry::get(const std::string& name) const
{
	auto it = map_.find(name);
	if (it == map_.end()) throw std::runtime_error("Unrnown function: " + name);

	return it->second;
}

double FunctionRegistry::call(const std::string& name, const std::vector<double>& args) const
{
	const auto& f = get(name);
	if (f.arity != static_cast<int>(args.size()))
		throw std::runtime_error("Arity mismatch for " + name);

	return f.fn(args.data(), static_cast<int>(args.size()));
}

std::vector<std::string> FunctionRegistry::list() const
{
	std::vector<std::string> out;
	out.reserve(map_.size());
	for (auto& [k, _] : map_) out.push_back(k);

	return out;
}

