#include <windows.h> 

HANDLE _single_instance_hMutex;
LPTSTR szMutexName = TEXT("Local\\24e7960d-4196-46e0-9e9d-4d0fe6c80853");

BOOL LockCreation() {
	_single_instance_hMutex = CreateMutex(NULL, FALSE, szMutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS ||
	       GetLastError() == ERROR_ACCESS_DENIED) {
		_single_instance_hMutex = NULL;
	}
	return _single_instance_hMutex != NULL;
}

void UnlockCreation() {
	if (_single_instance_hMutex) {
		CloseHandle(_single_instance_hMutex); //Do as late as possible.
		_single_instance_hMutex = NULL; //Good habit to be in.
	}
}
