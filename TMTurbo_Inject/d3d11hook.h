#pragma once

#include <Windows.h> 
#include <d3d11.h> 

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "./FW1FontWrapper_1_1/x86/FW1FontWrapper.lib")

#include "./FW1FontWrapper_1_1/FW1FontWrapper.h"

typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);


HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);

const void* __cdecl DetourFunc(BYTE* src, const BYTE* dest, const DWORD length);

DWORD __stdcall InitializeD3D11Hook(LPVOID);

void onD3D11Detach();