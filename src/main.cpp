#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)  {
    switch(fdwReason) { 
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
			// This was in the dllmain preset in vs2022
            if (lpvReserved != nullptr) {
                break; // do not do cleanup if process termination scenario
            }
            break;
    }
	
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}