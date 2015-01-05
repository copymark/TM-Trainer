#pragma once

#include "DllGlobals.h"
#include "cHack.h"
#include "Helper.h"

class _declspec(dllexport) CCodeChange : public CHelper
{
public:
	CCodeChange(void);
	~CCodeChange(void);

	/*
	Setzt die ganzen Werte
	*/
	void Initialize(cHack *pHack, UINT_PTR uiAddress, BYTE *pbNewCode, SIZE_T nSize);

	/*
	Konvertiert von Big Endian in Little Endian oder umgekehrt
	*/
	void ConvertEndians(LPVOID pInput, BYTE *pOutput, SIZE_T nSize);

	/*
	Setzt den neuen Code
	*/
	void SetNewCode(BYTE *pbNewCode);

private:
	/*
	Überschreibt das Original
	*/
	virtual void EnableFeature(void);

	/*
	Stellt Original wieder her
	*/
	virtual void DisableFeature(void);

	/*
	Addresse an die geschrieben werden soll
	*/
	UINT_PTR m_uiAddress;

	/*
	Wieviele Bytes überschrieben werden sollen
	*/
	SIZE_T m_nSize;

	/*
	Das Original an der Stelle der Adresse zum wiederherstellen
	*/
	BYTE *m_pbOriginal;

	/*
	Neuer Code der an die Addresse geschrieben wird
	*/
	BYTE *m_pbNewCode;
};

