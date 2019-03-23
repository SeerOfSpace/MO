#include "SymLinkWrapper.h"
#include <experimental/filesystem>
//#include <WinNT.h>

using namespace std;
namespace fs = std::experimental::filesystem;

bool SymLinkWrapper::convertString(const std::string& instr, LPWSTR &widestr)
{
	// Assumes std::string is encoded in the current Windows ANSI codepage
	int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
	if (bufferlen == 0)
	{
		// Something went wrong. Perhaps, check GetLastError() and log.
		return false;
	}
	// Allocate new LPWSTR - must deallocate it later
	widestr = new WCHAR[bufferlen + 1];
	::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
	// Ensure wide string is null terminated
	widestr[bufferlen] = 0;
	return true;
}

bool SymLinkWrapper::createSymLink(string source, string dest) {
	DWORD a;
	LPWSTR file1, file2;
	convertString(source, file1);
	convertString(dest, file2);
	bool b = fs::is_directory(source);
	if (b) {
		a = 0x1;
	}
	else {
		a = 0x0;
	}
	bool c = CreateSymbolicLink(file2, file1, a);
	delete[] file1;
	delete[] file2;
	return c;
}

bool SymLinkWrapper::isSymLink(string dir) {
	LPWSTR file;
	convertString(dir, file);
	DWORD attributes = GetFileAttributesW(file);
	delete[] file;
	if(attributes & FILE_ATTRIBUTE_REPARSE_POINT) {
		return true;
	}
	else {
		return false;
	}
}

bool SymLinkWrapper::deleteSymLink(string dir) {
	bool b;
	LPWSTR file;
	convertString(dir, file);
	//DWORD attributes = GetFileAttributesW(file);
	if (isSymLink(dir)) {
		if (fs::is_directory(dir)) {
			b = RemoveDirectoryW(file);
		}
		else {
			b = DeleteFileW(file);
		}
	}
	else {
		b = false;
	}
	delete[] file;
	return b;
}