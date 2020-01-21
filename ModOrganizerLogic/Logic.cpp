#include "Logic.h"
#include "SymLinkWrapper.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <chrono>

using namespace std;
namespace fs = std::experimental::filesystem;
DWORD MOLogic::processID = 0;
wstring MOLogic::processName = L"";
HANDLE MOLogic::gameHandle = NULL;
const string BACKUP_EXTENSION = " MOBAK";

MOLogic::MOLogic(MOLogicStruct moStruct) {
	modPath = fixPath(moStruct.modPath);
	gamePath = fixPath(moStruct.gamePath);
	tempPath = gamePath.string() + BACKUP_EXTENSION;
	exePath = fixPath(moStruct.exePath);
	exeArguments = moStruct.exeArguments;
	processName = moStruct.processName;
	appID = moStruct.appID;
	if (moStruct.mods.size() == 0) {
		for (fs::directory_entry entry : fs::directory_iterator(modPath)) {
			mods.push_back(entry);
		}
	}
	else {
		for (fs::path p : moStruct.mods) {
			fs::path temp = fs::system_complete(p);
			if (p.string().find(modPath.string()) == string::npos) {
				temp = modPath.string() + "\\" + p.string();
			}
			mods.push_back(temp);
		}
	}
}

fs::path MOLogic::fixPath(fs::path path) {
	if (path.empty() || path.string() == "") {
		return path;
	}
	int index = path.string().length()-1;
	while (index > 0 && path.string().at(index) == '\\' || path.string().at(index) == '//') {
		index--;
	}
	return fs::system_complete(path.string().substr(0, index+1));
}

bool MOLogic::validData() {
	if (!fs::exists(modPath)) {
		errorOutput("Invalid Mod Path: " + modPath.string());
		return false;
	}
	if (!fs::exists(gamePath)) {
		errorOutput("Invalid Game Path: " + gamePath.string());
		return false;
	}
	if (!fs::exists(exePath) && appID == 0) {
		errorOutput("Missing AppID / Invalid Exe Path: " + exePath.string());
		return false;
	}

	if (appID != 0 && processName == L"") {
		errorOutput("Process name missing");
		return false;
	}

	if (fs::exists(tempPath)) {
		errorOutput("Error: Temporary directory already exists");
		if (confirmation("Error: Temporary directory already exists\nRun cleanup procedure?")) {
			cleanup();
		}
		else {
			return false;
		}
	}

	return true;
}

void MOLogic::start() {
	stopping = false;
	threadID = GetCurrentThreadId();
	if (!validData()) {
		errorOutput("Stopping execution");
		return;
	}

	if (!rename(gamePath, tempPath)) {
		errorOutput("Stopping execution");
		return;
	}
	minorOutput("Creating directory: " + gamePath.string());
	fs::create_directory(gamePath);
	
	majorOutput("Starting modRecursion");
	for (fs::path p : mods) {
		if (!fs::exists(p) || p.string().find(modPath.string()) == string::npos) {
			errorOutput("Invalid Mod Path: " + p.string());
			if (confirmation("Invalid Mod Path: " + p.string() + "\nSkip mod and continue?")) {
				continue;
			}
			else {
				errorOutput("Stopping execution");
				cleanup();
				return;
			}
		}
		if (fs::is_directory(p)) {
			modRecursion(p);
		}
		else {
			modFile(p);
		}
		if (stopping) {
			errorOutput("Stopping execution");
			cleanup();
			return;
		}
	}

	majorOutput("Starting gameRecursion");
	gameRecursion(tempPath);
	if (stopping) {
		errorOutput("Stopping execution");
		cleanup();
		return;
	}


	majorOutput("Launching Game");
	if (processName != L"") {
		startGame();
		initializeHook(0, EVENT_OBJECT_CREATE);
		if (stopping) {
			errorOutput("Stopping execution");
			cleanup();
			return;
		}
	}
	else {
		processID = startGame();
	}
	gameHandle = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, false, processID);

	majorOutput("Finished");
	//this_thread::sleep_for(chrono::milliseconds(100));
	initializeHook(processID, EVENT_OBJECT_DESTROY);
	errorOutput("Stopping execution");
	if (stopping) {
		errorOutput("Closing game");
		TerminateProcess(gameHandle, NULL);
	}
	CloseHandle(gameHandle);
	cleanup();
	return;
}

int MOLogic::getIndex(fs::path p) {
	while (p.parent_path() != modPath) {
		p = p.parent_path();
	}
	return p.string().length();
}

void MOLogic::modRecursion(fs::path dir) {
	int index = getIndex(dir);
	for (fs::directory_entry entry : fs::directory_iterator(dir)) {
		if (stopping) {
			return;
		}
		string entryString = entry.path().string();
		fs::path dest = gamePath.string() + entryString.substr(index, entryString.length());
		if (fs::is_directory(entry)) {
			if (!fs::exists(dest)) {
				minorOutput("Creating directory: " + dest.string());
				fs::create_directories(dest);
			}
			modRecursion(entry);
		}
		else {
			modFileWork(entry, dest);
		}
	}
}

void MOLogic::modFile(fs::path file) {
	int index;
	if (file.parent_path() == modPath) {
		index = modPath.string().length();
	}
	else {
		index = getIndex(file);
	}
	string fileString = file.string();
	fs::path dest = gamePath.string() + fileString.substr(index, fileString.length());
	modFileWork(file, dest);
}

void MOLogic::modFileWork(fs::path source, fs::path dest) {
	if (!fs::exists(dest.parent_path())) {
		fs::create_directories(dest.parent_path());
	}
	fileWork(source, dest);
}

void MOLogic::fileWork(fs::path source, fs::path dest) {
	if (fs::exists(dest)) {
		minorOutput("File already exists: " + source.string());
		return;
	}
	minorOutput("Creating symlink for: " + source.string());
	bool result = SymLinkWrapper::createSymLink(source.string(), dest.string());
	if (!result) {
		errorOutput("Error creating symlink for: " + source.string());
		if (!confirmation("Error creating symlink for: " + source.string() + "\nSkip file and continue?")) {
			stopping = true;
		}
	}
}

void MOLogic::gameRecursion(fs::path dir) {
	int index = tempPath.string().length();
	for (fs::directory_entry entry : fs::directory_iterator(dir)) {
		if (stopping) {
			return;
		}
		string entryString = entry.path().string();
		fs::path dest = gamePath.string() + entryString.substr(index, entryString.length());
		if (!fs::exists(dest)) {
			fileWork(entry, dest);
		}
		else {
			if (fs::is_directory(entry)) {
				gameRecursion(entry);
			}
		}
	}
}

bool MOLogic::deleteRecursion(fs::path dir) {
	if (SymLinkWrapper::isSymLink(dir.string())) {
		minorOutput("Deleting SymLink: " + dir.string());
		SymLinkWrapper::deleteSymLink(dir.string());
	}
	else {
		string s;
		if (fs::is_directory(dir)) {
			for (fs::directory_entry entry : fs::directory_iterator(dir)) {
				if (!deleteRecursion(entry)) {
					return false;
				}
			}
			s = "Folder";
		}
		else {
			s = "File";
		}
		bool b = true;
		while (b) {
			try {
				minorOutput("Deleting " + s + ": " + dir.string());
				fs::remove(dir); //need to catch exceptions
				b = false;
			}
			catch (fs::filesystem_error e) {
				errorOutput("Error deleting failed for: " + dir.string());
				b = confirmation("Error deleting failed for: " + dir.string() + " \nRetry?");
				if (!b) {
					return false;
				}
			}
		}
	}
	return true;
}

void MOLogic::initializeHook(DWORD processID, DWORD event) {
	hook = SetWinEventHook(
		event, event,  // Range of events.
		NULL,                                          // Handle to DLL.
		handleWinEvent,                                // The callback.
		processID, 0,              // Process and thread IDs of interest (0 = all)
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWinEvent(hook);
}

void CALLBACK MOLogic::handleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
	if (event == EVENT_OBJECT_DESTROY) {
		DWORD exitCode;
		bool b = GetExitCodeProcess(gameHandle, &exitCode);
		if (b && exitCode != STILL_ACTIVE) {
			PostQuitMessage(0);
		}
	}
	else if (event == EVENT_OBJECT_CREATE) {
		if ((processID = findProcessId(processName)) != 0) {
			PostQuitMessage(0);
		}
	}
}

DWORD MOLogic::findProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

DWORD MOLogic::startGame() {
	if (appID != 0) {
		wstring wstr = L"steam://rungameid/" + to_wstring(appID);
		const wchar_t* wchar = wstr.c_str();
		ShellExecute(NULL, L"open", wchar, NULL, NULL, SW_SHOWDEFAULT);
		return 0;
	}
	else {
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		string args = exePath.string() + " " + exeArguments;
		args = args.c_str();

		CreateProcessA(
			exePath.string().c_str(), // The path
			&args.at(0),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			exePath.parent_path().string().c_str(),           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure
		);
		DWORD dwProcessId = pi.dwProcessId;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return dwProcessId;
	}	
}

void MOLogic::cleanup() {
	if (fs::exists(tempPath)) {
		this_thread::sleep_for(chrono::milliseconds(1000));
		majorOutput("Starting Cleanup");
		deleteRecursion(gamePath);
		rename(tempPath, gamePath);
		majorOutput("Cleanup Finished");
	}
}

bool MOLogic::rename(fs::path source, fs::path dest) {
	bool a = true;
	while (a) {
		try {
			minorOutput("Renaming: " + source.string());
			fs::rename(source, dest);
			a = false;
		}
		catch (fs::filesystem_error e) {
			errorOutput("Error: Renaming files failed");
			a = confirmation("Error: Renaming files failed \nRetry?");
			if (!a) {
				return false;
			}
		}
	}
	return true;
}

void MOLogic::stop() {
	stopping = true;
	PostThreadMessage(threadID, WM_QUIT, NULL, NULL);
}