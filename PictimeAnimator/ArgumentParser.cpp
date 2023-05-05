#include "ArgumentParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ArgumentParser::ArgumentParser()
{
	_options = std::vector<_Option>();
}

_Out_ int ArgumentParser::ParseArgument(_In_ int argc, _In_ char* argv[])
{
	if (argc == 1)
	{
		std::cout << GetDisplayString();
		return EXIT_FAILURE;
	}
	for (int optIndex = 1; optIndex < argc; optIndex++)
	{
		if (argv[optIndex][0] != '-' && argv[optIndex][1] != '-')
			return EXIT_FAILURE;
		std::string option_str = std::string(argv[optIndex]+2); // We do +1 to remove the '-'
		if (!Exists(option_str))
		{
			std::cout << "[*] ERROR flag: " << option_str << " dose not exist";
			return EXIT_FAILURE;
		}
		if (optIndex + 1 >= argc)
		{
			std::cout << "[*] ERROR missing value for flag: " << option_str;
			return EXIT_FAILURE;
		}
		SetValue(option_str, argv[++optIndex]);
	}
	for (auto option : _options)
	{
		if (!option.HasDefualtValue && !option.HasValueChanged)
		{
			std::cout << "[*] ERROR missing flag: " << option.Name;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int GetMaxStringSize(std::vector<_Option> options)
{
	int currentMax = 0;
	for (auto option : options)
	{
		if (currentMax < option.Name.size())
		{
			currentMax = option.Name.size();
		}
	}
	return currentMax;
}

std::string ArgumentParser::GetDisplayString()
{
	std::string outputMessage = "Allowed options:\n";
	int max = GetMaxStringSize(_options);
	for (auto option : _options)
	{
		std::string flagType = option.HasDefualtValue ? "(Optional) " : "(Mandatory) ";
		outputMessage += "--" + option.Name + std::string(max - option.Name.size(), ' ') + "  :  "
			+ flagType
			+ option.Description;

		if (option.HasDefualtValue)
		{
			outputMessage += " Defualt [" + option.DefualtValue + "]";
		}
		outputMessage += "\n";

	}
	return outputMessage;
}

void ArgumentParser::AddOptions(_In_ std::string name, _In_ std::string description)
{
	_Option option;
	option.Name = name;
	option.Description = description;
	option.HasDefualtValue = false;

	_options.push_back(option);
}

void ArgumentParser::AddOptions(_In_ std::string name, _In_ std::string description, _In_ std::string defualtValue)
{
	_Option option;
	option.Name = name;
	option.Description = description;
	option.DefualtValue = defualtValue;
	option.HasDefualtValue = true;

	_options.push_back(option);
}

bool ArgumentParser::Exists(std::string option)
{
	for (auto opt : _options)
	{
		if (opt.Name._Equal(option))
		{
			return true;
		}
	}
	return false;
}

std::string ArgumentParser::GetValue(std::string option)
{
	for (auto opt : _options)
	{
		if (opt.Name._Equal(option))
		{
			if (opt.HasValueChanged)
				return opt.Value;
			return opt.DefualtValue;
		}
	}
	std::cout << "[*] ERROR trying to read flag that does not exist flag: " << option;
	exit(EXIT_FAILURE);
}

void ArgumentParser::SetValue(std::string option, std::string value)
{
	for(int i=0; i<_options.size(); i++)
	{
		if (_options[i].Name._Equal(option))
		{
			_options[i].Value = value;
			_options[i].HasValueChanged = true;
			return;
		}
	}
}
