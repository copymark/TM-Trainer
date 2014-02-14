#pragma once

#include "DllGlobals.h"
#include "cHack.h"

class _declspec(dllexport) CCodeChange
{
public:
	CCodeChange(void);
	~CCodeChange(void);

	/*
	Setzt die ganzen Werte
	*/
	void Initialize(cHack *pHack, UINT_PTR uiAddress, BYTE *pbNewCode, SIZE_T nSize);

	/*
	Überschreibt das Original
	*/
	void Enable(void);

	/*
	Stellt Original wieder her
	*/
	void Disable(void);

	/*
	Gibt true zurück wenn aktiviert
	*/
	bool IsEnabled(void);

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

	/*
	Pointer zum Hackobjekt um Schreib/Lese Operationen durchuführen
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

