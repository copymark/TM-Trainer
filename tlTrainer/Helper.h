#pragma once
#include "DllGlobals.h"
#include "cHack.h"

class __declspec(dllexport) CHelper
{
public:
	CHelper();
	~CHelper();

	/*
	Gibt true zurück wenn aktiviert
	*/
	bool IsEnabled(void) const;

	void SwitchStatus(void);

	void Enable(void);
	void Disable(void);

protected:
	bool IsInitialized(void) const;
	void setIntializedStatus(bool status);

	cHack *GetHack(void) const;
	void SetHack(cHack *pHack);

private:
	virtual void EnableFeature(void) = NULL;
	virtual void DisableFeature(void) = NULL;

	/*
	Pointer zum Hackobjekt um Schreib/Lese Operationen durchzuführen
	*/
	cHack *m_pHack;

	/*
	Wenn gerade aktiviert
	*/
	bool m_bActivated;

	/*
	Wenn schon initialisiert
	*/
	bool m_bInitialized;
};

