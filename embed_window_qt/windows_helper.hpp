#pragma once

#include <Windows.h>
#include <Winuser.h>
#include <tlhelp32.h>

#include <QString>
#include <string>
#include <vector>

#pragma comment(lib, "user32.lib")

#ifndef TA_FAILED
#define TA_FAILED 0
#endif

#ifndef TA_SUCCESS_KILL
#define TA_SUCCESS_KILL 1
#endif

#ifndef TA_SUCCESS_CLEAN
#define TA_SUCCESS_CLEAN 2
#endif

// clang-format off
namespace win_helper {

struct windows_handle_data{
    long long procecss_id;
    HWND best_handle;
};

BOOL CALLBACK FindProcessWnd(HWND hWnd, LPARAM lparam)
{
    windows_handle_data& data = *(windows_handle_data*)lparam;
    DWORD procID = 0;
    GetWindowThreadProcessId(hWnd, &procID); // procID等于参数ID
    if (procID == data.procecss_id && 0 != data.procecss_id) {

        if ((HWND)0 == GetWindow(hWnd, GW_OWNER)
            && IsWindowVisible(hWnd))
        {
            data.best_handle =hWnd; return FALSE;
        }
    }

    data.best_handle = NULL;
    return TRUE;
}

BOOL FindWinIdByProcessName(const std::string processName, windows_handle_data& data)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
    // 将从hSnapShot中抽取数据到一个PROCESSENTRY32结构中（存储快照结构体）
    auto processInfo = new PROCESSENTRY32;
    //必须设置dwSize值
    processInfo->dwSize = sizeof(PROCESSENTRY32);

    while (FALSE != Process32Next(hSnapShot, processInfo))
    {
#ifdef UNICODE
        int size = WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, NULL, 0,NULL, NULL);
        char* name = new char[size + 11];

        // 取进程名然后对比
        if (WideCharToMultiByte(CP_ACP, 0, processInfo->szExeFile, -1, name, size, NULL, NULL))
        {
            if (strstr(name, processName.c_str())) {
                int pid = processInfo->th32ProcessID;
                data.procecss_id = pid;
                BOOL bRet = EnumWindows(FindProcessWnd, (LPARAM)&data);
                if (FALSE ==bRet) {
                    delete processInfo;
                    delete[] name;
                    return TRUE;
                }
            }
        }
        delete[] name;
#else
        if (strstr(processInfo->szExeFile, processName.c_str())) {
            int pid = processInfo->th32ProcessID;
            data.procecss_id = pid;
            BOOL bRet = EnumWindows(FindProcessWnd, (LPARAM)&data);
            if (FALSE ==bRet) {
                delete processInfo;
                return TRUE;
            }
        }
#endif
    }

    delete processInfo;

    return FALSE;
}

HWND FindWinidByClassName(const QString &className, const QString &windowTitle) {
#ifdef UNICODE
    return ::FindWindow(className.toStdWString().c_str(),
                        windowTitle.toStdWString().c_str());
#else
return ::FindWindow(className.toStdString().c_str(),
                    windowTitle.toStdString().c_str());
#endif
}

///
/// \brief GetProcessIdByName 根据进程名称获取进程ID
/// \param lpProcessName 进程名称
/// \return
///
std::vector<DWORD> GetProcessIdByName(LPCTSTR lpProcessName)
{
    std::vector<DWORD> ret;
    HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        printf("\n call 'CreateToolhelp32Snapshot' failed!, GetLastError():%ld", ::GetLastError());
        return ret;
    }

    //声明进程入口对象
    PROCESSENTRY32 pe32;
    //填充进程入口对象大小
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //遍历进程列表
    ::Process32First(hSnapShot, &pe32);
    do
    {
        if (!lstrcmp(pe32.szExeFile, lpProcessName))
        {
            ret.push_back(pe32.th32ProcessID);
        }
    } while (::Process32Next(hSnapShot, &pe32));

    ::CloseHandle(hSnapShot);

    return ret;
}

struct EnumWndPara {
    DWORD processId;
    std::vector<HWND> wndIdList;
};

[[maybe_unused]] auto myEnumWindowsProc = [](HWND hWnd, LPARAM lparam)->BOOL {
    EnumWndPara& data = *(EnumWndPara*)lparam;
    DWORD processid = 0;
    GetWindowThreadProcessId(hWnd, &processid);
    if (0 != data.processId && processid == data.processId) {

        if ((HWND)0 == GetWindow(hWnd, GW_OWNER)
            && IsWindowVisible(hWnd))
        {
            data.wndIdList.push_back(hWnd);
        }
    }
    return TRUE;
};

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
{
    DWORD dwID ;

    GetWindowThreadProcessId(hwnd, &dwID) ;

    if(dwID == (DWORD)lParam)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0) ;
    }

    return TRUE ;
}

DWORD WINAPI TerminateApp(DWORD dwPID, DWORD dwTimeout )
{
    HANDLE   hProc ;
    DWORD   dwRet ;

    // 如果 法用 PROCESS_TERMINATE 权限打开进程，那么立即放弃。
    hProc = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE, dwPID);

    if(hProc == NULL)
    {
        return TA_FAILED;
    }

    // TerminateAppEnum() 将 WM_CLOSE 消息发到所有其进程ID 与 所提供的进程ID 匹配的窗口.
    EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID);

    if ( WaitForSingleObject(hProc, dwTimeout) != WAIT_OBJECT_0)
        dwRet= (TerminateProcess(hProc, 0) ? TA_SUCCESS_KILL : TA_FAILED);
    else
        dwRet = TA_SUCCESS_CLEAN;

    CloseHandle(hProc) ;

    return dwRet;
}

}   // namespace win_helper

// clang-format on
