#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <string>
#include <sys/stat.h>

// TODO: add different types of injection + better command syntax

inline bool fileExists(const std::string &path) {
  struct stat buffer;
  return(stat(path.c_str(), &buffer) == 0);
}

bool loadLibraryAInjection(const char* dllpath, DWORD pid) {
  HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  LPVOID alloc = VirtualAllocEx(process, 0, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(process, alloc, dllpath, strlen(dllpath) + 1, 0);
  HANDLE loadThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), alloc, 0, 0);
  WaitForSingleObject(process, INFINITE);
  VirtualFreeEx(process, alloc, strlen(dllpath) + 1, MEM_RELEASE);
  if (process) {
    CloseHandle(process);
    std::cout << "[CreamInjector] > Sucessfully closed handle." << std::endl;
  }
  return true;
}

int main(int argc, char** argv) { 
  if (argc != 4) {
    std::cout << "[CreamInjector] > ARG1: loadlibrarya ARG2: Path to DLL C:\\class.dll ARG3: Name of Process Untitled - Notepad" << std::endl;
  } else {
    std::string typeofInjection = argv[1];
    DWORD pid;
    LPCSTR processName = argv[3];
    HWND hwnd = FindWindowA(0, (processName));
    GetWindowThreadProcessId(hwnd, &pid);
    const char* dllpath = argv[2];

    if (!fileExists(dllpath) || !hwnd) {
      std::cout << "[CreamInjector] > Couldn't find path or ProcessID" << std::endl;
      return 1;
    } else {
      if (typeofInjection == "loadlibrarya") {
        std::cout << "[CreamInjector] > Performing LoadLibraryA Injection" << std::endl;
        loadLibraryAInjection(dllpath, pid);
      } else if (typeofInjection == "test") {
        std::cout << "test" << std::endl;
      }
    }
  }
  return 0;
}
