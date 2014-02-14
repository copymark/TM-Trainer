#pragma once
#include "DllGlobals.h"
#include "cHack.h"
#include "CodeChange.h"

class _declspec(dllexport) CNop
{
public:
	CNop(void);
	~CNop(void);

	void Enable(void);

	void Disable(void);

	void Initialize(cHack *pHack, UINT_PTR uiAddress, SIZE_T nSize);

	bool IsEnabled(void);

private:

	CCodeChange m_CodeChange;
};

