#include <iostream>
#include <windows.h>
#include <winuser.h>

DWORD pid;

DWORD getWindow(LPCSTR argu) {
  HWND hwnd = FindWindowA(0, (argu));
  GetWindowThreadProcessId(hwnd, &pid);

  if (hwnd) {
    std::cout << pid << std::endl;
    return pid;
  } else {
    std::cout << "Couldn't find PID" << std::endl;
  }
}

int main(LPCSTR dllpath, LPCSTR processName) {
  pid = getWindow(processName);

  std::cout << "takes pid as argument" << std::endl;

  HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  LPVOID alloc = VirtualAllocEx(process, 0, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(process, dllpath, (LPVOID)dllpath, strlen(dllpath) + 1, 0);
  HANDLE loadThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), dllpath, 0, 0);
  WaitForSingleObject(process, INFINITE);
  VirtualFreeEx(process, dllpath, strlen(dllpath) + 1, MEM_RELEASE);

  return 0;
}
