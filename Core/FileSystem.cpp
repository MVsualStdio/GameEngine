#include <windows.h>
#include <shlobj.h>
#include "FileSystem.h"
#include <codecvt>
#include <wincodec.h>

std::wstring FileSystem::StringToWString(std::string str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wideString = converter.from_bytes(str);
	return wideString;
}

std::string const FileSystem::RootPath() {
	std::filesystem::path currentPath = std::filesystem::current_path();
	return currentPath.parent_path().string();
}

std::string const FileSystem::HLSLPath() {
	std::string path = FileSystem::RootPath();
	return path + "/HLSL";
}

std::string const FileSystem::HLSLPath(std::string sourcePath) {
	std::string path = FileSystem::HLSLPath();
	return path + sourcePath;
}

std::wstring const FileSystem::RootWPath() {
	std::string path = FileSystem::RootPath();
	return StringToWString(path);
}

std::wstring const FileSystem::HLSLWPath() {
	std::wstring path = FileSystem::RootWPath();
	return path + StringToWString("/HLSL");
}
std::wstring const FileSystem::HLSLWPath(std::string sourcePath) {
	std::wstring path = FileSystem::HLSLWPath();
	return path + StringToWString(sourcePath);
}