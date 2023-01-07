#pragma once
#include <UnrealEngine/UE.h>


///** Credit to UnrealEngine's cute and sexy employees */

//97



BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	HMODULE GameModule = GetModuleHandleA(NULL);

	

	if (!GameModule) return FALSE;
		
	if (!Initalize(GameModule)) return FALSE;
	//GUObjectArray->ObjObjects.Log();
	//GUObjectArray->ObjObjects.FindObject("Function Engine.Canvas.K2_DrawBox");
	/*SwapVmt(
		GameWorld->OwningGameInstance->LocalPlayers->LocalPlayer->ViewportClient,
		98,
		reinterpret_cast<void*>(&PostRender),
		reinterpret_cast<void**>(&OPostRender));
	//NamePoolData->Entries.Log();*/
	Sleep(1000);

    return TRUE;
}