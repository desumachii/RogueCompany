#pragma once

#include <Windows.h>
#include <Utilities/Types.h>
#include <Utilities/Log.h>

// #define WITH_CASE_PRESERVING_NAME

#include <UnrealEngine/FNamePool/FNamePool.h>
#include <UnrealEngine/UObject/CoreUObject.h>
#include <UnrealEngine/FUObjectArray/FUObjectArray.h>
#include <Utilities/PatternScanner.h> 



inline void ProcessEvent(void* Object, void* UFunction, void* Params);

#include"UnrealEngine/UE4SDK.h"





inline void(*OPostRender)(UGameViewportClient* GameViewportClient, Canvas* Canvas) = nullptr;

inline void SwapVmt(void* instance, const int32_t vmtIndex, void* hkFunc, void** outOriginalFunc)
{
	void** index = *static_cast<void***>(instance);
	if (outOriginalFunc)
		*outOriginalFunc = index[vmtIndex];

	DWORD virtualProtect;
	VirtualProtect(&index[vmtIndex], 0x8, PAGE_EXECUTE_READWRITE, &virtualProtect);
	index[vmtIndex] = hkFunc;
	VirtualProtect(&index[vmtIndex], 0x8, virtualProtect, &virtualProtect);
}


inline void PostRender(UGameViewportClient* GameViewportClient, Canvas* canvas);


inline void ProcessEvent(void* Object, void* UFunction, void* Params) {
	const auto vtable = *static_cast<void***>(Object);
	reinterpret_cast<void(*)(void*, void*, void*)>(vtable[0x42])(Object, UFunction, Params);
}


extern FNamePool* NamePoolData;
extern FUObjectArray* GUObjectArray;
extern void* EngineVersionString;
//extern UWorld* GameWorld;

bool Initalize(HMODULE GameModule);

