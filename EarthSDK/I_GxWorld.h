#pragma once

#include <Windows.h>

class I_GxWorld
{
public:
	virtual void Display()=0;
};

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) I_GxWorld* GxWorld_CreateWorldObject(HWND hwnd);

#ifdef __cplusplus
}
#endif