

#include <acknex.h>
#include <windows.h>

typedef int HKEY;
typedef int REGSAM;

long WINAPI RegEnumKeyEx(
	HKEY hKey,
	DWORD dwIndex,
	char *lpName, 
	long *lpcName,
	long *lpReserved, 
	char *lpClass, 
	long *lpcClass, 
	FILETIME *lpftLastWriteTime);
	
long WINAPI RegOpenKeyEx(HKEY hKey, char *lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY *phkResult);

long WINAPI RegCloseKey(HKEY hKey);