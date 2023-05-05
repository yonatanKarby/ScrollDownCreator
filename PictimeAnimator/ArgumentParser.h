#pragma once
#include <sal.h>
#include <vector>
#include <string>

struct _Option
{
public:
	std::string Name;
	std::string Description;

	std::string Value = "";
	std::string DefualtValue = "";
	bool HasValueChanged = false;
	bool HasDefualtValue = false;
};


class ArgumentParser
{
public:
	ArgumentParser();

	_Out_ int ParseArgument(_In_ int argc, _In_ char* argv[]);

	void AddOptions(_In_ std::string name, _In_ std::string description);
	void AddOptions(_In_ std::string name, _In_ std::string description, _In_ std::string defualtValue);

	bool Exists(_In_ std::string option);
	std::string GetValue(_In_ std::string option);

private:
	std::string GetDisplayString();

	void SetValue(_In_ std::string option, _In_ std::string value);

	std::vector<_Option> _options;
};
