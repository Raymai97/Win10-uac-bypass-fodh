#include "CurrProcUserIsAdmin.h"

static LPVOID MyAlloc(SIZE_T cb)
{
	return CoTaskMemAlloc(cb);
}

static void MyFree(LPVOID ptr)
{
	CoTaskMemFree(ptr);
}

/* FreeSid() when done */
static
PSID
MyGetAdminGroupSidAlloc(void)
{
	PSID adminSid = NULL;
	SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
	AllocateAndInitializeSid(
		&authority, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &adminSid);
	return adminSid;
}

/* MyFree() when done */
static
TOKEN_GROUPS *
MyGetTokenGroupsAlloc(
	HANDLE htok)
{
	DWORD cbGrps = 0;
	LPVOID buf = NULL;
	if (htok)
	{
		GetTokenInformation(htok, TokenGroups, NULL, 0, &cbGrps);
		if (buf = CoTaskMemAlloc(cbGrps))
		{
			GetTokenInformation(htok, TokenGroups, buf, cbGrps, &cbGrps);
		}
	}
	return buf;
}

EXTERN_C
BOOL
__stdcall
CurrProcUserIsAdmin(void)
{
	BOOL ret = FALSE;
	DWORD i = 0;
	HANDLE htok = NULL;
	TOKEN_GROUPS * pGrps = NULL;
	PSID adminSid = NULL;
	OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &htok);
	if (!htok) goto eof;
	adminSid = MyGetAdminGroupSidAlloc();
	if (!adminSid) goto eof;
	pGrps = MyGetTokenGroupsAlloc(htok);
	if (!pGrps) goto eof;
	for (i = 0; i < pGrps->GroupCount; ++i)
	{
		PSID sid = pGrps->Groups[i].Sid;
		if (EqualSid(sid, adminSid))
		{
			ret = TRUE;
			break;
		}
	}
eof:
	if (pGrps) { MyFree(pGrps); }
	if (adminSid) { FreeSid(adminSid); }
	if (htok) { CloseHandle(htok); }
	return ret;
}
