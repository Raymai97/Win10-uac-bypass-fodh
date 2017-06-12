#ifndef _MYWINAPI_CURRPROCUSERISADMIN_H
#define _MYWINAPI_CURRPROCUSERISADMIN_H
/* use advapi32.lib */
#include <Windows.h>

/*
	This function tests effective info, which never change
	while a user is logged-in. For example:
	If a user was Admin, and it is removed from Administrators
	group while it is still logged-in, this returns TRUE.
*/
EXTERN_C
BOOL
__stdcall
CurrProcUserIsAdmin(void);

#endif/*_MYWINAPI_CURRPROCUSERISADMIN_H*/
