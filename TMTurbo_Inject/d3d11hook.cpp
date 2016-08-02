#include "d3d11hook.h"

ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;

DWORD* pSwapChainVtable = NULL;
DWORD* pDeviceContextVTable = NULL;

D3D11PresentHook phookD3D11Present = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11ClearRenderTargetViewHook phookD3D11ClearRenderTargetView = NULL;

IFW1Factory *pFW1Factory = NULL;
IFW1FontWrapper *pFontWrapper = NULL;

bool firstTime = true;
void* detourBuffer = NULL;


HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (firstTime)
	{
		pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		FW1CreateFactory(FW1_VERSION, &pFW1Factory);
		pFW1Factory->CreateFontWrapper(pDevice, L"Tahoma", &pFontWrapper);

		pFW1Factory->Release();

		firstTime = false;
	}

	pFontWrapper->DrawString(pContext, L"D3D11 Hook by evolution536", 10.0f, 16.0f, 16.0f, 0xffff1612, FW1_RESTORESTATE);

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4])
{
	return phookD3D11ClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);
}

const void* __cdecl DetourFunc(BYTE* src, const BYTE* dest, const DWORD length)
{
	BYTE* jump = new BYTE[length + 5];
	detourBuffer = jump;

	DWORD dwVirtualProtectBackup;
	VirtualProtect(src, length, PAGE_READWRITE, &dwVirtualProtectBackup);

	memcpy(jump, src, length);
	jump += length;

	jump[0] = 0xE9;
	*(DWORD*)(jump + 1) = (DWORD)(src + length - jump) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dest - src) - 5;

	VirtualProtect(src, length, dwVirtualProtectBackup, &dwVirtualProtectBackup);

	return jump - length;
}

DWORD __stdcall InitializeD3D11Hook(LPVOID)
{
	HWND hWnd = GetForegroundWindow();
	IDXGISwapChain* pSwapChain;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = ((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1
											 , D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
	{
		MessageBox(hWnd, "Failed to create directX device and swapchain!", "uBoos?", MB_ICONERROR);
		return NULL;
	}

	pSwapChainVtable = (DWORD*)pSwapChain;
	pSwapChainVtable = (DWORD*)pSwapChainVtable[0];

	pDeviceContextVTable = (DWORD*)pContext;
	pDeviceContextVTable = (DWORD*)pDeviceContextVTable[0];

	phookD3D11Present = (D3D11PresentHook)DetourFunc((BYTE*)pSwapChainVtable[8], (BYTE*)hookD3D11Present, 5);
	phookD3D11DrawIndexed = (D3D11DrawIndexedHook)DetourFunc((BYTE*)pDeviceContextVTable[12], (BYTE*)hookD3D11DrawIndexed, 5);
	phookD3D11ClearRenderTargetView = (D3D11ClearRenderTargetViewHook)DetourFunc((BYTE*)pDeviceContextVTable[50], (BYTE*)hookD3D11ClearRenderTargetView, 5);

	DWORD dwOld;
	VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld);

	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

	return NULL;
}

void onD3D11Detach()
{
	if (pFontWrapper)
	{
		pFontWrapper->Release();
	}

	if (detourBuffer)
	{
		delete[] detourBuffer;
	}
}
