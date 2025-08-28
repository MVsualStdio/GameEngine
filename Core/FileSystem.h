#pragma once

#include <filesystem>
#include <string>

class FileSystem {
public:

	static std::wstring StringToWString(std::string str);

	static std::string const RootPath();
	static std::string const HLSLPath();
	static std::string const HLSLPath(std::string sourcePath);

	static std::wstring const RootWPath();
	static std::wstring const HLSLWPath();
	static std::wstring const HLSLWPath(std::string sourcePath);
};