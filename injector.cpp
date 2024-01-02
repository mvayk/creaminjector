#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <string>
#include <sys/stat.h>

// TODO: check if dllpath exists and add different types of injection + better command syntax

inline bool fileExists(const std::string &path) {
  struct stat buffer;
  return(stat(path.c_str(), &buffer) == 0);
}

int main(int argc, char** argv) { 
  if (argc != 3) {
    std::cout << "[CreamInjector] > ARG1: Path to DLL C:\\class.dll ARG2: Name of Process Untitled - Notepad" << std::endl;
  } else {
    DWORD pid;
    LPCSTR processName = argv[2];
    HWND hwnd = FindWindowA(0, (processName));
    GetWindowThreadProcessId(hwnd, &pid);
    const char* dllpath = argv[1];

    if (!fileExists(dllpath)) {
      std::cout << "[CreamInjector] > Couldn't find path." << std::endl;
      return EXIT_FAILURE;
    }

    if (!hwnd) {
      std::cout << "[CreamInjector] > Couldn't find PID from Process Name." << std::endl;
      return EXIT_FAILURE;
    } else {
      HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
      LPVOID alloc = VirtualAllocEx(process, 0, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
      WriteProcessMemory(process, alloc, dllpath, strlen(dllpath) + 1, 0);
      HANDLE loadThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), alloc, 0, 0);
      WaitForSingleObject(process, INFINITE);
      VirtualFreeEx(process, alloc, strlen(dllpath) + 1, MEM_RELEASE);
      if (process) {
        CloseHandle(process);
        std::cout << "[CreamInjector] > Sucessfully closed handle" << std::endl;
      }
      return EXIT_SUCCESS;
    }
  }
  return 0;
}
