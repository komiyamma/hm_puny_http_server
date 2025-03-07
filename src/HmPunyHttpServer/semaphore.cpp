#include <windows.h>
#include "semaphore.h"

HmSemaphore::HmSemaphore() {
    hSemaphore = CreateSemaphore(NULL, 1, 1, L"Global\\HmPunyHttpServer");
}


HmSemaphore::~HmSemaphore() {
	if (hSemaphore == NULL) {
		return;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(hSemaphore);
}

BOOL HmSemaphore::isSuccessWaitForOwnership() {
	if (hSemaphore == NULL) {
		return FALSE;
	}

    // ç≈ëÂÇ≈2ïbë“ã@
    DWORD waitResult = WaitForSingleObject(hSemaphore, 2000);
    if (waitResult == WAIT_OBJECT_0) {
        return TRUE;
    }
    else if (waitResult == WAIT_TIMEOUT) {
        return FALSE;
    }

    return FALSE;
}
