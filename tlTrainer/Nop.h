#pragma once
#include "DllGlobals.h"
#include "cHack.h"
#include "CodeChange.h"

class _declspec(dllexport) CNop : public CHelper
{
public:
	CNop(void);
	~CNop(void);

	void Initialize(cHack *pHack, UINT_PTR uiAddress, SIZE_T nSize);

	bool IsEnabled(void);

private:
	void EnableFeature(void);
	void DisableFeature(void);
	CCodeChange m_CodeChange;
};

