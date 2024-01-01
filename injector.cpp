#include <iostream>
#include <windows.h>

int main(int argc, char** argv) {
  std::cout << "takes pid as argument" << std::endl;
  LPCSTR DLLPath = "D:\\Libraries\\test.dll";
  DWORD pid = argc;

  HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  LPVOID alloc = VirtualAllocEx(process, 0, strlen(DLLPath) + 1, MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(process, DLLPath, (LPVOID)DLLPath, strlen(DLLPath) + 1, 0);
  HANDLE loadThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), DLLPath, 0, 0);
  WaitForSingleObject(process, INFINITE);
  VirtualFreeEx(process, DLLPath, strlen(DLLPath) + 1, MEM_RELEASE);

  return 0;
}
