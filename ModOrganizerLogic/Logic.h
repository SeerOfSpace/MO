#pragma once
#ifndef __Logic_H_Included__
#define __Logic_H_Included__
#define DllExport __declspec(dllexport)

#include <experimental/filesystem>
#include <vector>
#include <windows.h>

struct MOLogicStruct {
	std::experimental::filesystem::path modPath{};
	std::experimental::filesystem::path gamePath{};
	std::experimental::filesystem::path exePath{};
	std::string exeArguments;
	std::wstring processName{};
	unsigned int appID{};
	std::vector <std::experimental::filesystem::path> mods{};
};

class MOLogic {
public:
	DllExport MOLogic(MOLogicStruct);
	DllExport virtual void errorOutput(std::string) = 0;
	DllExport virtual void minorOutput(std::string) = 0;
	DllExport virtual void majorOutput(std::string) = 0;
	DllExport virtual bool confirmation(std::string) = 0;
	DllExport void start();
	DllExport void stop();

private:
	std::experimental::filesystem::path modPath;
	std::experimental::filesystem::path gamePath;
	std::experimental::filesystem::path exePath;
	std::experimental::filesystem::path tempPath;
	std::vector <std::experimental::filesystem::path> mods;
	HWINEVENTHOOK hook;
	static DWORD processID;
	static std::wstring processName;
	static HANDLE gameHandle;
	unsigned int appID;
	DWORD threadID;
	std::string exeArguments;

	int getIndex(std::experimental::filesystem::path);
	void modFileWork(std::experimental::filesystem::path, std::experimental::filesystem::path);
	void fileWork(std::experimental::filesystem::path, std::experimental::filesystem::path);
	void modRecursion(std::experimental::filesystem::path);
	void modFile(std::experimental::filesystem::path);
	void gameRecursion(std::experimental::filesystem::path);
	bool deleteRecursion(std::experimental::filesystem::path);
	bool stopping = false;
	void cleanup();
	bool validData();
	void initializeHook(DWORD, DWORD);
	static DWORD findProcessId(const std::wstring&);
	static void CALLBACK handleWinEvent(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);
	DWORD startGame();
	bool rename(std::experimental::filesystem::path source, std::experimental::filesystem::path dest);
	std::experimental::filesystem::path fixPath(std::experimental::filesystem::path path);
};

#endif