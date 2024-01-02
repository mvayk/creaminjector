#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>

int main(int argc, char** argv) { 

  if (argc != 3) {
    std::cout << "1: filepath to dll 2: name of process to inject to" << std::endl;
  } else {
    DWORD pid;
    std::string sudodllpath = argv[1];
    LPCSTR processName = argv[2];

    /*
    if (PathFileExists(dllpath.c_str())) {
      std::cout << "Nonexistent file path" << std::endl;
      return EXIT_FAILURE;
    }
    */

    HWND hwnd = FindWindowA(0, (processName));
    GetWindowThreadProcessId(hwnd, &pid);
    const char* dllpath = argv[1];

    if (pid == NULL) {
      return EXIT_FAILURE;
    } else {
      HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
      LPVOID alloc = VirtualAllocEx(process, 0, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
      WriteProcessMemory(process, alloc, dllpath, strlen(dllpath) + 1, 0);
      HANDLE loadThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), alloc, 0, 0);
      WaitForSingleObject(process, INFINITE);
      VirtualFreeEx(process, alloc, strlen(dllpath) + 1, MEM_RELEASE);
    }
  }

  return 0;
}
