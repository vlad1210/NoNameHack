// dllmain.cpp: определяет точку входа для приложения DLL.
#include <Windows.h>
#include "CSystem.h"

EndScene hr_end_scene_orig;
HRESULT APIENTRY hr_end_scene_hook(DWORD pD3D9)
{
	CSystem c_sys;
	c_sys.vHack();
	return hr_end_scene_orig(pD3D9);
}

void vThread()
{
	Hook ph_hook = Hook();
	ph_hook.vHook(hr_end_scene_hook, hr_end_scene_orig);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)vThread, 0, 0, 0));
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

