#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <stdarg.h>

#include "Tools.h"

void DrawRect (LPDIRECT3DDEVICE9 Device_t, int X, int Y, int L, int H, D3DCOLOR color)
{
	D3DRECT rect = {X, Y, X+L, Y+H};
	Device_t->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0); // bei Google gibt’s näheres
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if( FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)) )
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32>>28)&0xF)<<12)|(WORD)(((colour32>>20)&0xF)<<8)|(WORD)(((colour32>>12)&0xF)<<4)|(WORD)(((colour32>>4)&0xF)<<0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for(int xy=0; xy < 8*8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}


UINT g_uiMyStride = 1;
UINT g_uiMyNumVertices = 0;
UINT g_uiMyPrimCount = 0;
bool g_bNumVerticesEnabled = false;
bool g_bPrimCountEnabled = false;

const D3DCOLOR txtPink = D3DCOLOR_ARGB(255, 255, 0, 255);
IDirect3DTexture9 *texPink;
const D3DCOLOR txtGreen = D3DCOLOR_ARGB(255, 0, 255, 0);
IDirect3DTexture9 *texGreen;
const D3DCOLOR txtRed = D3DCOLOR_ARGB(255, 255, 0, 0);
IDirect3DTexture9 *texRed;

ID3DXFont *g_pFont;

void DrawFont (int X, int Y, D3DCOLOR Color, char *format, ...)
{
	char buffer[256];
	va_list args; // deswegen: #include <cstdio>
	va_start (args, format);
	vsprintf_s(buffer, sizeof(buffer), format, args);
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	g_pFont->DrawText( NULL, buffer, -1, &FontRect, DT_NOCLIP , Color ); // Zeichnen
	va_end (args);
}

typedef HRESULT(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9);
EndScene_t pEndScene;
HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	//static bool bEinmal = true;
	//if (bEinmal)
	//{
	//	GenerateTexture(pDevice, &texPink, txtPink);
	//	GenerateTexture(pDevice, &texGreen, txtGreen);
	//	GenerateTexture(pDevice, &texRed, txtRed);

	//	D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont );
	//	bEinmal = false;
	//}
	//DrawRect(pDevice, 0, 0, 50, 50, txtPink);
	//DrawFont(10, 10, txtGreen, "MyStride: %d", g_uiMyStride);
	//DrawFont(10, 25, txtGreen, "MyNumVertices: %d | %d", g_uiMyNumVertices, g_bNumVerticesEnabled);
	//DrawFont(10, 40, txtGreen, "MyPrimCount: %d | %d", g_uiMyPrimCount, g_bPrimCountEnabled);

	////pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//if (GetAsyncKeyState(VK_INSERT)&1)
	//{
	//	g_uiMyStride++;
	//}
	//if (GetAsyncKeyState(VK_DELETE)&1)
	//{
	//	g_uiMyStride--;
	//}
	//if (GetAsyncKeyState(VK_HOME)&1)
	//{
	//	g_uiMyNumVertices++;
	//}
	//if (GetAsyncKeyState(VK_END)&1)
	//{
	//	g_uiMyNumVertices--;
	//}
	//if (GetAsyncKeyState(VK_PRIOR)&1)
	//{
	//	g_uiMyPrimCount++;
	//}
	//if (GetAsyncKeyState(VK_NEXT)&1)
	//{
	//	g_uiMyPrimCount--;
	//}
	//if (GetAsyncKeyState(0x4E)&1)
	//{
	//	g_bNumVerticesEnabled = !g_bNumVerticesEnabled;
	//}
	//if (GetAsyncKeyState(0x50)&1)
	//{
	//	g_bPrimCountEnabled = !g_bPrimCountEnabled;
	//}
	return pEndScene(pDevice);
}

typedef HRESULT(__stdcall *DrawIndexedPrimitive_t)(LPDIRECT3DDEVICE9,D3DPRIMITIVETYPE,INT,UINT,UINT,UINT,UINT);
DrawIndexedPrimitive_t pDrawIndexedPrimitive;
HRESULT __stdcall hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//UINT offsetib;
	//IDirect3DVertexBuffer9 *dummy1;
	//UINT uiStride;
	//pDevice->GetStreamSource(NULL, &dummy1, &offsetib, &uiStride);
	////Tools::add_log("Stride: %d", g_uiStride);
	//if (uiStride == g_uiMyStride) 
	//{
	//	if (g_bNumVerticesEnabled && NumVertices != g_uiMyNumVertices)
	//		goto OVERJUMP;
	//	if (g_bPrimCountEnabled && primCount != g_uiMyPrimCount)
	//		goto OVERJUMP;
	//	pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	//	pDevice->SetRenderState(D3DRS_ZENABLE,false);
	//	//pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	//	pDevice->SetTexture( 0, texGreen);
	//	pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	//	pDevice->SetRenderState(D3DRS_ZENABLE,true);
	//	//pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	//	pDevice->SetTexture( 0, texRed);
	//	pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	//}
	//OVERJUMP:
	return pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}


LPTHREAD_START_ROUTINE HookThread(LPVOID lpParam)
{
	HMODULE hModule = NULL;
	while(!hModule)
	{
		hModule = GetModuleHandleA("d3d9.dll");
		Sleep(100);
	}
	//DWORD* VTableStart = 0;
	//DWORD *dwVTableAddress = (PDWORD)0x0D78704;//Tools::dwFindPattern((DWORD)hModule, 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");


	////Tools::add_log("dwVTableAddress1: %X", dwVTableAddress);

	//memcpy(&VTableStart, (void*)(*dwVTableAddress+2), 4);

	////Tools::add_log("VTableStart: %X", VTableStart);

	//DWORD dwDrawIndexedPrimitive = (DWORD)VTableStart[82]; // für mehr: blick in die d3d9.h werfen!
	//DWORD dwEndScene = (DWORD)VTableStart[42];

	//char buffer[512];
	//sprintf(buffer, "dwVtableaddress: %X | vtablestart: %X | %X | %X", *dwVTableAddress, VTableStart, dwDrawIndexedPrimitive, dwEndScene);
	//MessageBox(NULL, buffer, "info", NULL);

	//Tools::add_log("%X | %X", dwDrawIndexedPrimitive, dwEndScene);

	pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)Tools::DetourFunc((PBYTE)0x10029760, (PBYTE)hkDrawIndexedPrimitive, 5);
	pEndScene = (EndScene_t)Tools::DetourFunc((PBYTE)0x1001F8DF,(PBYTE)hkEndScene, 5);

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HookThread, NULL, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

